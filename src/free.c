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

void free(void *ptr)
{
    t_alloc     *elem;
    t_alloc     *prev;
    t_alloc     *next;
    size_t      size;

    if (!ptr)
        return;

    elem = ptr - HEADER - 1;
    size = elem->size;
    next = elem->next;
    prev = elem->prev;

    elem->free = 1;
    if (prev && prev->free)
    {
        prev->size += HEADER + elem->size;
        prev->next = next;
        elem = prev;
    }
    if (next && next->free)
    {
        elem->next = next->next;
        elem->size += HEADER + next->size;
        if (elem->next)
            elem->next->prev = elem;
    }

    if ((elem->size + HEADER) % getpagesize() == 0)
    {

        if (munmap(elem, elem->size + HEADER) == -1)
            return;
        if (size <= TINY)
            g_state.tiny = NULL;
        else if (size <= SMALL)
            g_state.small = NULL;
        else
            g_state.large = NULL;
    }
}

