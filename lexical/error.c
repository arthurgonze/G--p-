//
// Created by souzajbr on 13/09/2019.
//

#include <stdlib.h>
#include "error.h"

error_stack *errors_head = NULL;

/**
 * Add error to the stack
 * @param lineNumber
 * @param columnNumber
 * @param message
 */
void error_push(int lineNumber, int columnNumber, char *message) {

    error_stack *newError = malloc(sizeof(error_stack));

    newError->lineNumber = lineNumber;
    newError->columnNumber = columnNumber;
    newError->message = message;

    newError->next = errors_head;
    errors_head = newError;
}

/**
 * Get error from stack
 * @return
 */
error_stack *error_pop() {


    error_stack *top = errors_head;

    if (errors_head != NULL)
        errors_head = top->next;

    return top;
}
