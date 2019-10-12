/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/02 2:42:46 by kyazdani          #+#    #+#             */
/*   Updated: 2019/10/05 14:53:29 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

inline static size_t    get_multiple_of(size_t len, size_t alignment) {
    if (len % alignment == 0)
        return len;
    return alignment * (len / alignment + 1);
}

static void    *new_zone(t_alloc **ptr, t_alloc *prev, size_t size, size_t type_size)
{
    size_t  mmap_size;
    size_t  next_size;
    size_t  aligned_size;
    void    *ret;

    mmap_size = get_multiple_of((type_size + HEADER) * (size <= SMALL ? 100 : 1), getpagesize());
    ret = mmap(0, mmap_size, PROT, FLAGS, -1, 0);
    if (ret == MAP_FAILED)
        return NULL;

    *ptr = ret;
    (*ptr)->prev = prev;
    (*ptr)->size = size;
    (*ptr)->free = 0;

    aligned_size = get_multiple_of(size, 2);
    next_size = mmap_size - (HEADER * 2 + aligned_size);
    if (!next_size)
    {
        (*ptr)->next = NULL;
        return (void *)*ptr + HEADER;
    }

    (*ptr)->next = (void *)*ptr + HEADER + aligned_size;
    (*ptr)->next->size = next_size;
    (*ptr)->next->free = 1;
    (*ptr)->next->next = NULL;
    (*ptr)->next->prev = *ptr;

    return (void *)*ptr + HEADER;
}

static void                    *allocate(t_alloc **ptr, size_t size, size_t type)
{
    t_alloc *tmp;
    t_alloc *new_alloc;

    if (!*ptr)
        return new_zone(ptr, NULL, size, type);

    tmp = *ptr;
    while (tmp->next && (!tmp->free || tmp->size < size + HEADER))
        tmp = tmp->next;

    if (!tmp->free || tmp->size < size + HEADER)
        return new_zone(&tmp->next, tmp, size, type);

    tmp->free = 0;

    // if the current size is smaller than a header a bloc and another header
    if (tmp->size < get_multiple_of(size, 2) + HEADER * 2)
        return (void *)tmp + HEADER;

    // the next free bloc is header and block away from current header
    new_alloc = (void *)tmp + HEADER + get_multiple_of(size, 2);
    new_alloc->free = 1;
    // the next free bloc size is the current block size minus the new size and the header size
    new_alloc->size = tmp->size - (get_multiple_of(size, 2) + HEADER);
    new_alloc->next = tmp->next;
    new_alloc->prev = tmp;
    if (new_alloc->next)
        new_alloc->next->prev = new_alloc;

    tmp->size = size;
    tmp->next = new_alloc;

    return (void *)tmp + HEADER;
}

void                    *malloc(size_t size)
{
    static int      initialization = 1;
    struct rlimit   limits;

    if (initialization)
    {
        g_state.tiny = NULL;
        g_state.small = NULL;
        g_state.large = NULL;
        initialization = 0;
    }

    getrlimit(RLIMIT_DATA, &limits);

    if (size <= TINY)
        return (allocate(&g_state.tiny, size, TINY));
    else if (size <= SMALL)
        return (allocate(&g_state.small, size, SMALL));
    else
        return (allocate(&g_state.large, size, size));
}
