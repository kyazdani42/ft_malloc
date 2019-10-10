#include "ft_malloc.h"
#include <stdlib.h>
#include <stdio.h>

void    valid(void *to_print)
{
    printf("\x1b[32m✓\x1b[0m %p\n", to_print);
}

void    err(void)
{
    printf("\x1b[31m✕\x1b[0m\n");
}

int     main()
{
    void    *ret;
    void    *ret2;
    void    *ret3;

    ret = malloc(100);
    ret2 = malloc(10);
    ret3 = malloc(40);

    show_alloc_mem();
    free(ret2);
    show_alloc_mem();

}
