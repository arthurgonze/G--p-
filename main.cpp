#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include "SymbolTable.h"
#include "lexical/io.h"
#include "lexical/analyzer.h"
#include "lexical/error.h"
#include "token.h"
/// IO FUNCTIONS


void error(char *, ...);
// END IO FUNCTIONS

/// MAIN
/**
 * cp: copy f1 to f2
 * @return
 */
const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) return "";
    return dot + 1;
}

void showSymbolTable(struct SymbolTable symbolTable, bool showToken, bool showLexeme)
{
    struct symbol_info** block = symbolTable.block;
    int cont = 0;
    // Implement
    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        // Do not modify the head
        symbol_info *temp = block[i];
        if(temp == NULL)
            continue;

        while (temp!=NULL)
        {
            if(showToken)
                printf("%s ", token_id_to_name(temp->token));

            if(showLexeme)
                printf("%s", symbolTable.lexemeArray + temp->pos);

            printf("\n");
            temp = temp->next;
            cont++;
        }

        cont = 0;
    }
}


int main(int argc, char *argv[]) {

    if (argc == 1) io_init_with_stdin();

    else if (!io_init_with_file(argv[1]))
        return 1;

    lexical_analyzer_init();

    struct token_info token;
    do {
        token = lexical_analyzer_next_token();
        printf("%s.%s\n", token_id_to_name(token.token), token.lexeme);

    } while (token.token != ENDOFFILE);

    printf("\n");

    lexical_analyzer_dispose();

    printf("========================\n");
    printf("LITERAIS\n");
    showSymbolTable(get_literals_table(), false, true);
    printf("========================\n");

    printf("\n");

    printf("========================\n");
    printf("IDENTIFICADORES\n");
    showSymbolTable(get_identifiers_table(), false, true);
    printf("========================\n");

    printf("========================\n");
    printf("PALAVRAS RESERVADAS\n");
    showSymbolTable(get_reserved_words_table(), true, false);
    printf("========================\n");
    error_stack *error_info;
    while ((error_info = error_pop()) != NULL) {
        error("[LEXICAL ERROR] Line %d: %s", error_info->lineNumber, error_info->message);
    }


    return 0;
}


/// IO FUNCTIONS
/**
 * error: print an error message and die
 * @param fmt
 * @param ...
 */
void error(char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    fprintf(stderr, "error: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}
/// END OF IO FUNCTIONS