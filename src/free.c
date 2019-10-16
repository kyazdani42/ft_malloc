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

void free(void *ptr)
{
    t_alloc     *elem;
    t_alloc     *previous;

    if (!ptr) return;

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
        if (munmap(elem, elem->size + HEADER) == -1)
            return;
    }
}

