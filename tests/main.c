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
    void    *ret4;
    char    *ret5;
    void    *ret6;
    void    *ret7;

    ret = malloc(10);
    ret2 = malloc(30);
    free(ret);
    ret3 = malloc(20);
    ret4 = malloc(128);
    show_alloc_mem();
    free(ret2);
    show_alloc_mem();
    free(ret3);
    show_alloc_mem();
    ret5 = (char *)malloc(50);
    ret6 = malloc(2);
    free(ret6);
    ret5[0] = 'a';
    ret5[2] = 'a';
    ret5[10] = 'a';
    ret5[49] = 'a';
    free(ret4);
    ret5[2] = 'a';
    ret7 = malloc(0);
    free(ret5);
    free(ret7);
}

int     main()
{
    only_tiny();

    show_alloc_mem();

    return (0);
}
