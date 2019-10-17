/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                    +:+ +:+         +:+     */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/02 16:42:55 by kyazdani          #+#    #+#             */
/*   Updated: 2019/10/04 15:55:01 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

static void	*_realloc(void *ptr, size_t size)
{
    t_alloc     *header;
    void        *new_ptr;
    size_t      aligned_size;

    header = get_header_from_addr(ptr);
    if (!header)
        return (ptr);

    aligned_size = get_multiple_of(size, 16);

    if (aligned_size <= header->size)
        return (ptr);

    // if the next bloc is free and the size of current + next is more or equals to the new size
    if (header->next && header->next->free
            && header->next->zone == header->zone
            && header->next->size + HEADER + header->size >= aligned_size)
    {
        // |H|__|H|___|H|... > |H|_______|H|...
        header->size += HEADER + header->next->size;
        header->next = header->next->next;
        return (ptr);
    }

    new_ptr = _malloc(aligned_size);
    copy_memory(new_ptr, ptr, header->size);
    _free(ptr);
    return (new_ptr);
}

void    *realloc(void *ptr, size_t size)
{
    void    *ret;

    if (!ptr)
        return malloc(size);

    if (!size)
    {
        free(ptr);
        return (malloc(0));
    }

    pthread_mutex_lock(&g_mutex);
    ret = _realloc(ptr, size);
    pthread_mutex_unlock(&g_mutex);
    return (ret);
}
