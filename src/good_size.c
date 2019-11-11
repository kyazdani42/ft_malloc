#include "ft_malloc.h"

size_t          malloc_size(const void *ptr) {
    t_alloc     *cur;

    if (!(get_zone_set_cur((void *)ptr, &cur)))
        return (0);
    return (cur->size);
}

size_t          malloc_good_size(size_t size)
{
    return get_multiple_of(size, 16);
}
