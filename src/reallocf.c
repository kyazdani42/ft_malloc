#include "ft_malloc.h"

static void	copy_memory(void **to, void *from, size_t n)
{
	char		*dest;
	char	    *src;

    dest = *to;
    src = from;
	while (n--)
    {
		*dest = *src;
        dest++;
        src++;
    }
    dest = *to;
}
void    *reallocf(void *ptr, size_t size)
{
    t_alloc     *header;
    void        *new_value;
    size_t      aligned_size;

    if (!ptr)
        return malloc(size);

    if (!size)
    {
        free(ptr);
        return (malloc(0));
    }

    header = get_header_from_addr(ptr);
    if (!header)
        return (ptr);

    aligned_size = get_multiple_of(size, 16);

    if (aligned_size <= header->size)
        return (ptr);
    new_value = malloc(aligned_size);
    copy_memory(&new_value, ptr, header->size);
    free(ptr);
    return (new_value);
}
