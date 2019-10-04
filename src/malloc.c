/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/02 16:42:46 by kyazdani          #+#    #+#             */
/*   Updated: 2019/10/04 19:56:31 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

inline static size_t get_alloc_size(size_t len)
{
    if (len + sizeof(t_alloc) < TINY)
        return (TINY * getpagesize());
    if (len + sizeof(t_alloc) < SMALL)
        return (SMALL * getpagesize());
    return ((len + sizeof(t_alloc)) * getpagesize());
}

inline static void *use_mmap(size_t len)
{
    return (mmap(
                NULL,
                len,
                PROT_READ | PROT_WRITE,
                MAP_ANON | MAP_PRIVATE,
                -1,
                0
                ));
}

static void *init(size_t size)
{
    g_alloc = use_mmap(size);

    g_alloc->next = NULL;
    g_alloc->prev = NULL;
    g_alloc->free = 0;
    g_alloc->size = size;

    return g_alloc + sizeof(t_alloc) + 1;
}

void *malloc(size_t size)
{
    size_t  zone_size;

    if (size == 0)
        return (NULL);

    zone_size = get_alloc_size(size);
    if (!g_alloc)
        return(init(zone_size));

    return (NULL);
}
