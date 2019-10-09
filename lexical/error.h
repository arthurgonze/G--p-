//
// Created by souzajbr on 13/09/2019.
//
#ifdef __cplusplus
extern "C" {
#endif

#ifndef COMPILADOR_2019_3_ERROR_H
#define COMPILADOR_2019_3_ERROR_H

struct _error_stack {
    struct _error_stack *next;
    int lineNumber;
    int columnNumber;
    char const *message;
};

typedef struct _error_stack error_stack;

void error_push(int lineNumber, int columnNumber, char const *message);

error_stack *error_pop();

#endif //COMPILADOR_2019_3_ERROR_H
#ifdef __cplusplus
}
#endif