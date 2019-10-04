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
    printf("ptr->free: %d\n", ptr->free);
    printf("ptr->size %zu\n", ptr->size);
    printf("ptr->prev %p\n", ptr->prev);
    printf("ptr->next %p\n", ptr->next);
}

void    show_alloc_mem(void)
{
    t_alloc *ptr;

    printf("pagesize: %d\n", getpagesize());
    if (!g_alloc)
        return;
    ptr = g_alloc;
    while (ptr)
    {
        print_t_alloc(ptr);
        ptr = ptr->next;
    }
}
