/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/02 16:42:41 by kyazdani          #+#    #+#             */
/*   Updated: 2019/10/04 18:25:34 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

static void defrag(t_alloc *elem, t_alloc *prev, t_alloc *next)
{
    while (prev && prev->free)
    {
        prev->size += HEADER + elem->size;
        prev->next = elem->next;
        if (prev->next)
            prev->next->prev = prev;
        elem = prev;
        prev = prev->prev;
    }
    while (next && next->free)
    {
        elem->next = next->next;
        elem->size += HEADER + next->size;
        if (elem->next)
            elem->next->prev = elem;
        next = next->next;
    }
}

static void    *get_zone(t_alloc *elem)
{
    t_alloc *tmp;

    tmp = elem;
    while (tmp->prev)
        tmp = tmp->prev;
    if (tmp == g_state.tiny)
        return &g_state.tiny;
    else if (tmp == g_state.small)
        return &g_state.small;
    else
        return &g_state.large;
}

static int      valid(void *ptr)
{
    t_alloc *tmp;

    tmp = g_state.large;
    while (tmp)
    {
        if ((void *)tmp + HEADER == ptr)
            return (0);
        tmp = tmp->next;
    }
    tmp = g_state.small;
    while (tmp)
    {
        if ((void *)tmp + HEADER == ptr)
            return (0);
        tmp = tmp->next;
    }
    tmp = g_state.tiny;
    while (tmp)
    {
        if ((void *)tmp + HEADER == ptr)
            return (0);
        tmp = tmp->next;
    }
    return (1);
}

void free(void *ptr)
{
    t_alloc     *elem;
    t_alloc     **state;

    if (!ptr) return;

    if (valid(ptr) == 1)
        return;
    elem = ptr - HEADER;
    elem->free = 1;
    defrag(elem, elem->prev, elem->next);
    if ((elem->size + HEADER) % getpagesize() == 0)
    {
        state = get_zone(elem);
        *state = elem->prev ? elem->prev : elem->next;

        if (munmap(elem, elem->size + HEADER) == -1)
            return;
    }
}

