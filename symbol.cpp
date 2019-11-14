#include "symbol.h"

Symbol::Symbol(const char *lexeme, int scope, const char *lexemeScope)
{
    this->lexeme = lexeme;
    this->lexemeScope = lexemeScope;
    this->scope = scope;
    this->nextSymbol = nullptr;
}
Symbol::~Symbol()
{
    this->lexeme = nullptr;
    free(this->nextSymbol);
}

ReservedTokenSymbol::ReservedTokenSymbol(const char *lexeme, int tokenID): Symbol(lexeme, nullptr, nullptr)
{
    this->tokenID = tokenID;
}

VarSymbol::VarSymbol(const char *lexeme, int scope, const char *lexemeScope, TypeNode *type, bool pointer, int arraySize,  bool parameter) : Symbol(lexeme, scope, lexemeScope)
{
    this->type = type;
    this->pointer = pointer;
    this->arraySize = arraySize;
    this->parameter = parameter;

    this->offset = 0;
    this->size = 0;
}
VarSymbol::~VarSymbol()
{
    this->type = nullptr;
}

FunctionSymbol::FunctionSymbol(const char *lexeme, int scope, const char *lexemeScope, TypeNode *returnType, bool pointer, FormalListNode *varDecl) : Symbol(lexeme, scope, lexemeScope)
{
    this->returnType = returnType;
    this->pointer = pointer;
    this->varDecl = varDecl;

    this->localSize = 0;
    this->paramSize = 0;
    this->callSize = 0;
}
FunctionSymbol::~FunctionSymbol()
{
    this->returnType = nullptr;
    this->varDecl = nullptr;
}

StructSymbol::StructSymbol(const char *lexeme, int scope, const char *lexemeScope, VarDeclNode *varDecl) : Symbol(lexeme, scope, lexemeScope)
{
    this->varDecl = varDecl;
}
StructSymbol::~StructSymbol()
{
    this->varDecl = nullptr;
}
