#ifndef COMPILADOR_2019_3_SYMBOLTABLE_H
#define COMPILADOR_2019_3_SYMBOLTABLE_H

#include<bits/stdc++.h>
using namespace std;

// It has been seen if chain length is < 50 then some space is underutilized,
// Theoretically seen if > 50, prime number is taken then chain takes moderate space.
// So thats why 53 is used.
#define TABLE_SIZE 101
#define LEXEME_ARRAY_SIZE 4096

class SymbolTable
{
    // Two columns of the symbol table with name and class type.
    struct symbol_info
    {
        int token; //token id
        int pos; // lexem position in the lexemeArray
        struct symbol_info *next; // pointer to the next symbol
    };

public:
    void cInsert(int token, char *lexeme);
    bool cSearch(char *lexeme);
    void showSymbolTable();
private:

    char *lexemeArray = new char[LEXEME_ARRAY_SIZE]; // An array of to allocate lexem in continuous memory
    int headIndex = 0; // A variable to store the first free position in this array
    int lexemeArraySize = 0; // Actual size of the lexeme array

    int cHash(char *name);
    symbol_info **block = reinterpret_cast<symbol_info **>(new symbol_info[TABLE_SIZE]); // Table
};

#endif //COMPILADOR_2019_3_SYMBOLTABLE_H
