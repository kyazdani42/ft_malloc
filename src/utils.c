/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 15:21:46 by kyazdani          #+#    #+#             */
/*   Updated: 2019/10/18 15:21:49 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

inline static int   get_addr_length(long long addr, int base)
{
    int     i;

    i = 0;
    while (addr)
    {
        i++;
        addr /= base;
    }
    return (i);
}

void                putaddr(void *ptr)
{
    int         i;
    int         len;
    long long   addr;
    char        buf[100];
    const char  base[16] = "0123456789abcdef";

    addr = (long long)ptr;
    if (!addr)
    {
        write(1, "0x000000000", 12);
        return ;
    }
    len = get_addr_length(addr, 16) + 2;
    i = len;
    buf[0] = '0';
    buf[1] = 'x';
    while (--i > 1)
    {
        buf[i] = base[addr % 16];
        addr /= 16;
    }
    write(1, buf, len);
}

void                putnbr(size_t nb)
{
    int         len;
    int         i;
    char        buf[100];
    const char  base[10] = "0123456789";

    if (!nb)
    {
        write(1, "0", 1);
        return ;
    }
    len = get_addr_length(nb, 10);
    i = len;
    while (--i >= 0)
    {
        buf[i] = base[nb % 10];
        nb /= 10;
    }
    write(1, buf, len);
}

void                putstr(char *str)
{
    int     len;

    len = 0;
    while (str[len])
        len++;
    write(1, str, len);
}

size_t              get_multiple_of(size_t len, size_t alignment)
{
    if (len % alignment == 0)
        return (len != 0 ? len : alignment);
    return (alignment * (len / alignment + 1));
}
