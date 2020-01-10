#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <getopt.h>

/* FUNCTION DECLARATIONS */
int frobcmp(char const* a, char const* b);
int callFrob(const void* a, const void* b);
void checkError(ssize_t val);
void checkMemError(void* ptr);
void outputWords(char const* words);
void parseOpt(int argc, char* argv[]);
void storeAllWords(char** all_words, char* buf, size_t size);

/* GLOBAL VARIABLES */
int f_option;
int counter;

int main(int argc, char* argv[])
{
    /* PARSE FOR F-OPTION */
    parseOpt(argc, argv);
    
    /* DECLARE VARIABLES */
    int (* callCmp) (const void*, const void*);
    char* buffer1, * tempbuf;
    char** all_words;
    char charbuf[2], curr_char;
    size_t file_size, num_words, word_size, buf_size;
    struct stat buf;
    ssize_t ret_val;
    int is_EOF;
    
    /* INITIALIZE VARIABLES */
    callCmp = &callFrob;
    num_words = 0; word_size = 0; buf_size = 0;
    is_EOF = 0;
    
    /* FILE INFORMATION */
    ret_val = fstat(STDIN_FILENO, &buf);
    checkError(ret_val);
    file_size = buf.st_size + 1;
    
    buffer1 = (char*) malloc(sizeof(char) * file_size);
    checkMemError(buffer1);
    
    /* While we have not reached end of file, read from stdin */
    while (! is_EOF)
    {
        ret_val = read(STDIN_FILENO, charbuf, 1);
        checkError(ret_val);
        curr_char = charbuf[0];
        is_EOF = ! ret_val;
        if (word_size == 0 && curr_char == ' ')
            continue;
        
        /* If the buffer is full, reallocate more memory */
        if (buf_size == file_size)
        {
            file_size *= 2;
            tempbuf = (char*) realloc(buffer1, sizeof(char) * file_size);
            checkMemError(tempbuf);
            buffer1 = tempbuf;
        }
        
        /* Store the current char into the buffer */
        if (! is_EOF)
        {
            buffer1[buf_size] = curr_char;
            buf_size += 1;
            word_size += 1;
            if (curr_char != ' ')
                continue;
        }
        else
        {
            /* We have an empty file or a file with only space chars */
            if (buf_size == 0)
            {
                free(buffer1);
                return 0;
            }
            
            /* Add a space to the end if none */
            if (buffer1[buf_size - 1] != ' ')
            {
                buffer1[buf_size] = ' ';
                buf_size += 1;
            }
            if (word_size == 0)
                break;
        }
        
        num_words += 1;
        word_size = 0;
    }
    
    /* Allocate memory to hold all words */
    all_words = (char**) malloc(sizeof(char*) * num_words);
    checkMemError(all_words);
    
    /* Store the words from buffer1 into all_words */
    storeAllWords(all_words, buffer1, buf_size);

    /* Sort the input */
    qsort(all_words, num_words, sizeof(char*), callCmp);
    
    /* Output the results */
    size_t i = 0;
    for (i = 0; i < num_words; i++)
        outputWords(all_words[i]);

    /* Free Memory */
    free(all_words);
    free(buffer1);

    return 0;
}

/*  Compares two words lexicographically by unfrobnicating letter by letter
 *  and comparing these letters. Returns negative, zero, or positive if 
 *  a < b, a == b, a > b, respectively.  */
int frobcmp(char const* a, char const*b)
{
    char temp1, temp2;
    for (; *a != ' ' && *b != ' '; a++, b++)
    {
        temp1 = *a ^ 42;
        temp2 = *b ^ 42;

	if (f_option)
        {
            temp1 = toupper((unsigned char) temp1);
            temp2 = toupper((unsigned char) temp2);
        }
      
        if (*a == ' ' || temp1 < temp2)
            return -1;
        if (*b == ' ' || temp1 > temp2)
            return 1;
    }
    
    return 0;
}

/*  Used to pass frobcmp to qsort  */
int callFrob(const void* a, const void* b)
{
    counter += 1;
    int value = frobcmp(*(const char**) a, *(const char**) b);
    return value;
}

/*  If we received an input or output error, output to stderr and exit with status 1  */
void checkError(ssize_t val)
{
    if (val < 0)
    {
        char msg[] = "Error: System call unsuccessful\n";
        write(STDERR_FILENO, msg, sizeof(msg));
        exit(1);
    }
}

/*  We received a memory allocation error, thus we free all memory, output to stderr,
 *   and exit with status 1  */
void checkMemError(void* ptr)
{
    if (ptr == NULL)
    {
        char msg[] = "Error: Memory allocation error!\n";
        write(STDERR_FILENO, msg, sizeof(msg));
        exit(1);
    }
}

/* Function to write all the words to stdout */
void outputWords(char const* words)
{
    ssize_t ret;
    while (1)
    {
        ret = write(STDOUT_FILENO, words, 1);
        checkError(ret);
        if (*words == ' ')
        {
            return;
        }
        words += 1;
    }
}

/* Function to parse arguments for '-f' option. If incorrect option given, write to
 * stderr and exit */
void parseOpt(int argc, char* argv[])
{
    int opt;
    char msg[] = "Error: Invalid option\n";
    while ((opt = getopt(argc, argv, "f")) != -1)
    {
        switch (opt)
        {
            case 'f':
                f_option = 1;
                break;
            default:
                write(STDERR_FILENO, msg, sizeof(msg));
                exit(1);
        }
    }
}

/* Function to store each word in buf into all_words */
void storeAllWords(char** all_words, char* buf, size_t size)
{
    size_t i, num_word;
    char* word = buf;
    for (i = 0, num_word = 0; i < size; i++)
    {
        if (buf[i] == ' ')
        {
            all_words[num_word] = word;
            num_word += 1;
            word = buf + i + 1;
        }
    }
}

