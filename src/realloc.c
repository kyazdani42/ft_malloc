/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 15:34:28 by kyazdani          #+#    #+#             */
/*   Updated: 2019/10/20 15:34:32 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

inline static void	cut_block(t_alloc *block, size_t new_size)
{
    t_alloc *new_next;
    t_alloc *nextnext;
    size_t  new_next_size;

    new_next_size = block->next->size - (new_size - block->size);
	if (new_next_size < 16)
	{
		block->size += HEADER + block->next->size;
		block->next = block->next->next;
		if (block->next)
			block->next->prev = block;
	}
	else
	{
        nextnext = block->next->next;
        new_next = (void *)block + HEADER + new_size;
        new_next->free = 1;
        new_next->size = new_next_size;
        new_next->next = nextnext;
        new_next->prev = block;
        if (nextnext)
            nextnext->prev = new_next;
		block->size = new_size;
        block->next = new_next;
	}
}

static void			*reall_unthread(void *ptr, size_t size, int should_free)
{
	t_alloc		*block;
	void		*new_ptr;
	t_alloc		**zone;

	block = NULL;
	if (!(zone = get_zone_set_cur(ptr, &block)))
		return (NULL);
	size = get_multiple_of(size, 16);
	if (size <= block->size)
		return (ptr);
	else if (block->next && block->next->free
			&& size <= block->next->size + block->size + HEADER)
	{
		cut_block(block, size);
		return (ptr);
	}
	if (!(new_ptr = malloc_unthread(size)))
	{
		if (should_free)
			free_unthread(ptr);
		return (NULL);
	}
	copy_memory(new_ptr, ptr, block->size);
	free_unthread(ptr);
	return (new_ptr);
}

void				*reallocf(void *ptr, size_t size)
{
	void	*ret;

	if (!ptr)
		return (malloc(size));
	if (!size)
	{
		free(ptr);
		return (malloc(0));
	}
	initialize_mutex();
	ret = reall_unthread(ptr, size, 1);
	pthread_mutex_unlock(&g_mutex);
	return (ret);
}

void				*realloc(void *ptr, size_t size)
{
	void	*ret;

	if (!ptr)
		return (malloc(size));
	if (!size)
	{
		free(ptr);
		return (malloc(0));
	}
	initialize_mutex();
	ret = reall_unthread(ptr, size, 0);
	pthread_mutex_unlock(&g_mutex);
	return (ret);
}
