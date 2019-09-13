//
// Created by souzajbr on 13/09/2019.
//

#ifndef COMPILADOR_2019_3_ERROR_H
#define COMPILADOR_2019_3_ERROR_H

struct _error_stack {
    struct _error_stack *next;
    int lineNumber;
    char *message;
};

typedef struct _error_stack error_stack;

void error_push(int lineNumber, char *message);

error_stack *error_pop();

#endif //COMPILADOR_2019_3_ERROR_H
