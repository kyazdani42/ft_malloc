/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/23 21:39:23 by kyazdani          #+#    #+#             */
/*   Updated: 2019/10/23 21:39:25 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

static void		*new_zone(t_alloc *prev_zone, size_t size, size_t zone_size)
{
	t_alloc		*new_zone;

	zone_size = get_multiple_of(zone_size, PS);
	if ((new_zone = mmap(0, zone_size, PROT, FLAGS, -1, 0)) == MAP_FAILED)
		return (NULL);
	new_zone->size = size;
	new_zone->free = 0;
	new_zone->zone = prev_zone == NULL ? 0 : prev_zone->zone + 1;
	new_zone->prev = prev_zone;
	if (prev_zone)
		prev_zone->next = new_zone;
	if (zone_size < HEADER + size + HEADER + 16)
	{
		new_zone->size = zone_size - HEADER;
		new_zone->next = NULL;
		return (new_zone);
	}
	new_zone->next = (void *)new_zone + HEADER + size;
	new_zone->next->size = zone_size - (HEADER * 2 + size);
	new_zone->next->zone = new_zone->zone;
	new_zone->next->free = 1;
	new_zone->next->next = NULL;
	new_zone->next->prev = new_zone;
	return (new_zone);
}

static void		create_free_block(t_alloc *cur, size_t size)
{
	t_alloc		*new;

	cur->free = 0;
	if (cur->size < size + HEADER + 16)
		return ;
	new = (void *)cur + HEADER + size;
	new->size = cur->size - (HEADER + size);
	new->free = 1;
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
	t_alloc	*prev;

	if (!*zone)
	{
		if (!(*zone = new_zone(NULL, size, zone_size)))
			return (NULL);
		return ((void *)*zone + HEADER);
	}
	prev = NULL;
	block = *zone;
	while (block)
	{
		if (block->free && size <= block->size)
		{
			create_free_block(block, size);
			return ((void *)block + HEADER);
		}
		prev = block;
		block = block->next;
	}
	if (!(new = new_zone(prev, size, zone_size)))
		return (NULL);
	return ((void *)new + HEADER);
}

void			*malloc_unthread(size_t size)
{
	size_t	aligned_size;
	t_alloc *block;
	t_alloc *ret;

	aligned_size = get_multiple_of(size, 16);
	if (aligned_size <= TINY)
		return (allocate(&g_state.tiny, aligned_size, TINY_ZONE));
	else if (aligned_size <= SMALL)
		return (allocate(&g_state.small, aligned_size, SMALL_ZONE));
	else
	{
		block = g_state.large;
		while (block && block->next)
			block = block->next;
		ret = new_zone(block, aligned_size, aligned_size + HEADER);
		if (!block)
			g_state.large = ret;
		return ((void *)ret + HEADER);
	}
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
