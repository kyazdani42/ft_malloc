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
# include <limits.h>
# include <pthread.h>
# include <unistd.h>

# define PROT (PROT_READ | PROT_WRITE)
# define FLAGS (MAP_ANON | MAP_PRIVATE)

// MACOS
#ifdef __MACH__
# define PS getpagesize()
// LINUX
#else
# define PS sysconf(_SC_PAGESIZE)
#endif

typedef struct      s_alloc
{
    short            free;
    short            zone;
    size_t          size;
    struct s_alloc  *next;
    struct s_alloc  *prev;
}                   t_alloc;

# define HEADER sizeof(t_alloc)

# define TINY 512
# define SMALL 4096
# define TINY_ZONE (TINY + HEADER) * 100
# define SMALL_ZONE (SMALL + HEADER) * 100

typedef struct      s_state
{
    t_alloc         *tiny;
    t_alloc         *small;
    t_alloc         *large;
}                   t_state;

t_state             g_state;
pthread_mutex_t     g_mutex;

void                free(void *ptr);
void                *malloc(size_t size);
void                *realloc(void *ptr, size_t size);
void                *calloc(size_t count, size_t size);
void                *reallocf(void *ptr, size_t size);

void                putstr(char *str);
void                putnbr(size_t nb);
void                putaddr(void *ptr);

t_alloc             *get_header_from_addr(void *ptr);
size_t              get_multiple_of(size_t len, size_t alignment);

void                show_alloc_mem();

#endif
