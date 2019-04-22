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

#include "malloc.h"

/*
	steps:
		- determine size type of allocation (TINY | SMALL | BIG | ..)
		- determine if blob exists
		- if blob exists && size of allocation
		 is less than BIG, find a blob that has a space that fits (total_space - current_taken_space)
		- if no blob fits, allocate the memory and push the blob to the end of the blob list
		return the addr
		if something went wrong, return null
*/
void							*alloc_handler(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
	size_t				total_allocation_length;
	void*					mmap_ret;

	total_allocation_length = get_alloc_length(&len);
	if (blob_list == NULL)
	{
		mmap_ret = mmap(addr, total_allocation_length, prot, flags, fd, offset);
		if (mmap_ret == NULL)
			return (NULL);
		
	} else {
		return (NULL);
	}
	return (mmap_ret);
}

static t_blob		*create_blob(void *addr, size_t len)
{
	(void)addr;
	(void)len;
	return (NULL);
}

static size_t		get_alloc_length(size_t *len)
{
	if (*len < TINY)
		return (TINY);
	else if (*len < SMALL)
		return (SMALL);
	else if (*len < BIG)
		return (BIG);
	else
		return (*len);
}

static void			*make_alloc()
{
	return (NULL);
}
