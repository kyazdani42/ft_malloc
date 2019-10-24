/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/04 19:56:07 by kyazdani          #+#    #+#             */
/*   Updated: 2019/10/04 20:03:05 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

inline static void	print(t_alloc *ptr, t_alloc *prev)
{
	if (prev && prev->zone != ptr->zone)
		putstr("\x1b[31m\t\t\t      |\n\t\t\t      |\n\t\t\t      v\x1b[0m\n");
	putstr("\e[34m");
	putaddr(ptr->prev);
	putstr(" <---- ");
	if (ptr->free)
		putstr("\e[32m");
	else
		putstr("\e[35m");
	putaddr((void *)ptr + HEADER);
	putstr(" - ");
	putaddr((void *)ptr + HEADER + ptr->size);
	putstr(" : ");
	putnbr(ptr->size);
	putstr(" octets\e[34m ----> ");
	putaddr(ptr->next);
	putstr("\e[0m\n");
	prev = ptr;
	ptr = ptr->next;
}

static void			print_t_alloc(t_alloc *ptr)
{
	t_alloc		*prev;

	prev = NULL;
	if (!ptr)
		putstr("Empty zone\n");
	while (ptr)
		print(ptr, prev);
}

void				show_alloc_mem(void)
{
	pthread_mutex_lock(&g_mutex);
	putstr("------------------------------------------------------- \n");
	putstr("\x1b[1mTINY :\x1b[0m ");
	putaddr(g_state.tiny);
	putstr("\n");
	print_t_alloc(g_state.tiny);
	putstr("\n\x1b[1mSMALL :\x1b[0m ");
	putaddr(g_state.small);
	putstr("\n");
	print_t_alloc(g_state.small);
	putstr("\n\x1b[1mLARGE :\x1b[0m ");
	putaddr(g_state.large);
	putstr("\n");
	print_t_alloc(g_state.large);
	putstr("------------------------------------------------------- \n");
	pthread_mutex_unlock(&g_mutex);
}
