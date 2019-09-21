/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/02 16:42:46 by kyazdani          #+#    #+#             */
/*   Updated: 2019/04/22 18:18:55 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

size_t	get_alloc_length(size_t len)
{
    int page_size;
    int tiny;
    int small;
    int large;

    /* page_size = getpagesize(); */
    page_size = sysconf(_SC_PAGESIZE);
    tiny = TINY(page_size);
    small = SMALL(page_size);
    large = LARGE(page_size);
    if (len < tiny)
        return (tiny);
    if (len < small)
        return (small);
    if (len < large)
        return (large);
    return (len);
}
