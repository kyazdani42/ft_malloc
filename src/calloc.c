#include "ft_malloc.h"

void    *calloc(size_t count, size_t size)
{
    size_t   value;
    char     *str;
    void     *ret;

    if (!count || !size)
        return (NULL);
    value = count * size;
    if (value > INT_MAX)
        return (NULL);
    if (!(ret = malloc(value)))
        return (NULL);
    str = ret;
    while (value)
    {
        *str = 0;
        str++;
        value--;
    }
    return ret;
}


