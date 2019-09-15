//
// Created by souzajbr on 14/09/2019.
//

#ifndef COMPILADOR_2019_3_IO_H
#define COMPILADOR_2019_3_IO_H

#include "stdio.h"

#define BUFSIZE 4096

int io_get_next_char_from_stdin(void);
int io_get_next_char_from_file(FILE *stream);

#endif //COMPILADOR_2019_3_IO_H
