#ifndef COMPILADOR_2019_3_SYMBOLTABLE_H
#define COMPILADOR_2019_3_SYMBOLTABLE_H

#include<bits/stdc++.h>
using namespace std;

#define TABLE_SIZE 101 // An arbitrary prime number that showed good performance
#define LEXEME_ARRAY_SIZE 4096 // A memory block size

// Two columns of the symbol table with token ID and lexeme position in the lexeme array
struct symbol_info
{
    int token; //token id
    int pos; // lexem position in the lexemeArray
    struct symbol_info *next; // pointer to the next symbol
};

class SymbolTable
{
public:
    void cInsert(int token, char *lexeme);
    int cSearch(char *lexeme);
    symbol_info **block = reinterpret_cast<symbol_info **>(new symbol_info[TABLE_SIZE]); // Table
    char *lexemeArray = new char[LEXEME_ARRAY_SIZE]; // An array to allocate lexeme in continuous memory
    SymbolTable(); // Constructor
private:
    int headIndex = 0; // A variable to store the first free position in the lexeme array
    int lexemeArraySize = 0; // Actual size of the lexeme array
    long long cHash(string const& name); // Hash Function
};

#endif //COMPILADOR_2019_3_SYMBOLTABLE_H
