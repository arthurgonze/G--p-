//
// Created by souzajbr on 14/09/2019.
//

#include <stdio.h>
#include <stdbool.h>
#include "unistd.h"
#include <fcntl.h>

#include "io.h"

int fd = -1;
/**
 * getc: simple buffered version
 * @return
 */



bool io_init_with_file(char *path) {
    fd = open(path, O_RDONLY);
    return fd > 0;
}

bool io_init_with_stdin() {
    fd = 0;
    return true;
}

int io_get_next_char() {
    static char buf[BUFSIZE];
    static char *bufp = buf;
    static int n = 0;
    /* Buffer is empty */
    if (n == 0) {
        n = read(fd, buf, BUFSIZE);
        bufp = buf;
    }

    return (--n >= 0) ? (unsigned char) *bufp++ : EOF;
}

