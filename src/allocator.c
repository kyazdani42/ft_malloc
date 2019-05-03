/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/02 16:42:46 by kyazdani          #+#    #+#             */
/*   Updated: 2019/04/22 18:18:55 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

size_t	get_alloc_length(size_t len)
{
	int page_size;
	int tiny;
	int small;
	int large;

	page_size = getpagesize();
	tiny = TINY(page_size);
	small = SMALL(page_size);
	large = LARGE(page_size);
	if (len < tiny)
		return (tiny);
	if (len < small)
		return (small);
	if (len < large)
		return (large);
	return (len);
}

static void		*allocate(void *addr, size_t len)
{
	void		*mmap_ret;

	mmap_ret = mmap(addr, len, PROT_READ | PROT_WRITE,
					MAP_ANON | MAP_PRIVATE, -1, 0);
	return (mmap_ret);
}

void			*alloc_handler(void *addr, size_t len)
{
	size_t		total_allocation_length;
	void		*mmap_ret;
	t_blob		*allocations;

	total_allocation_length = get_alloc_length(len);
	allocations = get_allocations((void*)NULL);
	if (allocations == NULL)
	{
		mmap_ret = allocate(addr, len);
		get_allocations(mmap_ret);
		return (mmap_ret);
	}
	else
	{
		return ((void*)NULL);
	}
}
