/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */ /*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/02 16:42:55 by kyazdani          #+#    #+#             */ /*   Updated: 2019/10/04 15:55:01 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

static void	copy_memory(void *to, void *from, size_t n)
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

void	*actual_realloc(void *ptr, size_t size)
{
    t_alloc     *header;
    void        *new_value;
    size_t      aligned_size;

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

    if (aligned_size <= header->size)
        return (ptr);

    // if the next bloc is free and the size of current + next is more or equals to the new size
    if (header->next && header->next->free && header->next->zone == header->zone
        && header->next->size + HEADER + header->size >= aligned_size)
    {
        // |H|__|H|___|H|... > |H|_______|H|...
        header->size += HEADER + header->next->size;
        header->next = header->next->next;
        return (ptr);
    }

    new_value = malloc(aligned_size);
    copy_memory(new_value, ptr, header->size);
    free(ptr);
    return (new_value);
}

void    *realloc(void *ptr, size_t size)
{
    void    *ret;

    pthread_mutex_lock(&g_mutex);
    ret = actual_realloc(ptr, size);
    pthread_mutex_unlock(&g_mutex);
    return (ret);
}
