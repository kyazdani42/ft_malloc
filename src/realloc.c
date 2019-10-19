/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 15:44:45 by kyazdani          #+#    #+#             */
/*   Updated: 2019/10/18 15:44:47 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

inline static void	*reall_unthread(void *ptr, size_t size, int should_free)
{
    t_alloc     *cur;
    void        *new_ptr;

    if (get_zone_set_cur(ptr, &cur) == NULL)
        return (NULL);

    size = get_multiple_of(size, 16);

    if (size <= cur->size)
        return (ptr);

    if (should_resize(cur, size))
    {
        resize_alloc(&cur, size);
        return (ptr);
    }

    if (!(new_ptr = malloc_unthread(size)))
    {
        if (should_free)
            free_unthread(ptr);
        return (NULL);
    }

    copy_memory(new_ptr, ptr, cur->size);
    free_unthread(ptr);
    return (new_ptr);
}

void				*reallocf(void *ptr, size_t size)
{
    void	*ret;

    if (!ptr)
        return (malloc(size));

    if (!size)
    {
        free(ptr);
        return (malloc(0));
    }

    pthread_mutex_lock(&g_mutex);
    ret = reall_unthread(ptr, size, 1);
    pthread_mutex_unlock(&g_mutex);

    return (ret);
}

void				*realloc(void *ptr, size_t size)
{
    void	*ret;

    if (!ptr)
        return (malloc(size));

    if (!size)
    {
        free(ptr);
        return (malloc(0));
    }

    pthread_mutex_lock(&g_mutex);
    ret = reall_unthread(ptr, size, 0);
    pthread_mutex_unlock(&g_mutex);

    return (ret);
}
