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

inline static void	*_realloc(void *ptr, size_t size)
{
    t_alloc     *header;
    t_alloc     *next;
    void        *new_ptr;

    if (!(header = get_header_from_addr(ptr)))
        return (ptr);

    size = get_multiple_of(size, 16);

    if (size <= header->size)
        return (ptr);

    next = header->next;
    if (should_resize(header, next, size))
    {
        resize_alloc(header, next, size);
        return (ptr);
    }

    if (!(new_ptr = _malloc(size)))
        return (ptr);
    copy_memory(new_ptr, ptr, header->size);
    _free(ptr);

    return (new_ptr);
}

void                *realloc(void *ptr, size_t size)
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
