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
    int             free;
    int             zone;
    size_t          size;
    struct s_alloc  *next;
}                   t_alloc;

# define HEADER get_multiple_of(sizeof(t_alloc), 16)

# define TINY 512
# define SMALL 4096
# define TINY_ZONE (TINY) * 100
# define SMALL_ZONE (SMALL) * 100

typedef struct      s_state
{
    t_alloc         *tiny;
    t_alloc         *small;
    t_alloc         *large;
}                   t_state;

t_state             g_state;
pthread_mutex_t     g_mutex;

void                free(void *ptr);
void                free_unthread(void *ptr);

void                *malloc(size_t size);
void                *malloc_unthread(size_t size);

void                *calloc(size_t count, size_t size);

void                *realloc(void *ptr, size_t size);
void                *reallocf(void *ptr, size_t size);
t_alloc             *get_header_from_addr(void *ptr);
void                copy_memory(void *to, void *from, size_t n);
int                 should_resize(t_alloc *cur, t_alloc *next, size_t size);
void                resize_alloc(t_alloc **header, t_alloc **next, size_t size);
int                 check_new_size(t_alloc *cur, size_t size);

void                putstr(char *str);
void                putnbr(size_t nb);
void                putaddr(void *ptr);
size_t              get_multiple_of(size_t len, size_t alignment);

void                show_alloc_mem();

#endif
