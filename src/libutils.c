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

t_alloc     **get_zone_set_cur(void *ptr, t_alloc **cur)
{
    *cur = g_state.large;
    while (*cur)
    {
        if ((char *)*cur + HEADER == ptr)
            return (&g_state.large);
        *cur = (*cur)->next;
    }
    *cur = g_state.small;
    while (*cur)
    {
        if ((char *)*cur + HEADER == ptr)
            return (&g_state.small);
        *cur = (*cur)->next;
    }
    *cur = g_state.tiny;
    while (*cur)
    {
        if ((char *)*cur + HEADER == ptr)
            return (&g_state.tiny);
        *cur = (*cur)->next;
    }
    return (NULL);
}

void        copy_memory(void *to, void *from, size_t n)
{
    char    *dest;
    char    *src;

    dest = to;
    src = from;
    while (n--)
    {
        *dest = *src;
        dest++;
        src++;
    }
}

void        initialize_mutex(void)
{
    static int  initialization = 1;

    if (initialization)
    {
        pthread_mutex_init(&g_mutex, NULL);
        initialization = 0;
    }
    pthread_mutex_lock(&g_mutex);
}
