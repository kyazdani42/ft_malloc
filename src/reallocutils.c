/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reallocutils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 14:18:10 by kyazdani          #+#    #+#             */
/*   Updated: 2019/10/18 14:18:13 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

int		check_new_size(t_alloc *cur, size_t size)
{
	if (cur->size <= TINY && size > TINY)
		return (0);
	if (cur->size <= SMALL && (size > SMALL || size <= TINY))
		return (0);
	return (1);
}

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
	size_t		next_size;

	next = (*cur)->next;
	next_size = next->size - (size - (*cur)->size);
	if (next_size < (HEADER * 2 + 16))
	{
		(*cur)->size += HEADER + next->size;
		(*cur)->next = next->next;
	}
	else
	{
		(*cur)->size = size;
		(*cur)->next = (void *)*cur + HEADER + size;
		(*cur)->next->next = next->next;
		next = (*cur)->next;
		next->prev = *cur;
		next->free = 1;
		next->zone = (*cur)->zone;
		next->size = next_size;
	}
}

t_alloc	*get_header_from_addr(void *ptr)
{
	t_alloc	*tmp;

	tmp = g_state.large;
	while (tmp)
	{
		if ((void *)tmp + HEADER == ptr)
			return (tmp);
		tmp = tmp->next;
	}
	tmp = g_state.small;
	while (tmp)
	{
		if ((void *)tmp + HEADER == ptr)
			return (tmp);
		tmp = tmp->next;
	}
	tmp = g_state.tiny;
	while (tmp)
	{
		if ((void *)tmp + HEADER == ptr)
			return (tmp);
		tmp = tmp->next;
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
