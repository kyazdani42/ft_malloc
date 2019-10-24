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
