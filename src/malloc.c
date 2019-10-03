/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/02 16:42:46 by kyazdani          #+#    #+#             */
/*   Updated: 2019/10/03 19:37:00 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

inline static size_t      get_alloc_length(size_t len)
{
    size_t page_size;
    size_t tiny;
    size_t small;
    size_t large;

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
    // must return a multiple of page_size and add 1 page size to it to be sure the malloc is big enough
    return (page_size * (len / page_size) + page_size);
}

inline static void *use_mmap(size_t len)
{
    return (mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0));
}

static void *init(size_t len)
{
    size_t  size;

    size = get_alloc_length(len);
    g_alloc = use_mmap(size);

    g_alloc->next = NULL;
    g_alloc->prev = NULL;
    g_alloc->free = 1;
    g_alloc->size = size;

    return g_alloc + sizeof(g_alloc) + 1;
}

void	    *ft_malloc(size_t size)
{
    if (!size)
        return (NULL);
    if (!g_alloc)
        return(init(size));
    return (NULL);
}
