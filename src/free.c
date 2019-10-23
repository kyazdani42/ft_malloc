/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/02 15:26:52 by kyazdani          #+#    #+#             */
/*   Updated: 2019/10/05 14:44:48 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

inline static int		should_munmap(t_alloc *cur)
{
	t_alloc		*next;
	t_alloc		*prev;

	next = cur->next;
	prev = cur->prev;
	return ((!prev && !next) || (!prev && next && next->zone != cur->zone) ||
			(!next && prev && prev->zone != cur->zone) ||
			(next && prev && prev->zone != cur->zone && next->zone != cur->zone));
}

static void				defrag(t_alloc **e)
{
	t_alloc		*next;
	t_alloc		*prev;

	prev = (*e)->prev;
	next = (*e)->next;
	while (prev && prev->free && prev->zone == (*e)->zone)
	{
		prev->size += HEADER + (*e)->size;
		prev->next = next;
		if (next)
			next->prev = prev;
		*e = prev;
		prev = prev->prev;
	}
	while (next && next->free && next->zone == (*e)->zone)
	{
		(*e)->size += HEADER + next->size;
		next = next->next;
		(*e)->next = next;
		if (next)
			next->prev = *e;
	}
}

static void				remove_zone(t_alloc **zone, t_alloc *cur)
{
	t_alloc *prev;
	t_alloc *next;

	prev = cur->prev;
	next = cur->next;
	if (prev)
	{
		prev->next = next;
		if (next)
			next->prev = prev;
	}
    else if (*zone == cur)
    {
        *zone = (*zone)->next;
        if (*zone)
            (*zone)->prev = NULL;
    }
	munmap((void *)cur, cur->size + HEADER);
}

void					free_unthread(void *ptr)
{
	t_alloc		*cur;
	t_alloc		**zone;

	if (!ptr)
		return ;
	if (!(zone = get_zone_set_cur(ptr, &cur)))
		return ;
	cur->free = 1;
	defrag(&cur);
	if (should_munmap(cur))
		remove_zone(zone, cur);
}

void					free(void *ptr)
{
	initialize_mutex();
	free_unthread(ptr);
	pthread_mutex_unlock(&g_mutex);
}
