#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <zconf.h>
#include "SymbolTable.h"

/// IO FUNCTIONS
#define BUFSIZE 4096
#define PERMS 0666 /*RW for owner, group, others */
#undef getchar
#undef getc

int getchar(void);
int getc(FILE *stream);
void error(char *, ...);

// END IO FUNCTIONS

/// MAIN
/**
 * cp: copy f1 to f2
 * @return
 */
int main(int argc, char *argv[])
{
    struct SymbolTable symbolTable;

    symbolTable.insert("ID","test1");
    symbolTable.insert("NUMFLOAT","2.0");

//    symbolTable.contains("test1");
//    symbolTable.contains("2.0");
//    symbolTable.contains("You Think I'm fat, huh? Fat men got a gun bitch! Yeah, fat fools packin' bitch! You think I ain't for real?");

    return 0;

//    int f1, f2;
//    char ch;
//
//    if (argc==1)
//    {
//        while ((ch = getchar())!=EOF)
//        {
//            printf("%c", ch);
//        }
//    }
//    else
//    {
//        if (argc!=3)
//        {
//            error("Usage: cp from to \n");
//        }
//        else if (argc==3)
//        {
//
//            if ((f1 = open(argv[1], O_RDONLY, 0))==-1)
//            {
//                error("cp: can't open %s \n", argv[1]);
//            }
//
//            if ((f2 = creat(argv[2], PERMS))==-1)
//            {
//                error("cp: can't creat %s, mode %03o \n", argv[2], PERMS);
//            }
//
//            while ((ch = getc(f1))!=EOF)
//            {
//                printf("%c", ch);
//            }
//
//        }
//    }
}


/// IO FUNCTIONS
/**
 * getchar: simple buffere version
 * @return
 */
int getchar(void)
{
    static char buf[BUFSIZE];
    static char *bufp = buf;
    static int n = 0;
    /* Buffer is empty */
    if (n==0)
    {
        n = read(0, buf, sizeof(buf));
        bufp = buf;
    }

    return (--n >= 0) ? (unsigned char) *bufp++ : EOF;
}

/**
 * getc: simple buffered version
 * @return
 */
int getc(FILE *stream)
{
    static char buf[BUFSIZE];
    static char *bufp = buf;
    static int n = 0;
    /* Buffer is empty */
    if (n==0)
    {
        n = read(reinterpret_cast<ssize_t>(stream), buf, sizeof(buf));
        bufp = buf;
    }

    return (--n >= 0) ? (unsigned char) *bufp++ : EOF;
}

/**
 * error: print an error message and die
 * @param fmt
 * @param ...
 */
void error(char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    fprintf(stderr, "error: ");
    vprintf(fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(1);
}
/// END OF IO FUNCTIONS