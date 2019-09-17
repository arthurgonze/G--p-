#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <zconf.h>
#include "SymbolTable.h"
#include "lexical/io.h"
#include "lexical/analyzer.h"
#include "lexical/error.h"
#include "token.h"
/// IO FUNCTIONS

#define PERMS 0666 /*RW for owner, group, others */

void error(char *, ...);

// END IO FUNCTIONS

/// MAIN
/**
 * cp: copy f1 to f2
 * @return
 */

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) return "";
    return dot + 1;
}

struct symbol_info {
    int token;
    int pos;
    struct symbol_info *next;
} *block[CHAIN_LENGTH];

int main(int argc, char *argv[]) {

    if(argc == 1) io_init_with_stdin();

    else
        if (!io_init_with_file(argv[1]))
            return 1;

    lexical_analyzer_init();

    int token = 0;
    do  {
        token = lexical_analyzer_next_token();
        printf("%s\n", token_id_to_name(token));

    } while(token != ENDOFFILE);

    error_stack* error_info;
    while((error_info = error_pop()) != NULL) {
        error("[LEXICAL ERROR] Line %d: %s", error_info->lineNumber, error_info->message);
    }

//    char *name = new char[M];
//    char *classtype = new char[M];
//    SymbolTable table;
//    name[0] = 't';
//    name[1] = 'e';
//    name[2] = 's';
//    name[3] = 't';
//    classtype[0] = 'I';
//    classtype[1] = 'D';
//    table.cInsert(name,classtype);
//    printf("%d", table.cSearch(name,classtype));


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
void error(char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    fprintf(stderr, "error: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}
/// END OF IO FUNCTIONS