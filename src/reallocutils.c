/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reallocutils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 14:18:10 by kyazdani          #+#    #+#             */
/*   Updated: 2019/10/18 14:18:13 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

int   should_resize(t_alloc *cur, t_alloc *next, size_t size)
{
    return  next
        && next->free
        && next->zone == cur->zone
        && cur->size + HEADER + next->size >= size;
}

void    resize_alloc(t_alloc *header, t_alloc *next, size_t size)
{
        size_t      next_size;

        next_size = next->size - (size - header->size);
        if (next_size < (HEADER + 16))
        {
            header->size += HEADER + next->size;
            header->next = next->next;
        }
        else
        {
            header->size = size;
            header->next = (void *)header + HEADER + size;
            header->next->next = next->next;
            next = header->next;
            next->free = 1;
            next->zone = header->zone;
            next->size = next_size;
        }
}

t_alloc  *get_header_from_addr(void *ptr)
{
    t_alloc *tmp;

    tmp = g_state.large;
    while (tmp)
    {
        if ((void *)tmp + HEADER == ptr)
            return (tmp);
        tmp = tmp->next;
    }
    tmp = g_state.small;
    while (tmp)
    {
        if ((void *)tmp + HEADER == ptr)
            return (tmp);
        tmp = tmp->next;
    }
    tmp = g_state.tiny;
    while (tmp)
    {
        if ((void *)tmp + HEADER == ptr)
            return (tmp);
        tmp = tmp->next;
    }
    return (NULL);
}

void	copy_memory(void *to, void *from, size_t n)
{
	char		*dest;
	char	    *src;

    dest = to;
    src = from;
	while (n--)
    {
		*dest = *src;
        dest++;
        src++;
    }
}
