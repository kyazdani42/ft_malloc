#include "ft_malloc.h"

void    *calloc(size_t count, size_t size)
{
    size_t   value;
    char     *str;
    void     *ret;
    size_t   i;

    value = count * size;
    if (!(ret = malloc(value)))
        return (NULL);
    str = ret;
    i = -1;
    while (++i < value)
        str[i] = 0;
    return ret;
}

