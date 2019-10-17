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

inline static void     defrag(t_alloc **e, t_alloc **p)
{
    t_alloc     *next;
    t_alloc     *elem;
    t_alloc     *prev;

    prev = *p;
    elem = *e;

    while (prev && prev->free && prev->zone == elem->zone)
    {
        prev->size += HEADER + elem->size;
        prev->next = elem->next;
        elem = prev;
    }
    next = elem->next;
    while (next && next->free && next->zone == elem->zone)
    {
        elem->size += HEADER + next->size;
        next = next->next;
        elem->next = next;
    }
}

inline static t_alloc  **get_zone(t_alloc *elem)
{
    t_alloc *tmp;

    tmp = g_state.tiny;
    while (tmp->next)
    {
        if (tmp == elem)
            return &g_state.tiny;
        tmp = tmp->next;
    }
    tmp = g_state.small;
    while (tmp->next)
    {
        if (tmp == elem)
            return &g_state.small;
        tmp = tmp->next;
    }
    tmp = g_state.large;
    while (tmp->next)
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
    return (!prev && !next) ||
        (!prev && next->zone != elem->zone) ||
        (!next && prev->zone != elem->zone) ||
        (prev->zone != elem->zone && next->zone != elem->zone);
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

    elem->free = 1;
    if (!(zone = get_zone(elem)))
        return;

    defrag(&elem, &prev);
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

