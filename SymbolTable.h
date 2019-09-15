#ifndef COMPILADOR_2019_3_SYMBOLTABLE_H
#define COMPILADOR_2019_3_SYMBOLTABLE_H

#include <cstdlib>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <tic.h>

using namespace std;
struct newToken
{
    char *tokenName; // ID, NUM, LITERAL , ETC
    char *attributeValue; // POINTER TO SYMBOL TABLE IF NEEDED
    bool operator==(const newToken &rhs) const { return tokenName==rhs.tokenName; }
    bool operator!=(const newToken &rhs) const { return !(*this==rhs); }
};

class SymbolTable
{
public:
    explicit SymbolTable(int size = 101);

    bool contains(const newToken &x) const;
    bool insert(const char* tokenName, const char* lexeme);

private:
    vector<list<newToken>> theLists; // The array of lists
    int tableSize;
    size_t hash(const string tokenName) const;
};

#endif //COMPILADOR_2019_3_SYMBOLTABLE_H
