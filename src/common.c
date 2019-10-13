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

int     next_valid(t_alloc *alloc)
{
    return alloc->next && (void *)alloc + alloc->size == alloc->next;
}

int     prev_valid(t_alloc *alloc)
{
    return alloc->prev && (void *)alloc - alloc->prev->size - HEADER == alloc->prev;
}

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

void    *reallocf(void *ptr, size_t size)
{
    (void)ptr;
    (void)size;
    putstr("reallocf !\n");
    return (NULL);
}

void    *valloc(size_t size)
{
    (void)size;
    putstr("valloc !\n");
    return (NULL);
}
