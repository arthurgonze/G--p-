#ifndef COMPILADOR_2019_3_SYMBOLTABLE_H
#define COMPILADOR_2019_3_SYMBOLTABLE_H

#include<bits/stdc++.h>

using namespace std;

// It has been seen if chain length is < 50 then some space is underutilized,
// Theoretically seen if > 50, prime number is taken then chain takes moderate space.
// So thats why 53 is used.
#define CHAIN_LENGTH 53
#define M 128

class SymbolTable
{
    // Two columns of the symbol table with name and class type.
    struct symbol_info
    {
        char *name;
        char *classtype;
        struct symbol_info *next;
    } *block[CHAIN_LENGTH];

public:
    void cInsert(char *name, char *classtype);
    bool cSearch(char *name, char *classtype);
    void showSymbolTable();
private:
    char* lexemeArray;

    int cHash(char *name);
};

#endif //COMPILADOR_2019_3_SYMBOLTABLE_H
