/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyazdani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 14:15:20 by kyazdani          #+#    #+#             */
/*   Updated: 2019/10/19 14:15:21 by kyazdani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

void	*calloc(size_t count, size_t size)
{
	size_t	value;
	char	*str;
	void	*ret;

	pthread_mutex_lock(&g_mutex);
	value = get_multiple_of(count * size, 16);
	if (!(ret = malloc_unthread(value)))
	{
		pthread_mutex_unlock(&g_mutex);
		return (NULL);
	}
	str = ret;
	while (value)
	{
		*str = 0;
		str++;
		value--;
	}
	pthread_mutex_unlock(&g_mutex);
	return (ret);
}
