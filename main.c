#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "token.h" // token definitions
#include "lexical/analyzer.h"

#define TAM 20

struct token
{
    char lexema[10];
    int pos;
};

struct token *hash[TAM];
struct token *id;
struct token *num;

int hashFunc(int key)
{
    return key%TAM;
}

struct token* search(int key)
{
    
}

int main()
{

    printf("%d\n", INT);
    init();

    int c = 0;
    while((c = getNextToken()) != -1)
        printf("%s\n", idToTokenName(c));

    return 0;
}