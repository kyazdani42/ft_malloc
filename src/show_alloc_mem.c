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
        printf("ptr: %p\n", ptr);
        printf("ptr->free: %d\n", ptr->free);
        printf("ptr->size: %zu\n", ptr->size);
        printf("ptr->next: %p\n", ptr->next);
        ptr = ptr->next;
    }
}

void    show_alloc_mem(void)
{
    printf("pagesize: %d\n", getpagesize());

    printf("TINY:\n");
    print_t_alloc(g_state.tiny);
    printf("SMALL:\n");
    print_t_alloc(g_state.small);
    printf("LARGE:\n");
    print_t_alloc(g_state.large);
}

