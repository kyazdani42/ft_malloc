#include "ft_malloc.h"

void    *calloc(size_t count, size_t size)
{
    size_t   value;
    char     *str;
    void     *ret;
    size_t   i;

    pthread_mutex_lock(&g_mutex);
    value = count * size;
    if (!(ret = _malloc(value)))
    {
        pthread_mutex_unlock(&g_mutex);
        return (NULL);
    }
    str = ret;
    i = -1;
    while (++i < value)
        str[i] = 0;
    pthread_mutex_unlock(&g_mutex);
    return ret;
}

