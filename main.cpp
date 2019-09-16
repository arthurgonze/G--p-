#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include "SymbolTable.h"
#pragma pack(1)
/// IO FUNCTIONS

#define PERMS 0666 /*RW for owner, group, others */

void error(char *, ...);

// END IO FUNCTIONS

/// MAIN
/**
 * cp: copy f1 to f2
 * @return
 */
int main(int argc, char *argv[])
{
    SymbolTable table;
//    char *name = new char[M];
//    char *classtype = new char[M];
//    name[0] = 't';
//    name[1] = 'e';
//    name[2] = 's';
//    name[3] = 't';
//    classtype[0] = 'I';
//    classtype[1] = 'D';
//    table.cInsert(name, classtype);
//    table.cInsert(name, classtype);



    for (int j = 0; j < CHAIN_LENGTH*100; j++)
    {
        char *buffer = (char *) malloc(CHAIN_LENGTH + 1);
        char *buffer2 = (char *) malloc(CHAIN_LENGTH + 1);

        if (buffer==NULL)
            exit(1);
        if (buffer2==NULL)
            exit(2);
        for (int i = 0; i < 2; i++)
        {
            buffer2[i] = rand()%26 + 'A';
        }

        for (int i = 0; i < 5; i++)
        {
            buffer[i] = rand()%26 + 'a';
        }
        table.cInsert(buffer, buffer2);
    }

    table.showSymbolTable();
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