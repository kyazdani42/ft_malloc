/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reallocf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 14:21:58 by kyazdani          #+#    #+#             */
/*   Updated: 2019/10/18 14:21:59 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

inline static void	*_reallocf(void *ptr, size_t size)
{
    t_alloc     *header;
    t_alloc     *next;
    void        *new_ptr;

    size = get_multiple_of(size, 16);
    if (!(header = get_header_from_addr(ptr)))
        return (NULL);

    if (size <= header->size)
        return (ptr);

    next = header->next;
    if (should_resize(header, next, size))
    {
        resize_alloc(header, next, size);
        return (ptr);
    }
    if (!(new_ptr = _malloc(size)))
    {
        _free(ptr);
        return (NULL);
    }
    copy_memory(new_ptr, ptr, header->size);
    _free(ptr);

    return (new_ptr);
}

void                *reallocf(void *ptr, size_t size)
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
    ret = _reallocf(ptr, size);
    pthread_mutex_unlock(&g_mutex);
    return (ret);
}
