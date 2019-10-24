/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/24 15:13:43 by kyazdani          #+#    #+#             */
/*   Updated: 2019/10/24 15:13:44 by kyazdani         ###   ########.fr       */
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

static void				remove_zone(t_alloc **zone, t_alloc *to_remove)
{
	t_alloc	*prev;
	t_alloc	*next;

	prev = to_remove->prev;
	next = to_remove->next;
	if (prev)
	{
		prev->next = next;
		if (next != NULL)
			next->prev = prev;
	}
    if (*zone == to_remove)
    {
        *zone = (*zone)->next;
        if (*zone != NULL)
            (*zone)->prev = NULL;
    }
	munmap((void *)to_remove, to_remove->size + HEADER);
}

static void				defrag(t_alloc *zone)
{
	t_alloc	*tmp;

	tmp = zone;
	while (tmp)
	{
		while (tmp->free && tmp->next && tmp->next->free && tmp->zone == tmp->next->zone)
		{
			if (tmp->next->next)
				tmp->next->next->prev = tmp;
			tmp->size += HEADER + tmp->next->size;
			tmp->next = tmp->next->next;
		}
		tmp = tmp->next;
	}
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
	defrag(*zone);
	if (should_munmap(cur))
		remove_zone(zone, cur);
}

void					free(void *ptr)
{
	initialize_mutex();
	free_unthread(ptr);
	pthread_mutex_unlock(&g_mutex);
}
