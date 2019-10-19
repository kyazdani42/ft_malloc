/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libutils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 14:18:10 by kyazdani          #+#    #+#             */
/*   Updated: 2019/10/18 14:18:13 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

int		should_resize(t_alloc *cur, size_t size)
{
	t_alloc		*next;

	next = cur->next;
	return (next
		&& next->free
		&& next->zone == cur->zone
		&& cur->size + HEADER + next->size >= size);
}

void	resize_alloc(t_alloc **cur, size_t size)
{
	t_alloc		*next;
    t_alloc     *new;
	size_t		next_size;

	next = (*cur)->next;
	next_size = next->size - (size - (*cur)->size);
	if (next_size < (HEADER * 2 + 16))
	{
		(*cur)->size += HEADER + next->size;
        next = next->next;
		(*cur)->next = next;
        if (next)
            next->prev = *cur;
	}
	else
	{
		new = (void *)*cur + HEADER + size;
        next = next->next;
        new->free = 1;
        new->size = next_size;
        new->zone = (*cur)->zone;
        new->next = next;
        new->prev = *cur;
        if (next)
            next->prev = new;
		(*cur)->next = new;
		(*cur)->size = size;
	}
}

t_alloc	**get_zone_set_cur(void *ptr, t_alloc **cur)
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

void	copy_memory(void *to, void *from, size_t n)
{
	char		*dest;
	char		*src;

	dest = to;
	src = from;
	while (n--)
	{
		*dest = *src;
		dest++;
		src++;
	}
}
