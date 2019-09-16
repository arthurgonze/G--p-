#ifndef COMPILADOR_2019_3_SYMBOLTABLE_H
#define COMPILADOR_2019_3_SYMBOLTABLE_H

#include<bits/stdc++.h>
#pragma pack(1)
using namespace std;

// It has been seen if chain length is < 50 then some space is underutilized,
// Theoretically seen if > 50, prime number is taken then chain takes moderate space.
// So thats why 53 is used.
#define CHAIN_LENGTH 101
#define M 128

class SymbolTable
{
    // Two columns of the symbol table with name and class type.
    struct symbol_info
    {
        char *name;
        char *classtype;
        struct symbol_info *next;
    };

    struct node
    {
        struct node *next;
    };

public:
    void cInsert(char *name, char *classtype);
    bool cSearch(char *name, char *classtype);
    void showSymbolTable();
private:
    char* lexemeArray;

    int cHash(char *name);
    symbol_info **block = reinterpret_cast<symbol_info **>(new symbol_info[CHAIN_LENGTH]);// ponteiro para ponteiro de symbol_infos

    symbol_info *blockptr = reinterpret_cast<symbol_info *>(new symbol_info[CHAIN_LENGTH*CHAIN_LENGTH]);//ponteiro para objeto symbol_info
//    symbol_info *block[CHAIN_LENGTH]; // array of array of symbols
};

#endif //COMPILADOR_2019_3_SYMBOLTABLE_H
