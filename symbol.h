#ifndef COMPILADOR_2019_3_SYMBOL_H
#define COMPILADOR_2019_3_SYMBOL_H

#include "ast.h"
#include "symboltable.h"

class Symbol
{
private:
    const char *lexeme;
    const char *lexemeScope;
    int scope;
    Symbol *nextSymbol;
public:
    Symbol(const char *lexeme, int scope, const char *lexemeScope);
    virtual ~Symbol();

    bool isScope(const char *lexemeScope) { return this->lexemeScope==lexemeScope; }

    inline const char *getLexeme() { return this->lexeme; }
    inline Symbol *getNextSymbol() { return this->nextSymbol; }
    inline const char *getLexemeScope() const { return lexemeScope; }
    inline int getScope() const { return scope; }

    inline void setNextSymbol(Symbol *nextSymbol) { this->nextSymbol = nextSymbol; }
    inline void setLexeme(const char *lexeme) { this->lexeme = lexeme; }
    inline void setLexemeScope(const char *LexemeScope) { this->lexemeScope = LexemeScope; }
    inline void setScope(int scope) { this->scope = scope; }
};

class ReservedTokenSymbol : public Symbol
{
private:
    int tokenID;
public:
    ReservedTokenSymbol(const char *lexeme, int tokenID);
    inline int getTokenID() { return this->tokenID; }
    inline void setTokenID(int tokenID) { this->tokenID = tokenID; }

};

class IdSymbol : public Symbol
{
public:
    explicit IdSymbol(const char *lexeme) : Symbol(lexeme, nullptr, nullptr) {};
};

class LiteralSymbol : public Symbol
{
public:
    explicit LiteralSymbol(const char *lexeme) : Symbol(lexeme, nullptr, nullptr) {};
};

class NumIntSymbol : public Symbol
{
public:
    explicit NumIntSymbol(const char *lexeme) : Symbol(lexeme, nullptr, nullptr) {};
};

class NumFloatSymbol : public Symbol
{
public:
    explicit NumFloatSymbol(const char *lexeme) : Symbol(lexeme, nullptr, nullptr) {};
};

class VarSymbol : public Symbol
{
private:
    TypeNode *type;
    bool pointer;
    bool parameter;
    int arraySize;
    int offset;
    int size;
public:
    VarSymbol(const char *lexeme, int scope, const char *lexemeScope, TypeNode *type, bool pointer, int arraySize,  bool parameter);
    ~VarSymbol() override;

    inline bool isPointer() { return pointer; }
    inline bool isParameter() { return parameter; }
    inline TypeNode *getType() { return type; }
    inline int getArraySize() { return arraySize; }
    inline int getOffset() { return offset; }
    inline int getSize() { return size; }


    inline void setOffset(int offset) { this->offset = offset; }
    inline void setSize(int size) { this->size = size; }
    inline void setType(TypeNode *type) { this->type = type; }
    inline void setPointer(bool pointer) { this->pointer = pointer; }
    inline void setParameter(bool parameter) { this->parameter = parameter; }
    inline void setArraySize(int arraySize) { this->arraySize = arraySize; }
};

class FunctionSymbol : public Symbol
{
private:
    TypeNode *returnType;
    FormalListNode *varDecl;
    bool pointer;
    int localSize;
    int paramSize;
    int callSize;
public:
    FunctionSymbol(const char *lexeme, int scope, const char *lexemeScope, TypeNode *returnType, bool pointer, FormalListNode *varDecl);
    ~FunctionSymbol() override;

    inline void incrementLocalSize(int size) { this->localSize += size; }
    inline void incrementParamSize(int size) { this->paramSize += size; }

    inline bool isPointer() { return pointer; }
    inline TypeNode *getReturnType() { return this->returnType; }
    inline FormalListNode *getVarDecl() { return this->varDecl; }
    inline int getLocalSize() { return this->localSize; }
    inline int getParamSize() { return this->paramSize; }
    inline int getCallSize() { return this->callSize; }

    inline void setReturnType(TypeNode *returnType) { this->returnType = returnType; }
    inline void setVarDecl(FormalListNode *varDecl) { this->varDecl = varDecl; }
    inline void setPointer(bool pointer) { this->pointer = pointer; }
    inline void setLocalSize(int localSize) { this->localSize = localSize; }
    inline void setParamSize(int paramSize) { this->paramSize = paramSize; }
    inline void setCallSize(int callSize) { this->callSize = callSize; }
};

class StructSymbol : public Symbol
{
private:
    VarDeclNode *varDecl;
    int size;
public:
    StructSymbol(const char *lexeme, int scope, const char *lexemeScope, VarDeclNode *varDecl);
    ~StructSymbol() override;

    inline VarDeclNode *getVarDecl() { return this->varDecl; }
    inline int getSize() { return this->size; }

    inline void setVarDecl(VarDeclNode *varDecl) { this->varDecl = varDecl; }
    inline void setSize(int size) { this->size = size; }
};

#endif //COMPILADOR_2019_3_SYMBOL_H
