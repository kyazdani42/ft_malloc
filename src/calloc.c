#include "ft_malloc.h"

void    *calloc(size_t count, size_t size)
{
    size_t   value;
    char     *str;
    void     *ret;
    size_t   aligned_value;

    value = count * size;
    if (!(ret = malloc(value)))
        return (NULL);

    pthread_mutex_lock(&g_mutex);
    str = ret;
    aligned_value = get_multiple_of(value, 16);
    while (aligned_value)
    {
        *str = 0;
        str++;
        aligned_value--;
    }

    pthread_mutex_unlock(&g_mutex);
    return ret;
}

