#ifndef COMPILADOR_2019_3_SYMBOLTABLE_H
#define COMPILADOR_2019_3_SYMBOLTABLE_H

#include <cstring>
#include <string>
#include "symbol.h"

using namespace std;

#define TABLE_SIZE 101 // An arbitrary prime number that showed good performance
#define LEXEME_ARRAY_SIZE 4096 // A memory block size

// Two columns of the symbol table with token ID and lexeme position in the lexeme array
struct symbol_info
{
    int token; //token id
    int pos; // lexem position in the lexemeArray
    struct symbol_info *next; // pointer to the next symbol

    ~symbol_info();
};


class Symbol;
class StructSymbol;
class FunctionSymbol;
class VarSymbol;

class SymbolTable
{
protected:
    int headIndex = 0; // A variable to store the first free position in the lexeme array
    int lexemeArraySize = 0; // Actual size of the lexeme array

    int currentScope;// new
    const char *previousScopeLexeme;// new
    const char *currentScopeLexeme;// new

    static unsigned long cHash(string const &name); // Hash Function

    void insert(Symbol *symbol, const char *lexeme); // new

    Symbol *search(const char *lexeme);// new

    void print();
public:
    SymbolTable();
    virtual ~SymbolTable();

    symbol_info **block; // Table
    Symbol **newBlock;// TODO agora symbol_info == Symbol
    char *lexemeArray = new char[LEXEME_ARRAY_SIZE]; // An array to allocate lexeme in continuous memory


    void beginScope(const char *lexemeScope);// new
    void endScope();// new

    void cInsert(int token, const char *lexeme);
    symbol_info *auxInsert(symbol_info *root, int token, const char *lexeme);

    int cSearch(char *lexeme);// TODO não é const char *?
};

//Extend Symbol Table to create the Reserved words table, Literals table and Identifiers table
class ReservedWordsTable : public SymbolTable
{
public:
    void print();// TODO especializar o print?
    // TODO metodo de busca proprio?
    // ex.: ReservedTokenSymbol *search(const char *lexeme) { return (ReservedTokenSymbol*) SymbolTable::search(lexeme); }
};
class LiteralsTable : public SymbolTable
{
public:
    void cInsert(const char *lexeme);
    void print();
};
class IdsTable : public SymbolTable
{
public:
    void cInsert(const char *lexeme);
    void print();
};
class NumIntTable : public SymbolTable
{
public:
    void cInsert(const char *lexeme);
    void print();
};
class NumFloatTable : public SymbolTable
{
public:
    void cInsert(const char *lexeme);
    void print();
};

class StructTable : public SymbolTable // TODO
{
public:
    StructTable();
    virtual ~StructTable();

    bool insert(const char *lexeme, VarDeclNode *varDecl);

    StructSymbol *search(const char *lexeme);

    void print();
};

class FunctionTable : public SymbolTable // TODO
{
public:
    FunctionTable();
    virtual ~FunctionTable();

    bool insert(TypeNode *type, const char *lexeme, FormalListNode *varDecl, bool pointer);

    FunctionSymbol *search(const char *lexeme);

    FunctionSymbol *searchInScope(const char *lexeme, const char *lexemeScope);

    void print();
};

class VarTable : public SymbolTable // TODO
{
public:
    VarTable();
    virtual ~VarTable();

    bool insert(TypeNode *type, const char *lexeme, bool pointer, int array, bool parameter);

    VarSymbol *search(const char *lexeme);
    VarSymbol *searchInScope(const char *lexeme, const char *LexemeScope);

    void print();
};

#endif //COMPILADOR_2019_3_SYMBOLTABLE_H
