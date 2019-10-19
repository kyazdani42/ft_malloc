/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/02 15:26:52 by kyazdani          #+#    #+#             */
/*   Updated: 2019/10/05 14:44:48 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

inline static void     defrag(t_alloc **e, t_alloc **p, t_alloc *prevprev) {
    t_alloc     *next;
    t_alloc     *prev;

    prev = *p;
    if (prev && prev->free && prev->zone == (*e)->zone)
    {
        prev->size += HEADER + (*e)->size;
        prev->next = (*e)->next;
        *e = prev;
        *p = prevprev;
    }
    next = (*e)->next;
    while (next && next->free && next->zone == (*e)->zone)
    {
        (*e)->size += HEADER + next->size;
        next = next->next;
        (*e)->next = next;
    }
}

inline static int    loop_zone(void *ptr, t_alloc *zone, t_alloc **cur, t_alloc **prev, t_alloc **prevprev)
{
    *cur = zone;
    *prev = NULL;
    *prevprev = NULL;
    while (*cur)
    {
        if ((void *)*cur + HEADER == ptr)
            return (1);
        *prevprev = *prev;
        *prev = *cur;
        *cur = (*cur)->next;
    }
    return (0);
}

inline static t_alloc  **set_zone_and_elements(void *ptr, t_alloc **cur, t_alloc **prev, t_alloc **prevprev)
{
    if (loop_zone(ptr, g_state.large, cur, prev, prevprev))
        return &g_state.large;
    if (loop_zone(ptr, g_state.small, cur, prev, prevprev))
        return &g_state.small;
    if (loop_zone(ptr, g_state.tiny, cur, prev, prevprev))
        return &g_state.tiny;
    return (NULL);
}

void  _free(void *ptr)
{
    t_alloc     *cur;
    t_alloc     *prev;
    t_alloc     *tmp;
    t_alloc     **zone;

    if (!ptr || !(zone = set_zone_and_elements(ptr, &cur, &prev, &tmp)))
        return;
    cur->free = 1;
    defrag(&cur, &prev, tmp);
    tmp = cur->next;
    if (((!prev && !tmp) || (!prev && tmp && tmp->zone != cur->zone) ||
                (!tmp && prev && prev->zone != cur->zone) ||
                (tmp && prev && prev->zone != cur->zone && tmp->zone != cur->zone)))
    {
        if (!prev)
            *zone = cur->next;
        else
            prev->next = cur->next;
        if (munmap(cur, cur->size + HEADER) == -1)
            return;
    }
}

void free(void *ptr)
{
    pthread_mutex_lock(&g_mutex);
    _free(ptr);
    pthread_mutex_unlock(&g_mutex);
}

