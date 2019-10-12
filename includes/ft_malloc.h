/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/02 15:26:52 by kyazdani          #+#    #+#             */
/*   Updated: 2019/10/05 14:44:48 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include <sys/types.h>
# include <sys/mman.h>
# include <sys/resource.h>
# include <sys/time.h>
# include <unistd.h>

# define TINY 512
# define SMALL 4096
# define PROT (PROT_READ | PROT_WRITE)
# define FLAGS (MAP_ANON | MAP_PRIVATE)

typedef struct      s_alloc
{
    int             free;
    size_t          size;
    struct s_alloc  *next;
    struct s_alloc  *prev;
}                   t_alloc;

# define HEADER sizeof(t_alloc)

typedef struct      s_state
{
    t_alloc         *tiny;
    t_alloc         *small;
    t_alloc         *large;
}                   t_state;

t_state             g_state;

void                free(void *ptr);
void                *malloc(size_t size);
void                *realloc(void *ptr, size_t size);
void                putstr(char *str);
void                putnbr(size_t nb);
void                putaddr(void *ptr);
void                show_alloc_mem();
void                show_alloc_mem_ex();

#endif
