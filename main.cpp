#include <cstring>

#include "SymbolTable.h"
#include "lexical/analyzer.h"
#include "token.h"
#include "syntactical/Parser.h"

#define RETURN_CODE_OK 0
#define RETURN_CODE_FILE_ERROR 1
#define RETURN_CODE_LEXICAL_ERROR 2

int main(int argc, char *argv[])
{

    int returnCode = RETURN_CODE_OK; //Process exit return code

    //Checks the first argument and open the correct input (stdin or file)
    FILE *input = stdin;
    if (argc!=1)
    { //If it has arguments, open the file

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

    Parser *parser = new Parser();
    parser->StartParser();

	//Print the symbol tables for reserved words, identifiers and literals
	get_reserved_words_table().print();
	get_identifiers_table().print();
	get_literals_table().print();

    lexical_analyzer_dispose();

	printf("\n");

    return 0;
}
