/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/02 2:42:46 by kyazdani           #+#    #+#             */
/*   Updated: 2019/10/05 14:53:29 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

static void    *new_zone(int zone_index, size_t size, size_t zone_size)
{
    size_t      mmap_size;
    size_t      next_size;
    t_alloc     *ret;

    mmap_size = get_multiple_of(zone_size, PS);
    ret = mmap(0, mmap_size, PROT, FLAGS, -1, 0);
    if (ret == MAP_FAILED)
        return NULL;

    ret->size = size;
    ret->free = 0;
    ret->zone = zone_index;

    if (!(next_size = mmap_size - (HEADER * 2 + size)))
    {
        ret->next = NULL;
        return ret;
    }

    ret->next = (void *)ret + HEADER + size;
    ret->next->size = next_size;
    ret->next->zone = ret->zone;
    ret->next->free = 1;
    ret->next->next = NULL;

    return ret;
}

#include <stdio.h>
static void                    *allocate(t_alloc **ptr, size_t size, size_t zone_size)
{
    t_alloc     *alloc;
    t_alloc     *new;

    show_alloc_mem();
    if (!*ptr)
    {
        *ptr = new_zone(0, size, zone_size);
        return (void *)*ptr + HEADER;
    }

    alloc = *ptr;
    while (alloc->next && (!alloc->free || alloc->size < size + HEADER))
        alloc = alloc->next;

    if (!alloc->free || alloc->size < size + HEADER)
    {
        new = new_zone(alloc->zone + 1, size, zone_size);
        alloc->next = new;
        return (void *)new + HEADER;
    }
    putstr("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");

    alloc->free = 0;
    if (alloc->size < size + HEADER * 2)
        return (void *)alloc + HEADER;

    new = (void *)alloc + HEADER + size;
    new->size = alloc->size - (size + HEADER);
    new->zone = alloc->zone;
    new->next = alloc->next;
    new->free = 1;

    alloc->size = size;
    alloc->next = new;

    show_alloc_mem();
    putstr("\n\n\n");
    usleep(500000);
    return (void *)alloc + HEADER;
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
