/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/02 15:26:52 by kyazdani          #+#    #+#             */
/*   Updated: 2019/10/03 19:38:00 by kyazdani         ###   ########.fr       */
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

typedef struct		s_alloc
{
    size_t			size;
    int             free;
    struct s_alloc  *prev;
    struct s_alloc	*next;
}					t_alloc;

t_alloc             *g_alloc;

void				free(void *ptr);
void				*malloc(size_t size);
void				*realloc(void *ptr, size_t size);
void				show_alloc_mem();

#endif
