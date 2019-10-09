//
// Created by souzajbr on 14/09/2019.
//

#include <stdio.h>
#include <stdbool.h>
#include "unistd.h"
#include <fcntl.h>

#include "io.h"

FILE *fd;

/**
 * Prepare IO from file
 * @param path
 * @return
 */
bool io_init_with_file(char *path) {
    //fd = open(path, O_RDONLY);
    fd = fopen(path,"r");
    return fd != NULL;
}

/**
 * Prepare IO from stdin
 * @return
 */
bool io_init_with_stdin() {
    fd = stdin;
    return true;
}

/**
 * Returns the next char from buffer
 * @return
 */
int io_get_next_char() {
    static char buf[BUFSIZE];
    static char *bufp = buf;
    static size_t n = 0;
    /* Buffer is empty */
    if (n == 0) {
        n = fread(buf, 1, BUFSIZE,fd);
        bufp = buf;
    }

    return (--n >= 0) ? (unsigned char) *bufp++ : EOF;
}

