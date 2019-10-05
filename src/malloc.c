/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/02 16:42:46 by kyazdani          #+#    #+#             */
/*   Updated: 2019/10/05 14:53:29 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

inline static void      *use_mmap(size_t len)
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

inline static size_t    get_size(size_t len)
{
    size_t  pagesize;

    pagesize = getpagesize();
    if (len % pagesize == 0)
        return len;
    return pagesize * (len / pagesize + 1);
}

void    *allocate(size_t size, t_alloc **ptr)
{
    size_t  mmap_size;

    mmap_size = get_size((size + sizeof(t_alloc)) * (size < SMALL ? 100 : 1));

    *ptr = use_mmap(mmap_size);
    (*ptr)->size = size;
    (*ptr)->free = 0;

    if (size < SMALL) {
        (*ptr)->next = (void *)*ptr + sizeof(t_alloc) + size; // + alignment ?
        (*ptr)->next->size = mmap_size - (sizeof(t_alloc) + size);
        (*ptr)->next->next = NULL;
        (*ptr)->next->free = 1;
    } else {
        (*ptr)->next = NULL;
    }

    return (void *)*ptr + sizeof(t_alloc);
}

void                    *malloc(size_t size)
{
    static int  initialization = 1;

    if (!size)
        return (NULL);

    if (initialization)
    {
        g_state.tiny = NULL;
        g_state.small = NULL;
        g_state.large = NULL;
        initialization = 0;
    }

    if (size < TINY)
        return (allocate(size, &(g_state).tiny));
    else if (size < SMALL)
        return (allocate(size, &(g_state).small));
    else
        return allocate(size, &(g_state).large);
}

