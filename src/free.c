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

static void     defrag(t_alloc **elem)
{
    t_alloc *prev;
    t_alloc *next;

    prev = (*elem)->prev;
    next = (*elem)->next;
    while (prev && prev->free && prev->zone == (*elem)->zone)
    {
        prev->size += HEADER + (*elem)->size;
        prev->next = (*elem)->next;
        if (prev->next)
            prev->next->prev = prev;
        *elem = prev;
        prev = prev->prev;
    }
    while (next && next->free && next->zone == (*elem)->zone)
    {
        (*elem)->next = next->next;
        (*elem)->size += HEADER + next->size;
        next = next->next;
        if (next)
            next->prev = *elem;
    }
}

static t_alloc  **get_zone(t_alloc *elem)
{
    t_alloc *tmp;

    tmp = elem;
    while (tmp->prev)
        tmp = tmp->prev;
    if (tmp == g_state.tiny)
        return &g_state.tiny;
    else if (tmp == g_state.small)
        return &g_state.small;
    else if (tmp == g_state.large)
        return &g_state.large;
    else
        return (NULL);
}

void  _free(void *ptr)
{
    t_alloc     *elem;
    t_alloc     *previous;
    t_alloc     **zone;

    if (!ptr)
        return;

    elem = get_header_from_addr(ptr);
    if (elem == NULL)
        return;

    elem->free = 1;
    defrag(&elem);

    if ((
                !elem->prev && !elem->next)
            || (!elem->prev && elem->next->zone != elem->zone)
            || (!elem->next && elem->prev->zone != elem->zone)
            || (elem->prev->zone != elem->zone && elem->next->zone != elem->zone
               ))
    {
        previous = elem->prev;
        if (previous)
            previous->next = elem->next;
        else
        {
            if (!(zone = get_zone(elem)))
                return;
            *zone = elem->next;
        }
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

