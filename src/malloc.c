/* ************************************************************************** */
/*                                                                            */ /*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 14:25:22 by kyazdani          #+#    #+#             */
/*   Updated: 2019/10/19 14:25:23 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

static void		*new_zone(t_alloc *cur, size_t size, size_t zone_size)
{
    size_t		mmap_size;
    t_alloc		*new;
    t_alloc		*ptr;

    mmap_size = get_multiple_of(zone_size, PS);
    ptr = mmap(0, mmap_size, PROT, FLAGS, -1, 0);
    if (ptr == MAP_FAILED)
        return (NULL);

    ptr->size = size;
    ptr->free = 0;
    ptr->zone = cur == NULL ? 0 : cur->zone + 1;
    ptr->prev = cur;
    if (cur)
        cur->next = ptr;
    if (mmap_size < HEADER * 2 + size + 16)
    {
        ptr->size = mmap_size - HEADER;
        ptr->next = NULL;
        return (ptr);
    }
    new = (void *)ptr + HEADER + size;
    ptr->next = new;

    new->size = mmap_size - (HEADER * 2 + size);
    new->zone = cur == NULL ? 0 : cur->zone + 1;
    new->free = 1;
    new->next = NULL;
    new->prev = ptr;

    return (ptr);
}

static void		create_free_block(t_alloc *cur, size_t size)
{
    t_alloc     *new;

    new = (void *)cur + HEADER + size;
    new->free = 1;
    new->size = cur->size - (HEADER + size);
    new->zone = cur->zone;
    new->next = cur->next;
    new->prev = cur;
    cur->next = new;
    cur->size = size;
}

static void		*allocate(t_alloc **zone, size_t size, size_t zone_size)
{
    t_alloc *block;
    t_alloc *new;

    if (!*zone) {
        if (!(*zone = new_zone(NULL, size, zone_size)))
            return (NULL);
        return ((void *)*zone + HEADER);
    }

    block = *zone;

    while (block->next && (!block->free || block->size < size))
        block = block->next;

    if (!block->free || block->size < size) {
        if (!(new = new_zone(block, size, zone_size)))
            return (NULL);
        return ((void *)new + HEADER);
    }

    block->free = 0;

    if (block->size < size + HEADER + 16)
        return ((void *)block + HEADER);
    create_free_block(block, size);

    return ((void *)block + HEADER);
}

void			*malloc_unthread(size_t size)
{
    size_t	aligned_size;

    aligned_size = get_multiple_of(size, 16);
    if (aligned_size <= TINY)
        return (allocate(&g_state.tiny, aligned_size, TINY_ZONE));
    else if (aligned_size <= SMALL)
        return (allocate(&g_state.small, aligned_size, SMALL_ZONE));
    else
        return (allocate(&g_state.large, aligned_size, aligned_size + HEADER));
}

void			*malloc(size_t size)
{
    static int		initialization = 1;
    void			*ret;

	initialize_mutex();
    if (initialization)
    {
        g_state.tiny = NULL;
        g_state.small = NULL;
        g_state.large = NULL;
        initialization = 0;
	}
    ret = malloc_unthread(size);
    pthread_mutex_unlock(&g_mutex);
    return (ret);
}
