//
// Created by souzajbr on 13/09/2019.
//

#include <stdlib.h>
#include "error.h"

error_stack *errors_head = NULL;

void error_push(int lineNumber, char *message) {

    error_stack *newError = malloc(sizeof(error_stack));

    newError->lineNumber = lineNumber;
    newError->message = message;

    newError->next = errors_head;
    errors_head = newError;
}

error_stack *error_pop() {


    error_stack *top = errors_head;

    if (errors_head != NULL)
        errors_head = top->next;

    return top;
}
