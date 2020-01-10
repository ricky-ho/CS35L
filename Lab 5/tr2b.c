#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void checkIOError(FILE* fp, const char* msg);
char find(const char* from, const char* to, char letter, size_t len);

int main(int argc, const char* argv[])
{
    /* Check that two operands are given. NOTE: argc is always at least 1 */
    if (argc != 3)
    {
        fprintf(stderr, "Error: Program must be given two operands!\n");
        exit(1);
    }

    /* DECLARE VARIABLES */
    const char* from, * to;
    size_t from_len, to_len;
    char curr;

    /* INITIALIZE VARIABLES */
    from = argv[1];
    to = argv[2];
    from_len = strlen(from);
    to_len = strlen(to);

    /* Check that from and to have the same lengths */
    if (from_len != to_len)
    {
        fprintf(stderr, "Error: Operands must have the same length\n");
        exit(1);
    }

    /* Check for duplicate bytes in from */
    const char* p, *q;
    for (p = from; *p ; p++)
    {
        for (q = p+1; *q; q++)
        {
            if (*p == *q)
            {
                fprintf(stderr, "Error: From must not contain duplicate bytes\n");
                exit(1);
            }
        }
    }

    /* Transliterate bytes */
    while (1)
    {
        curr = getchar();
        checkIOError(stdin, "Input");

        if (feof(stdin))
            break;

        putchar(find(from, to, curr, from_len));
        checkIOError(stdout, "Output");
    }
    
    return 0;
}

void checkIOError(FILE* fp, const char* msg)
{
    if (ferror(fp) != 0)
    {
        fprintf(stderr, "%s Error!\n", msg);
        exit(1);
    }
}

char find(const char* from, const char* to, char letter, size_t len)
{
    size_t it;
    for (it = 0; it < len; it++)
    {
        if (from[it] == letter)
            return to[it];
    }
    return letter;
}
