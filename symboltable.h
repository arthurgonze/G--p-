#ifndef COMPILADOR_2019_3_SYMBOLTABLE_H
#define COMPILADOR_2019_3_SYMBOLTABLE_H

#include <cstring>
#include <string>
#include <iostream>
#include "symbol.h"

using namespace std;

#define TABLE_SIZE 101 // An arbitrary prime number that showed good performance
#define LEXEME_ARRAY_SIZE 4096 // A memory block size

class Symbol;
class StructSymbol;
class FunctionSymbol;
class VarSymbol;
class ReservedTokenSymbol;

class SymbolTable
{
protected:
    int headIndex = 0; // A variable to store the first free position in the lexeme array
    int lexemeArraySize = 0; // Actual size of the lexeme array

    int currentScope;// new
    const char *previousScopeLexeme;// new
    const char *currentScopeLexeme;// new

    static unsigned long cHash(string const &name); // Hash Function
    //symbol_info *auxInsert(symbol_info *root, int token, const char *lexeme);
    Symbol *auxInsert(Symbol *root, int token, const char *lexeme);

    virtual void print();
public:
    SymbolTable();
    virtual ~SymbolTable() = default;

    //symbol_info **block; // Table
    Symbol **block; // Table
    char *lexemeArray = new char[LEXEME_ARRAY_SIZE]; // An array to allocate lexeme in continuous memory


    void beginScope(const char *lexemeScope);// new
    void endScope();// new

    //void cInsert(int token, const char *lexeme);
    void cInsert(Symbol *symbol, const char *lexeme); // new

    //int cSearch(char *lexeme);// TODO não é const char *?
    Symbol *cSearch(const char *lexeme);// new
};

//Extend Symbol Table to create the Reserved words table, Literals table and Identifiers table
class ReservedWordsTable : public SymbolTable
{
public:
    void cInsert(int tokenID, const char *lexeme);
    void print() override;// TODO especializar o print?
    // TODO metodo de busca proprio?
    ReservedTokenSymbol *cSearch(const char *lexeme) { return (ReservedTokenSymbol *) SymbolTable::cSearch(lexeme); }
};
class LiteralsTable : public SymbolTable
{
public:
    void cInsert(const char *lexeme);
    void print() override;
};
class IdsTable : public SymbolTable
{
public:
    void cInsert(const char *lexeme);
    void print() override;
};
class NumIntTable : public SymbolTable
{
public:
    void cInsert(const char *lexeme);
    void print() override;
};
class NumFloatTable : public SymbolTable
{
public:
    void cInsert(const char *lexeme);
    void print() override;
};

class StructTable : public SymbolTable // TODO
{
public:
    StructTable();
    ~StructTable() override;

    bool insert(const char *lexeme, VarDeclNode *varDecl);

    StructSymbol *search(const char *lexeme);

    void print() override;
};

class FunctionTable : public SymbolTable // TODO
{
public:
    FunctionTable();
    ~FunctionTable() override;

    bool insert(TypeNode *returnType, const char *lexeme, FormalListNode *varDecl, bool pointer);

    FunctionSymbol *search(const char *lexeme);

    FunctionSymbol *searchInScope(const char *lexeme, const char *lexemeScope);

    void print() override;
};

class VarTable : public SymbolTable // TODO
{
public:
    VarTable();
    ~VarTable() override;

    bool insert(TypeNode *type, const char *lexeme, bool pointer, int arraySize, bool parameter);

    VarSymbol *search(const char *lexeme);
    VarSymbol *searchInScope(const char *lexeme, const char *LexemeScope);

    void print() override;
};

#endif //COMPILADOR_2019_3_SYMBOLTABLE_H
