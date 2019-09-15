#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "token.h" // token definitions
#include "lexical/analyzer.h"
#include "lexical/error.h"
#include <fcntl.h>
#include <zconf.h>


/// IO FUNCTIONS

#define PERMS 0666 /*RW for owner, group, others */

void error(char *, ...);
// END IO FUNCTIONS

/// MAIN
/**
 * cp: copy f1 to f2
 * @return
 */
int main(int argc, char *argv[]) {

    int f1, f2;
    char ch;

    if (argc == 1) {
        while ((ch = getchar()) != EOF) {
            printf("%c", ch);
        }
    } else {
        if (argc != 3) {
            error("Usage: cp from to \n");
        } else if (argc == 3) {

            if ((f1 = open(argv[1], O_RDONLY, 0)) == -1) {
                error("cp: can't open %s \n", argv[1]);
            }

            if ((f2 = creat(argv[2], PERMS)) == -1) {
                error("cp: can't creat %s, mode %03o \n", argv[2], PERMS);
            }

            while ((ch = getc(f1)) != EOF) {
                printf("%c", ch);
            }


        }
        printf("%d\n", INT);
        lexical_analyzer_init();

        int c = 0;
        do {
            c = lexical_analyzer_next_token();
            printf("%s\n", idToTokenName(c));
        } while (c != ENDOFFILE);

        lexical_analyzer_dispose();

        error_stack *errorStackPointer;
        while ((errorStackPointer = error_pop()) != NULL) {
            printf("[ERR] line %d -> %s\n", errorStackPointer->lineNumber, errorStackPointer->message);
        }
    }
    return 0;
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
    vprintf(fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(1);
}
/// END OF IO FUNCTIONS