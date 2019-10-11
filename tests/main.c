#include "ft_malloc.h"
#include <stdio.h>

void    valid(void *to_print)
{
    printf("\x1b[32m✓\x1b[0m %p\n", to_print);
}

void    err(void)
{
    printf("\x1b[31m✕\x1b[0m\n");
}

void    only_tiny()
{
    void    *ret;
    void    *ret2;
    void    *ret3;

    ret = malloc(10);
    ret2 = malloc(30);
    ret3 = malloc(20);
    free(ret);
    free(ret3);
    free(ret2);
    ret = malloc(10);
    ret2 = malloc(40);
    free(ret);
    ret = malloc(5);
    ret3 = malloc(5);
    free(ret);
    free(ret2);
    free(ret3);
    ret2 = malloc(5);
    free(ret2);
}

void    only_large()
{
    void    *ret;
    void    *ret2;
    void    *ret3;

    ret = malloc(1000);
    ret2 = malloc(2000);
    show_alloc_mem();
    free(ret2);
    show_alloc_mem();
    ret3 = malloc(5000);
    free(ret3);
    free(ret);
}

int     main()
{
    only_tiny();
    /* only_large(); */

    show_alloc_mem();

    return (0);
}
