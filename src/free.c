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

    if (!ptr)
        return;

    elem = ptr - HEADER - 1;
    next = elem->next;
    prev = elem;
    // SIGSEGV when elem is first in the list
    // might consider implementing a pointer to the previous element in t_alloc
    while (prev && prev->next != elem)
        prev = (void *)prev - 1;

    if ((elem->size + HEADER) % getpagesize() == 0)
    {
        if (munmap(elem, elem->size + HEADER) == -1)
            return;
        prev->next = next;
    }

}
