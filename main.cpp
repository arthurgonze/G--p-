#include "SymbolTable.h"
#include "lexical/analyzer.h"
#include "token.h"
#include "syntactical/Parser.h"

#define RETURN_CODE_OK 0
#define RETURN_CODE_FILE_ERROR 1
#define RETURN_CODE_LEXICAL_ERROR 2

void send_error_to_stderr(char const *fmt, ...);

void print_symbol_table(SymbolTable symbolTable, bool showHeader, bool showToken, bool showLexeme,
                        bool showInternalCode, char const *tableName)
{
    struct symbol_info **block = symbolTable.block;

    printf("\nTABELA DE SÍMBOLOS: %s\n", tableName);
    printf("-------------------------------------------\n");

    //Check if column names headers needs to be printed
    //If necessary, print selected;
    if (showHeader)
    {
        if (showToken)
            printf("TOKEN\t\t");

        if (showLexeme)
            printf("LEXEMA\t\t");

        if (showInternalCode)
            printf("CÓDIGO");

        printf("\n-------------------------------------------\n");
    }
    for (int i = 0; i < TABLE_SIZE; ++i)
    { //Get all the entries

        symbol_info *temp = block[i];
        if (temp==NULL) //If its null, nothing to be done
            continue;

        while (temp!=NULL)
        {

            //Verify flags to print only requested columns
            if (showToken)
                printf("%s\t\t", token_id_to_name(temp->token));

            if (showLexeme)
                printf("%s\t\t", symbolTable.lexemeArray + temp->pos);

            if (showInternalCode)
                printf("%d", temp->token);

            printf("\n");
            temp = temp->next;

        }

    }

    printf("\n");
}

int main(int argc, char *argv[])
{

    int returnCode = RETURN_CODE_OK; //Process exit return code

    //Checks the first argument and open the correct input (stdin or file)
    FILE *input = stdin;
    if (argc!=1)
    { //If it has arguments, open the file

        char *fileName = argv[1];
        size_t argumentSize = strlen(argv[1]);
        const char *dot = strrchr(argv[1], '.');
        if (!dot || dot==argv[1])
        { //Check for dot in fileName, but ignore if its on start (hidden file)

            fileName = (char *) malloc(argumentSize + 5); // Adds 5 chars do add the extension (.cmm (4) + \0 (1))
            strcpy(fileName, argv[1]); //Copy the file
            strcpy(fileName + argumentSize, ".cmm"); //Add the extension

        }

        input = fopen(fileName, "r");
        if (!input)
            return RETURN_CODE_FILE_ERROR;
    }

    lexical_analyzer_init(input);

    Parser *parser = new Parser();
    parser->StartParser();

    //Print every token found on input
//    struct token_info token;
//    do
//    {
//
//        token = lexical_analyzer_next_token();
//        printf("%s", token_id_to_name(token.token));
//
//        switch (token.token)
//        {
//            case ID:
//            case NUMINT:
//            case NUMFLOAT:
//            case LITERAL:
//            case LITERALCHAR:printf(".%s", token.lexeme);
//        }
//
//        printf("\n");
//
//    }
//    while (token.token!=ENDOFFILE);
//
//    printf("\n");

    lexical_analyzer_dispose();

    //Print the symbol tables for reserved words, identifiers and literals
    print_symbol_table(get_reserved_words_table(), true, true, false, true, "PALAVRAS RESERVADAS");
    print_symbol_table(get_identifiers_table(), false, false, true, false, "IDENTIFICADORES");
    print_symbol_table(get_literals_table(), false, false, true, false, "LITERAIS");

    return 0;
}

/**
 * error: print an error message and die
 * @param fmt
 * @param ...
 */
void send_error_to_stderr(char const *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    fprintf(stderr, "error: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}
