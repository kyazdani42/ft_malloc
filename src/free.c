/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/24 15:13:43 by kyazdani          #+#    #+#             */
/*   Updated: 2019/10/24 15:13:44 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

inline static int       should_munmap(t_alloc *cur)
{
    t_alloc     *next;
    t_alloc     *prev;

    next = cur->next;
    prev = cur->prev;
    return ((!prev && !next) || (!prev && next && next->zone != cur->zone) ||
            (!next && prev && prev->zone != cur->zone) ||
            (next && prev && prev->zone != cur->zone && next->zone != cur->zone));
}

static void             remove_zone(t_alloc **zone, t_alloc *to_remove)
{
    t_alloc     *prev;
    t_alloc     *next;

    prev = to_remove->prev;
    next = to_remove->next;
    if (prev)
    {
        prev->next = to_remove->next;
        if (next)
            next->prev = prev;
    }
    else
    {
        *zone = (*zone)->next;
        if (*zone)
            (*zone)->prev = NULL;
    }
    munmap((char *)to_remove, to_remove->size + HEADER);
}

static void             defrag(t_alloc **cur)
{
    t_alloc     *prev;
    t_alloc     *next;

    prev = (*cur)->prev;
    next = (*cur)->next;
    while (prev && prev->free && prev->zone == (*cur)->zone)
    {
        prev->size += HEADER + (*cur)->size;
        prev->next = next;
        if (next)
            next->prev = prev;
        *cur = prev;
        prev = prev->prev;
    }
    while (next && next->free && next->zone == (*cur)->zone)
    {
        (*cur)->size += HEADER + next->size;
        next = next->next;
        (*cur)->next = next;
        if (next)
            next->prev = *cur;
    }
}

void                    free_unthread(void *ptr)
{
    t_alloc     *cur;
    t_alloc     **zone;

    if (!ptr)
        return ;
    if (!(zone = get_zone_set_cur(ptr, &cur)))
        return ;
    cur->free = 1;
    defrag(&cur);
    if (should_munmap(cur))
        remove_zone(zone, cur);
}

void                    free(void *ptr)
{
    initialize_mutex();
    free_unthread(ptr);
    pthread_mutex_unlock(&g_mutex);
}
