//
// Created by souzajbr on 14/09/2019.
//

#include <stdio.h>
#include "unistd.h"

#include "io.h"

/**
 * getchar: simple buffer version
 * @return
 */
int io_get_next_char_from_stdin(void) {
    static char buf[BUFSIZE];
    static char *bufp = buf;
    static int n = 0;
    /* Buffer is empty */
    if (n == 0) {
        n = read(0, buf, sizeof(buf));
        bufp = buf;
    }

    return (--n >= 0) ? (unsigned char) *bufp++ : EOF;
}

/**
 * getc: simple buffered version
 * @return
 */
int io_get_next_char_from_file(FILE *stream) {
    static char buf[BUFSIZE];
    static char *bufp = buf;
    static int n = 0;
    /* Buffer is empty */
    if (n == 0) {
        n = read(stream, buf, sizeof(buf));
        bufp = buf;
    }

    return (--n >= 0) ? (unsigned char) *bufp++ : EOF;
}

