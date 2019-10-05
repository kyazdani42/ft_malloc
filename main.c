#include "ft_malloc.h"

int     len(char *str)
{
    int i;

    i = 0;
    while (str[i])
        i++;
    return i;
}

void    write_str(char *str)
{
    write(1, str, len(str));
}

void    valid(void)
{
    write_str("\x1b[32m✓\x1b[0m\n");
}

void    err(void)
{
    write_str("\x1b[31m✕\x1b[0m\n");
}

int     main()
{
    write_str("test: malloc(0) ");
    if (malloc(0) == NULL)
        valid();
    else
        err();

    write_str("test: simple malloc ");
    if (malloc(10) != NULL)
        valid();
    else
        err();
    show_alloc_mem();
}
