#include "ft_malloc.h"

void    *valloc(size_t size)
{
    (void)size;
    show_alloc_mem();
    return (NULL);
}
