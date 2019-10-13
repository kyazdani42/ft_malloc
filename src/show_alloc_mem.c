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

static void    print_t_alloc(t_alloc *ptr)
{
    if (!ptr)
        putstr("Empty zone\n");
    while (ptr)
    {
        if (ptr->free)
            putstr("\e[32m");
        else
            putstr("\e[35m");
        putaddr((void *)ptr + HEADER);
        putstr(" - ");
        putaddr((void *)ptr + HEADER + ptr->size);
        putstr(" : ");
        putnbr(ptr->size);
        putstr(" octets\n");
        putstr("\e[0m");
        ptr = ptr->next;
    }
}

void    show_alloc_mem(void)
{
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
}

