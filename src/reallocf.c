#include "ft_malloc.h"

void    *_reallocf(void *ptr, size_t size)
{
    t_alloc     *header;
    void        *new_value;
    size_t      aligned_size;

    header = get_header_from_addr(ptr);
    if (!header)
        return (ptr);

    aligned_size = get_multiple_of(size, 16);

    if (aligned_size <= header->size)
        return (ptr);
    new_value = _malloc(aligned_size);
    copy_memory(&new_value, ptr, header->size);
    _free(ptr);
    return (new_value);
}

void    *reallocf(void *ptr, size_t size)
{
    void    *ret;

    if (!ptr)
        return malloc(size);

    if (!size)
    {
        free(ptr);
        return (malloc(0));
    }

    pthread_mutex_lock(&g_mutex);
    ret = _reallocf(ptr, size);
    pthread_mutex_unlock(&g_mutex);
    return (ret);
}
