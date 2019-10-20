/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 15:34:28 by kyazdani          #+#    #+#             */
/*   Updated: 2019/10/20 15:34:32 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

/* static int		should_resize(t_alloc *cur, size_t size) */
/* { */
/* 	t_alloc		*next; */
/*  */
/* 	next = cur->next; */
/* 	return (next */
/* 		&& next->free */
/* 		&& next->zone == cur->zone */
/* 		&& cur->size + HEADER + next->size >= size); */
/* } */
/*  */
/* static void	resize_alloc(t_alloc *cur, size_t size) */
/* { */
/* 	t_alloc		*next; */
/*     t_alloc     *new; */
/* 	size_t		new_size; */
/*  */
/* 	next = cur->next; */
/* 	new_size = next->size - (size - cur->size); */
/* 	if (new_size < 16) */
/* 	{ */
/* 		cur->size += HEADER + next->size; */
/* 		next = next->next; */
/* 		cur->next = next; */
/* 		if (next) */
/* 			next->prev = cur; */
/* 	} */
/* 	else */
/* 	{ */
/* 		next = next->next; */
/* 		cur->size = size; */
/* 		new = (void *)cur + HEADER + size; */
/* 		cur->next = new; */
/* 		new->prev = cur; */
/* 		new->next = next; */
/* 		new->size = new_size; */
/* 		new->zone = cur->zone; */
/* 		new->free = 1; */
/* 		if (next) */
/* 			next->prev = new; */
/* 	} */
/* } */

static void			*reall_unthread(void *ptr, size_t size, int should_free)
{
    t_alloc     *cur;
    void        *new_ptr;
	t_alloc		**zone;

	cur = NULL;
    if (!(zone = get_zone_set_cur(ptr, &cur)))
        return (NULL);

    size = get_multiple_of(size, 16);

    if (size <= cur->size)
        return (ptr);

    /* if (should_resize(cur, size)) */
    /* { */
    /*     resize_alloc(cur, size); */
    /*     return (ptr); */
    /* } */

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
