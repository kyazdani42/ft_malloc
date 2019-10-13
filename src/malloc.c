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

static void    *new_zone(t_alloc **ptr, t_alloc *prev, size_t size, size_t type_size)
{
    size_t  mmap_size;
    size_t  next_size;
    void    *ret;

    mmap_size = get_multiple_of(type_size * (size <= SMALL ? 100 : 1) + HEADER, getpagesize());
    ret = mmap(0, mmap_size, PROT, FLAGS, -1, 0);
    if (ret == MAP_FAILED)
        return NULL;
    *ptr = ret;
    (*ptr)->prev = prev;
    (*ptr)->size = size;
    (*ptr)->free = 0;

    next_size = mmap_size - (HEADER * 2 + size);
    if (!next_size)
    {
        (*ptr)->next = NULL;
        return (void *)*ptr + HEADER;
    }

    (*ptr)->next = (void *)*ptr + HEADER + size;
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
    if (tmp->size < size + HEADER * 2)
        return (void *)tmp + HEADER;

    new_alloc = (void *)tmp + HEADER + size;
    new_alloc->size = tmp->size - (size + HEADER);
    new_alloc->next = tmp->next;
    if (new_alloc->next)
        new_alloc->next->prev = new_alloc;
    new_alloc->prev = tmp;
    new_alloc->free = 1;

    tmp->size = size;
    tmp->next = new_alloc;
    return (void *)tmp + HEADER;
}

void                    *malloc(size_t size)
{
    static int      initialization = 1;
    static          pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    size_t          aligned_size;
    struct rlimit   limits;
    void            *ret;

    pthread_mutex_lock(&mutex);

    if (initialization)
    {
        g_state.tiny = NULL;
        g_state.small = NULL;
        g_state.large = NULL;
        initialization = 0;
    }

    getrlimit(RLIMIT_DATA, &limits);

    aligned_size = get_multiple_of(size, 16);
    if (aligned_size <= TINY)
        ret = allocate(&g_state.tiny, aligned_size, TINY);
    else if (aligned_size <= SMALL)
        ret = allocate(&g_state.small, aligned_size, SMALL);
    else
        ret = allocate(&g_state.large, aligned_size, aligned_size);

    pthread_mutex_unlock(&mutex);
    return ret;
}
