//
// Created by souzajbr on 14/09/2019.
//
#ifdef __cplusplus
extern "C" {
#endif

#ifndef COMPILADOR_2019_3_IO_H
#define COMPILADOR_2019_3_IO_H

#include "stdio.h"

#define BUFSIZE 4096

bool io_init_with_stdin();
bool io_init_with_file(char* path);
int io_get_next_char();

#endif //COMPILADOR_2019_3_IO_H
#ifdef __cplusplus
}
#endif