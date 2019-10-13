/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/02 16:42:55 by kyazdani          #+#    #+#             */
/*   Updated: 2019/10/04 15:55:01 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

void	*realloc(void *ptr, size_t size)
{
    t_alloc *header;
    size_t  aligned_size;
    size_t  new_next_size;

    if (!ptr)
        return malloc(size);

    if (!size)
    {
        free(ptr);
        return (malloc(0));
    }

    header = get_header_from_addr(ptr);
    if (!header)
        return (ptr);

    aligned_size = get_multiple_of(size, 16);
    if (header->size == aligned_size)
        return (ptr);
    if (header->size >= aligned_size + HEADER)
    {
        header->size = aligned_size;
        header->next = (void *)header + aligned_size;
        header->next->free = 1;
        header->next->size = header->size - aligned_size - HEADER;
        return (ptr);
    }
    if (header->next && header->next->free && header->next->size + HEADER + header->size >= aligned_size)
    {
        new_next_size = header->next->size - (aligned_size - header->size);
        header->size = aligned_size;
        header->next = (void *)header + HEADER + aligned_size;
        header->next->size = new_next_size;
        return (ptr);
    }

    free(ptr);
    return (malloc(size));
}
