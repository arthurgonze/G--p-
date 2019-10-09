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

error_stack* aux_error_push(error_stack* actual, error_stack* insert) {

    if(actual != NULL) {
        actual->next = aux_error_push(actual->next, insert);
        return actual;
    }


    return insert;
}
void error_push(int lineNumber, int columnNumber, char  const*message) {

    error_stack *newError = malloc(sizeof(error_stack));

    newError->lineNumber = lineNumber;
    newError->columnNumber = columnNumber;
    newError->message = message;

    newError->next = NULL;
    errors_head = aux_error_push(errors_head, newError);
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
