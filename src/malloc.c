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

static void    *new_zone(t_alloc **ptr, t_alloc *prev, size_t size, size_t zone_size)
{
    size_t      mmap_size;
    size_t      next_size;
    void        *ret;
    t_alloc     *next;

    mmap_size = get_multiple_of(zone_size, PS);
    ret = mmap(0, mmap_size, PROT, FLAGS, -1, 0);
    if (ret == MAP_FAILED)
        return NULL;

    *ptr = ret;
    (*ptr)->prev = prev;
    (*ptr)->size = size;
    (*ptr)->free = 0;
    (*ptr)->zone = prev ? prev->zone + 1 : 0;
    if (prev)
        prev->next = *ptr;

    if (!(next_size = mmap_size - (HEADER * 2 + size)))
    {
        (*ptr)->next = NULL;
        return (void *)*ptr + HEADER;
    }

    (*ptr)->next = (void *)*ptr + HEADER + size;
    next = (*ptr)->next;
    next->size = next_size;
    next->zone = (*ptr)->zone;
    next->free = 1;
    next->prev = *ptr;
    next->next = NULL;

    return (void *)*ptr + HEADER;
}

static void                    *allocate(t_alloc **ptr, size_t size, size_t zone_size)
{
    t_alloc     *tmp;
    t_alloc     *next;
    t_alloc     *new_alloc;

    if (!*ptr)
        return new_zone(ptr, NULL, size, zone_size);

    tmp = *ptr;
    while (tmp->next && (!tmp->free || tmp->size < size + HEADER))
        tmp = tmp->next;

    if (!tmp->free || tmp->size < size + HEADER)
        return new_zone(&tmp->next, tmp, size, zone_size);

    tmp->free = 0;
    if (tmp->size < size + HEADER * 2)
        return (void *)tmp + HEADER;

    new_alloc = (void *)tmp + HEADER + size;
    new_alloc->size = tmp->size - (size + HEADER);
    new_alloc->zone = tmp->zone;
    new_alloc->next = tmp->next;
    next = new_alloc->next;
    if (next)
        next->prev = new_alloc;
    new_alloc->prev = tmp;
    new_alloc->free = 1;

    tmp->size = size;
    tmp->next = new_alloc;
    return (void *)tmp + HEADER;
}

void                    *_malloc(size_t size)
{
    size_t  aligned_size;

    aligned_size = get_multiple_of(size, 16);
    if (aligned_size <= TINY)
        return (allocate(&g_state.tiny, aligned_size, TINY_ZONE));
    else if (aligned_size <= SMALL)
        return (allocate(&g_state.small, aligned_size, SMALL_ZONE));
    else
        return (allocate(&g_state.large, aligned_size, aligned_size + HEADER));
}

void                    *malloc(size_t size)
{
    static int      initialization = 1;
    void            *ret;

    if (initialization)
        pthread_mutex_init(&g_mutex, NULL);

    pthread_mutex_lock(&g_mutex);
    if (initialization)
    {
        g_state.tiny = NULL;
        g_state.small = NULL;
        g_state.large = NULL;
        initialization = 0;
    }

    ret = _malloc(size);
    pthread_mutex_unlock(&g_mutex);
    return ret;
}
