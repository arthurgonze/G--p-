#include <cstring>

#include "symbol_table.h"
#include "lexycal.h"
#include "token.h"
#include "parser.h"
#include "semantic.h"

#define RETURN_CODE_OK 0
#define RETURN_CODE_FILE_ERROR 1
#define RETURN_CODE_LEXICAL_ERROR 2

int main(int argc, char *argv[]) {

    int returnCode = RETURN_CODE_OK; //Process exit return code

    //Checks the first argument and open the correct input (stdin or file)
    FILE *input = stdin;
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
        if (fileName == NULL)
            return RETURN_CODE_FILE_ERROR;

        input = fopen(fileName, "r");
        if (!input) {
            printf("Could not open the source file '%s'", fileName);
            return RETURN_CODE_FILE_ERROR;
        }
    }

    lexical_analyzer_init(input);

    Parser *parser = new Parser();
    ProgramNode *ast = parser->StartParser();
    lexical_analyzer_dispose();
    delete (parser);
    startSemantic(ast);
    Translator *translator = endSemantic();

    if(translator)
    {
        startTranslator(ast, translator);
        delete (ast);
        endTranslator(translator);
    }
    delete (translator);
    printf("\n");

    return returnCode;
}
