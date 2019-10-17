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

inline static t_alloc  **get_zone(t_alloc *elem)
{
    t_alloc *tmp;

    tmp = g_state.tiny;
    while (tmp && tmp->next)
    {
        if (tmp == elem)
            return &g_state.tiny;
        tmp = tmp->next;
    }
    tmp = g_state.small;
    while (tmp && tmp->next)
    {
        if (tmp == elem)
            return &g_state.small;
        tmp = tmp->next;
    }
    tmp = g_state.large;
    while (tmp && tmp->next)
    {
        if (tmp == elem)
            return &g_state.large;
        tmp = tmp->next;
    }

    return (NULL);
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

t_alloc  *get_header(void *ptr, t_alloc **prev)
{
    t_alloc *tmp;

    tmp = g_state.large;
    *prev = NULL;
    while (tmp)
    {
        if ((void *)tmp + HEADER == ptr)
            return (tmp);
        *prev = tmp;
        tmp = tmp->next;
    }
    tmp = g_state.small;
    *prev = NULL;
    while (tmp)
    {
        if ((void *)tmp + HEADER == ptr)
            return (tmp);
        *prev = tmp;
        tmp = tmp->next;
    }
    tmp = g_state.tiny;
    *prev = NULL;
    while (tmp)
    {
        if ((void *)tmp + HEADER == ptr)
            return (tmp);
        *prev = tmp;
        tmp = tmp->next;
    }
    *prev = NULL;
    return (NULL);
}



void  _free(void *ptr)
{
    t_alloc     *elem;
    t_alloc     *prev;
    t_alloc     **zone;

    if (!ptr)
        return;

    if (!(elem = get_header(ptr, &prev)))
        return;

    if (!(zone = get_zone(elem)))
        return;

    elem->free = 1;

    defrag(&elem, prev);
    // this is cost expensive...
    // consider putting a prev element back
    prev = get_prev(*zone, elem);
    if (should_munmap(elem, prev))
    {
        if (!prev)
            *zone = elem->next;
        else
            prev->next = elem->next;
        if (munmap(elem, elem->size + HEADER) == -1)
            return;
    }
}

void free(void *ptr)
{
    pthread_mutex_lock(&g_mutex);
    _free(ptr);
    pthread_mutex_unlock(&g_mutex);
}

