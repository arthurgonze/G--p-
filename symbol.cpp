#include "symbol.h"

Symbol::Symbol(int lexemeIndex, int scope, const char *lexemeScope) {
    this->lexemeIndex = lexemeIndex;
    this->lexemeScope = lexemeScope;
    this->scope = scope;
    this->nextSymbol = nullptr;
}

Symbol::~Symbol() {
    free(this->nextSymbol);
}

Symbol::Symbol(int lexemeIndex) {
    this->lexemeIndex = lexemeIndex;
    this->lexemeScope = nullptr;
    this->scope = -1;
    this->nextSymbol = nullptr;
}

Symbol::Symbol() {
    this->lexemeIndex = 0;
    this->lexemeScope = nullptr;
    this->scope = -1;
    this->nextSymbol = nullptr;
}

ReservedTokenSymbol::ReservedTokenSymbol(int lexemeIndex, int tokenID) : Symbol(lexemeIndex) {
    this->tokenID = tokenID;
}

VarSymbol::VarSymbol(int lexemeIndex, int scope, const char *lexemeScope, TypeNode *type,
                     bool pointer, int arraySize, bool parameter) : Symbol(lexemeIndex, scope, lexemeScope) {
    this->type = type;
    this->pointer = pointer;
    this->arraySize = arraySize;
    this->parameter = parameter;

    this->offset = 0;
    this->size = 0;
}

VarSymbol::~VarSymbol() {
    this->type = nullptr;
}

FunctionSymbol::FunctionSymbol(int lexemeIndex, int scope, const char *lexemeScope, TypeNode *returnType,
                               bool pointer, FormalListNode *varDecl) : Symbol(lexemeIndex, scope, lexemeScope) {
    this->returnType = returnType;
    this->pointer = pointer;
    this->varDecl = varDecl;

    this->localSize = 0;
    this->paramSize = 0;
    this->callSize = 0;
}

FunctionSymbol::~FunctionSymbol() {
    this->returnType = nullptr;
    this->varDecl = nullptr;
}

StructSymbol::StructSymbol(int lexemeIndex, int scope, const char *lexemeScope, VarDeclNode *varDecl)
        : Symbol(lexemeIndex, scope, lexemeScope) {
    this->varDecl = varDecl;
}

StructSymbol::~StructSymbol() {
    this->varDecl = nullptr;
}

StructSymbol::StructSymbol(int scope, const char *lexemeScope, VarDeclNode *varDecl) : Symbol(0, scope, lexemeScope) {
    this->varDecl = varDecl;
}
