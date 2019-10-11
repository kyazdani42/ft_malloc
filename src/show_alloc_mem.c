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
#include <stdio.h>

void    print_t_alloc(t_alloc *ptr)
{
    if (!ptr)
        printf("Empty zone\n");
    while (ptr)
    {
        printf("prev \x1b[33m%16p\x1b[0m | ", ptr->prev);
        printf("addr \x1b[32m%16p\x1b[0m | ", ptr);
        printf("next \x1b[34m%16p\x1b[0m | ", ptr->next);
        printf("free %d | ", ptr->free);
        printf("size %zu\n\n", ptr->size);
        ptr = ptr->next;
    }
}

void    show_alloc_mem(void)
{
    printf("\n\x1b[1mTINY:\x1b[0m\n\n");
    print_t_alloc(g_state.tiny);
    /* printf("SMALL:\n"); */
    /* print_t_alloc(g_state.small); */
    /* printf("LARGE:\n"); */
    /* print_t_alloc(g_state.large); */
}

