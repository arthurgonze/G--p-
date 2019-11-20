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
class LiteralSymbol;
class IdSymbol;
class NumIntSymbol;
class NumFloatSymbol;

class SymbolTable
{
protected:
    int headIndex = 0; // A variable to store the first free position in the lexeme array
    int lexemeArraySize = 0; // Actual size of the lexeme array

    int currentScope;// new
    const char *previousScopeLexeme;// new
    const char *currentScopeLexeme;// new

    static unsigned long cHash(string const &name); // Hash Function
    Symbol *auxInsert(Symbol *root, const char *lexeme);

    virtual void print();
public:
    SymbolTable();
    virtual ~SymbolTable() = default;

    Symbol **block; // Table
    char *lexemeArray = new char[LEXEME_ARRAY_SIZE]; // An array to allocate lexeme in continuous memory


    void beginScope(const char *lexemeScope);// new
    void endScope();// new

    void cInsert(Symbol *symbol, const char *lexeme); // new

    Symbol *cSearch(const char *lexeme);// new
};

//Extend Symbol Table to create the Reserved words table, Literals table and Identifiers table
class ReservedWordsTable : public SymbolTable
{
public:
    void cInsert(int tokenID, const char *lexeme);
    void print() override;
    ReservedTokenSymbol *cSearch(const char *lexeme) { return (ReservedTokenSymbol *) SymbolTable::cSearch(lexeme); }
};
class LiteralsTable : public SymbolTable
{
public:
    void cInsert(const char *lexeme);
    void print() override;

    LiteralSymbol *cSearch(const char *lexeme) { return (LiteralSymbol *) SymbolTable::cSearch(lexeme); }
};
class IdsTable : public SymbolTable
{
public:
    void cInsert(const char *lexeme);
    void print() override;
    IdSymbol *cSearch(const char *lexeme) { return (IdSymbol *) SymbolTable::cSearch(lexeme); }
};
class NumIntTable : public SymbolTable
{
public:
    void cInsert(const char *lexeme);
    void print() override;
    NumIntSymbol *cSearch(const char *lexeme) { return (NumIntSymbol *) SymbolTable::cSearch(lexeme); }
};
class NumFloatTable : public SymbolTable
{
public:
    void cInsert(const char *lexeme);
    void print() override;
    NumFloatSymbol *cSearch(const char *lexeme) { return (NumFloatSymbol *) SymbolTable::cSearch(lexeme); }
};

class StructTable : public SymbolTable
{
public:
    StructTable();
    ~StructTable() override;

    bool cInsert(const char *lexeme, VarDeclNode *varDecl);

    StructSymbol *cSearch(const char *lexeme);

    void print() override;
};

class FunctionTable : public SymbolTable
{
public:
    FunctionTable();
    ~FunctionTable() override;

    bool cInsert(TypeNode *returnType, const char *lexeme, FormalListNode *varDecl, bool pointer);

    FunctionSymbol *cSearch(const char *lexeme);

    FunctionSymbol *searchInScope(const char *lexeme, const char *lexemeScope);

    void print() override;
};

class VarTable : public SymbolTable
{
public:
    VarTable();
    ~VarTable() override;

    bool cInsert(TypeNode *type, const char *lexeme, bool pointer, int arraySize, bool parameter);

    VarSymbol *cSearch(const char *lexeme);
    VarSymbol *searchInScope(const char *lexeme, const char *LexemeScope);

    void print() override;
};

#endif //COMPILADOR_2019_3_SYMBOLTABLE_H
