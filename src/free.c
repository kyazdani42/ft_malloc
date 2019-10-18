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

inline static void     defrag(t_alloc **e, t_alloc *prev) {
    t_alloc     *next;

    if (prev && prev->free && prev->zone == (*e)->zone)
    {
        prev->size += HEADER + (*e)->size;
        prev->next = (*e)->next;
        *e = prev;
    }
    next = (*e)->next;
    while (next && next->free && next->zone == (*e)->zone)
    {
        (*e)->size += HEADER + next->size;
        next = next->next;
        (*e)->next = next;
    }
}

inline static int   should_munmap(t_alloc *elem, t_alloc *prev)
{
    t_alloc     *next;

    next = elem->next;
    if (!prev && !next)
        return (1);
    if (!prev)
        return next->zone != elem->zone ? 1 : 0;
    if (!next)
        return prev->zone != elem->zone ? 1 : 0;
    return (elem->zone != prev->zone && elem->zone != next->zone ? 1 : 0);
}

t_alloc    *get_prev(t_alloc *zone, t_alloc *elem)
{
    t_alloc *tmp;

    tmp = zone;
    if (zone == elem)
        return (NULL);
    while (tmp && tmp->next && tmp->next != elem)
        tmp = tmp->next;
    return (tmp);
}

inline static int    loop_zone(void *ptr, t_alloc *zone, t_alloc **cur, t_alloc **prev)
{
    *cur = zone;
    *prev = NULL;
    while (*cur)
    {
        if ((void *)*cur + HEADER == ptr)
            return (1);
        *prev = *cur;
        *cur = (*cur)->next;
    }
    *prev = NULL;
    *cur = NULL;
    return (0);
}

inline static t_alloc  **set_zone_and_elements(void *ptr, t_alloc **cur, t_alloc **prev)
{
    if (loop_zone(ptr, g_state.large, cur, prev))
        return &g_state.large;
    if (loop_zone(ptr, g_state.small, cur, prev))
        return &g_state.small;
    if (loop_zone(ptr, g_state.tiny, cur, prev))
        return &g_state.tiny;
    return (NULL);
}

void  _free(void *ptr)
{
    t_alloc     *cur;
    t_alloc     *prev;
    t_alloc     **zone;

    if (!ptr)
        return;

    if (!(zone = set_zone_and_elements(ptr, &cur, &prev)))
        return;

    cur->free = 1;
    defrag(&cur, prev);
    prev = get_prev(*zone, cur);
    if (should_munmap(cur, prev))
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

