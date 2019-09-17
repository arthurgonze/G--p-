#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include "SymbolTable.h"
#include "lexical/io.h"
#include "lexical/analyzer.h"
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
    if(!dot || dot == filename) return "";
    return dot + 1;
}

int main(int argc, char *argv[])
{

    if(argc == 1) io_init_with_stdin(); else io_init_with_file(argv[1]);

    lexical_analyzer_init();

    int token = 0;

    return 0;
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