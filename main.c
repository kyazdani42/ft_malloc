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
    void    *test;
    size_t  val;
    int     i;

    i = -1;
    printf("testing tiny allocs\n");
    while (++i < 100)
    {
        val = rand() % 128;
        /* printf("testing malloc(%d)\n", val); */
        if (!(test = malloc(val)))
            err();
        /* else */
        /*     valid(test); */
    }

    i = -1;
    printf("testing small allocs\n");
    while (++i < 100)
    {
        val = rand() % 512 + 128;
        /* printf("testing malloc(%d)\n", val); */
        if (!(test = malloc(val)))
            err();
        /* else */
        /*     valid(test); */
    }

    i = -1;
    printf("testing large allocs\n");
    while (++i < 10000)
    {
        val = rand() % 1000 + 512;
        /* printf("testing malloc(%d)\n", val); */
        if (!(test = malloc(val)))
        {
            err();
            show_alloc_mem();
            printf("%zu\n", val);
            exit(1);
        }
        /* else */
        /*     valid(test); */
    }
        /* show_alloc_mem(); */
}
