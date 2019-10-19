/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reallocf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 14:21:58 by kyazdani          #+#    #+#             */
/*   Updated: 2019/10/18 14:21:59 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

/* inline static void	*reallocf_unthread(void *ptr, size_t size) */
/* { */
/* 	t_alloc		*header; */
/* 	void		*new_ptr; */
/*  */
/* 	if (!(header = get_header_from_addr(ptr))) */
/* 		return (NULL); */
/* 	size = get_multiple_of(size, 16); */
/* 	if (size <= header->size) */
/* 		return (ptr); */
/* 	if (should_resize(header, size) && check_new_size(header, size)) */
/* 	{ */
/* 		resize_alloc(&header, size); */
/* 		return (ptr); */
/* 	} */
/* 	if (!(new_ptr = malloc_unthread(size))) */
/* 	{ */
/* 		free_unthread(ptr); */
/* 		return (NULL); */
/* 	} */
/* 	copy_memory(new_ptr, ptr, header->size); */
/* 	free_unthread(ptr); */
/* 	return (new_ptr); */
/* } */
/*  */
void				*reallocf(void *ptr, size_t size)
{
	free(ptr);
	return (malloc(size));

	/* void	*ret; */
    /*  */
	/* if (!ptr) */
	/* 	return (malloc(size)); */
	/* if (!size) */
	/* { */
	/* 	free(ptr); */
	/* 	return (malloc(0)); */
	/* } */
	/* pthread_mutex_lock(&g_mutex); */
	/* ret = reallocf_unthread(ptr, size); */
	/* pthread_mutex_unlock(&g_mutex); */
	/* return (ret); */
}
