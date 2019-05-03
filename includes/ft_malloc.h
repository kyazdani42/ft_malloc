/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/02 15:26:52 by kyazdani          #+#    #+#             */
/*   Updated: 2019/04/22 18:16:21 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include <sys/types.h>
# include <sys/mman.h>
# include <sys/resource.h>
# include <sys/time.h>
# include <unistd.h>

# define TINY(x) x * 50
# define SMALL(x) x * 100
# define LARGE(x) x * 200

enum	e_size {
	TINY,
	SMALL,
	LARGE
};

typedef struct		s_alloc
{
	void			*addr;
	size_t			len;
	struct s_alloc	*next;
}					t_alloc;

typedef struct		s_blob
{
	size_t			size;
	t_alloc			*allocations;
	struct s_blob	*next;
}					t_blob;

void				free(void *ptr);
void				*malloc(size_t size);
void				*realloc(void *ptr, size_t size);
void				show_alloc_mem();

void				*alloc_handler(void *addr, size_t len);
t_blob				*get_allocations(void *blob);
size_t				get_alloc_length(size_t len);

#endif
