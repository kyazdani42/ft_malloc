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

#include "../includes/malloc.h"

static size_t	get_alloc_length(size_t *len)
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

void			*alloc_handler(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
	size_t		total_allocation_length;
	void		*mmap_ret;

	total_allocation_length = get_alloc_length(&len);
	if (g_blob_list == NULL)
		mmap_ret = mmap(addr, total_allocation_length, prot, flags, fd, offset);
	else
		return (NULL);
	return (mmap_ret);
}
