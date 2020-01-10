#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char find(const char* from, const char* to, char letter, size_t len);

int main(int argc, const char* argv[])
{
    /* Check that two operands are given. NOTE: argc is always at least 1 */
    if (argc != 3)
    {
        char msg[] = "Error: Program must be given two operands!\n";
        write(STDERR_FILENO, msg, sizeof(msg));
        exit(1);
    }

    /* DECLARE VARIABLES */
    const char* from, * to;
    size_t from_len, to_len;
    ssize_t ret_val;
    char curr[1];

    /* INITIALIZE VARIABLES */
    from = argv[1];
    to = argv[2];
    from_len = strlen(from);
    to_len = strlen(to);

    /* Check that from and to have the same lengths */
    if (from_len != to_len)
    {
        char msg[] = "Error: Operands must have the same length\n";
        write(STDERR_FILENO, msg, sizeof(msg));
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
                char msg[] = "Error: From must not contain duplicate bytes\n";
                write(STDERR_FILENO, msg, sizeof(msg));
                exit(1);
            }
        }
    }

    /* Transliterate bytes */
    while (1)
    {
        ret_val = read(STDIN_FILENO, curr, 1);
        
        if (! ret_val)
            break;
        else if (ret_val == -1)
        {
            char msg[] = "Error: Reading unsuccessful\n";
            write(STDERR_FILENO, msg, sizeof(msg));
            exit(1);
        }
        
        curr[0] = find(from, to, curr[0], from_len);
        
        ret_val = write(STDOUT_FILENO, curr, 1);
        if (ret_val == -1)
        {
            char msg[] = "Error: Writing unsuccessful\n";
            write(STDERR_FILENO, msg, sizeof(msg));
            exit(1);
        }
    }
    
    return 0;
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
