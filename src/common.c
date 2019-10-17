#include "ft_malloc.h"

size_t    get_multiple_of(size_t len, size_t alignment) {
    if (len % alignment == 0)
        return len;
    return alignment * (len / alignment + 1);
}

t_alloc  *get_header_from_addr(void *ptr)
{
    t_alloc *tmp;

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
	char	    *src;

    dest = to;
    src = from;
	while (n--)
    {
		*dest = *src;
        dest++;
        src++;
    }
}
