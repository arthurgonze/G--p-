#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "token.h" // token definitions

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

    return 0;
}