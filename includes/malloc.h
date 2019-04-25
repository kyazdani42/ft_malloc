/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/02 15:26:52 by kyazdani          #+#    #+#             */
/*   Updated: 2019/04/22 18:16:21 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include <sys/types.h>
# include <sys/mman.h>
# include <sys/resource.h>
# include <sys/time.h>
# include <unistd.h>

# define TINY 100
# define SMALL 600
# define BIG 1000

typedef struct		s_alloc
{
	void			*addr;
	size_t			len;
	struct s_alloc	*next;
}					t_alloc;

typedef struct		s_blob
{
	size_t			total_space;
	size_t			current_taken_space;
	t_alloc			*allocations;
	struct s_blob	*next;
}					t_blob;

t_blob				*g_blob_list = NULL;

void				free(void *ptr);
void				*malloc(size_t size);
void				*realloc(void *ptr, size_t size);
void				*alloc_handler();

#endif
