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

void print_symbol_table(struct SymbolTable symbolTable, bool showHeader, bool showToken, bool showLexeme, bool showInternalCode, char* tableName)
{
    struct symbol_info** block = symbolTable.block;
    int cont = 0;

    printf("\nTABELA DE SÍMBOLOS: %s\n", tableName);
    printf("-------------------------------------------\n");

    if(showHeader) {
        if (showToken)
            printf("TOKEN\t\t");

        if (showLexeme)
            printf("LEXEMA\t\t");

        if (showInternalCode)
            printf("CÓDIGO");

        printf("\n-------------------------------------------\n");
    }
    for (int i = 0; i < TABLE_SIZE; ++i)
    {

        symbol_info *temp = block[i];
        if(temp == NULL)
            continue;

        while (temp!=NULL)
        {

            //Verify flags to print only requested columns
            if(showToken)
                printf("%s\t\t", token_id_to_name(temp->token));

            if(showLexeme)
                printf("%s\t\t", symbolTable.lexemeArray + temp->pos);

            if(showInternalCode)
                printf("%d", temp->token);

            printf("\n");
            temp = temp->next;
            cont++;
        }

        cont = 0;
    }

    printf("\n");
}

int main(int argc, char *argv[]) {

    int tokenWithLexeme[5] = {ID, LITERAL, LITERALCHAR, NUMFLOAT, NUMINT}; //Define tokens that's needed to print the lexeme
    bool isLexemeNeeded = false; //Flag to print lexeme

    //Open the correct input (stdin or file)
    if (argc == 1) io_init_with_stdin();
    else if (!io_init_with_file(argv[1]))
        return 1;

    lexical_analyzer_init();

    //Print every token found on input
    struct token_info token;
    do {

        token = lexical_analyzer_next_token();
        printf("%s",  token_id_to_name(token.token));

        //Check if the lexeme must be printed
        isLexemeNeeded = false;
        for(int i = 0; i < 4; i++) {
            if(tokenWithLexeme[i] == token.token)
                isLexemeNeeded = true;
        }

        if(isLexemeNeeded)
            printf(".%s",  token.lexeme);

        printf("\n");

    } while (token.token != ENDOFFILE);

    printf("\n");

    lexical_analyzer_dispose();

    //Print the symbol tables for reserved words, identifiers and literals
    print_symbol_table(get_reserved_words_table(), true, true, false, true, "PALAVRAS RESERVADASs");
    print_symbol_table(get_identifiers_table(), false, false, true, false, "IDENTIFICADORES");
    print_symbol_table(get_identifiers_table(), false, false, true, false, "LITERAIS");

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