#include "ft_malloc.h"

inline static int   should_resize(t_alloc *header, size_t size)
{
    return  header->next
        && header->next->free
        && header->next->zone == header->zone
        && header->next->size + HEADER + header->size >= size;
}

static void	*_reallocf(void *ptr, size_t size)
{
    t_alloc     *header;
    t_alloc     *next;
    void        *new_ptr;
    size_t      aligned_size;

    if (!(header = get_header_from_addr(ptr)))
        return (ptr);

    aligned_size = get_multiple_of(size, 16);

    if (aligned_size <= header->size)
        return (ptr);

    if (should_resize(header, aligned_size))
    {
        next = header->next;
        header->size += HEADER + next->size;
        header->next = next->next;
        return (ptr);
    }
    if (!(new_ptr = _malloc(aligned_size)))
    {
        free(ptr);
        return (NULL);
    }

    copy_memory(new_ptr, ptr, header->size);
    _free(ptr);
    return (new_ptr);
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
