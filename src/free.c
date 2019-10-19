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

inline static void		defrag(t_alloc **e)
{
	t_alloc		*next;
	t_alloc		*prev;

	prev = (*e)->prev;
	next = (*e)->next;
	while (prev && prev->free && prev->zone == (*e)->zone)
	{
		prev->size += HEADER + (*e)->size;
		*e = prev;
		(*e)->next = next;
		if (next)
			next->prev = *e;
		prev = prev->prev;
	}
	while (next && next->free && next->zone == (*e)->zone)
	{
		(*e)->size += HEADER + next->size;
		next = next->next;
		if (next)
			next->prev = *e;
		(*e)->next = next;
	}
}

inline static t_alloc	**set_zone_and_elements(void *ptr, t_alloc **cur)
{
	*cur = g_state.large;
	while (*cur)
	{
		if ((void *)*cur + HEADER == ptr)
			return (&g_state.large);
		*cur = (*cur)->next;
	}
	*cur = g_state.small;
	while (*cur)
	{
		if ((void *)*cur + HEADER == ptr)
			return (&g_state.small);
		*cur = (*cur)->next;
	}
	*cur = g_state.tiny;
	while (*cur)
	{
		if ((void *)*cur + HEADER == ptr)
			return (&g_state.tiny);
		*cur = (*cur)->next;
	}
	return (NULL);
}

void					free_unthread(void *ptr)
{
	t_alloc		*cur;
	t_alloc		**zone;

	if (!ptr)
		return ;
	if (!(zone = set_zone_and_elements(ptr, &cur)))
		return ;
	cur->free = 1;
	defrag(&cur);
	if (should_munmap(cur))
	{
		if (!cur->prev && !cur->next)
			*zone = NULL;
		else if (!cur->prev && cur->next)
		{
			*zone = cur->next;
			(*zone)->prev = NULL;
		}
		else
		{
			*zone = cur->prev;
			(*zone)->next = cur->next;
		}
		if (munmap(cur, cur->size + HEADER) == -1)
			return ;
	}
}

void					free(void *ptr)
{
	pthread_mutex_lock(&g_mutex);
	free_unthread(ptr);
	pthread_mutex_unlock(&g_mutex);
}
