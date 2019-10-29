#include <cstring>

#include "SymbolTable.h"
#include "lexical/analyzer.h"
#include "token.h"

#define RETURN_CODE_OK 0
#define RETURN_CODE_FILE_ERROR 1
#define RETURN_CODE_LEXICAL_ERROR 2

void print_symbol_table(SymbolTable symbolTable, bool showHeader, bool showToken, bool showLexeme,
                        bool showInternalCode, char const *tableName) {
    struct symbol_info **block = symbolTable.block;

    printf("\nTABELA DE SÍMBOLOS: %s\n", tableName);
    printf("-------------------------------------------\n");

    //Check if column names headers needs to be printed
    //If necessary, print selected;
    if (showHeader) {
        if (showToken)
            printf("TOKEN\t\t");

        if (showLexeme)
            printf("LEXEMA\t\t");

        if (showInternalCode)
            printf("CÓDIGO");

        printf("\n-------------------------------------------\n");
    }
    for (int i = 0; i < TABLE_SIZE; ++i) { //Get all the entries

        symbol_info *temp = block[i];
        if (temp == NULL) //If its null, nothing to be done
            continue;

        while (temp != NULL) {

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

int main(int argc, char *argv[]) {

    int returnCode = RETURN_CODE_OK; //Process exit return code


    //Checks the first argument and open the correct input (stdin or file)
    FILE* input = stdin;
    if (argc != 1) { //If it has arguments, open the file

        char *fileName = argv[1];
        size_t argumentSize = strlen(fileName);
        const char *dot = strrchr(fileName, '.');
        if (!dot || dot == fileName) { //Check for dot in fileName, but ignore if its on start (hidden file)

            fileName = (char *) malloc(argumentSize + 5); // Adds 5 chars do add the extension (.cmm (4) + \0 (1))
            
			if (!fileName)
				return -1;
			
			strcpy(fileName, argv[1]); //Copy the file
            strcpy(fileName + argumentSize, ".cmm"); //Add the extension

        }
		if(fileName == NULL)
			return RETURN_CODE_FILE_ERROR;

        input = fopen(fileName, "r");
		if (!input)
		{
			printf("Could not open the source file '%s'", fileName);
			return RETURN_CODE_FILE_ERROR;
		}
    }

    lexical_analyzer_init(input);

    //Print every token found on input
	int token;
    do {

        token = lexical_analyzer_next_token();
        printf("%s", token_id_to_name(token));


        switch (token) {
            case ID:
            case NUMINT:
            case NUMFLOAT:
            case LITERAL:
            case LITERALCHAR:
                printf(".%s", lexical_analyzer_last_lexeme());
        }

        printf("\n");

    } while (token != ENDOFFILE);

    printf("\n");

    lexical_analyzer_dispose();

    //Print the symbol tables for reserved words, identifiers and literals
    print_symbol_table(get_reserved_words_table(), true, true, false, true, "PALAVRAS RESERVADAS");
    print_symbol_table(get_identifiers_table(), false, false, true, false, "IDENTIFICADORES");
    print_symbol_table(get_literals_table(), false, false, true, false, "LITERAIS");

    return returnCode;
}
