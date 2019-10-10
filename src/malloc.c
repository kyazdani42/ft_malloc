/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/02 16:42:46 by kyazdani          #+#    #+#             */
/*   Updated: 2019/10/05 14:53:29 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

inline static size_t    get_size(size_t len)
{
    size_t  pagesize;

    pagesize = getpagesize();
    if (len % pagesize == 0)
        return len;
    return pagesize * (len / pagesize + 1);
}

void    *new_zone(t_alloc **ptr, size_t size, size_t type_size)
{
    size_t  mmap_size;
    void    *ret;

    mmap_size = get_size((type_size + HEADER) * (type_size + HEADER <= SMALL ? 100 : 1));

    ret = mmap(NULL, mmap_size, PROT_READ | PROT_WRITE,
            MAP_ANON | MAP_PRIVATE, -1, 0);
    if (ret == MAP_FAILED)
        return NULL;
    *ptr = ret;
    (*ptr)->size = size;
    (*ptr)->free = 0;

    if (size <= SMALL)
    {
        (*ptr)->next = (void *)*ptr + HEADER + size + 1; // + alignment ?
        (*ptr)->next->size = mmap_size - (HEADER + size);
        (*ptr)->next->free = 1;
        (*ptr)->next->next = NULL;
    } else
        (*ptr)->next = NULL;

    return (void *)*ptr + HEADER + 1;
}

void                    *allocate(t_alloc **ptr, size_t size, size_t type)
{
    t_alloc *tmp;
    t_alloc *new_alloc;

    if (!*ptr)
        return new_zone(ptr, size, type);

    tmp = *ptr;
    while (tmp->next && (!tmp->free || (tmp->free && tmp->size < size + HEADER)))
        tmp = tmp->next;

    if (!tmp->free || (tmp->free && tmp->size < size + HEADER))
        return new_zone(&tmp->next, size, type);

    tmp->free = 0;
    if (tmp->size < size + HEADER * 2)
        return (void *)tmp + HEADER + 1;

    new_alloc = (void *)tmp + size + HEADER + 1;
    new_alloc->free = 1;
    new_alloc->size = tmp->size - (size + HEADER + 1);
    new_alloc->next = tmp->next;
    tmp->size = size;
    tmp->next = new_alloc;

    return (void *)tmp + HEADER + 1;
}

void                    *malloc(size_t size)
{
    static int  initialization = 1;

    if (initialization)
    {
        g_state.tiny = NULL;
        g_state.small = NULL;
        g_state.large = NULL;
        initialization = 0;
    }

    if (size <= TINY)
        return (allocate(&g_state.tiny, size, TINY));
    else if (size <= SMALL)
        return (allocate(&g_state.small, size, SMALL));
    else
        return allocate(&g_state.large, size, size);
}

