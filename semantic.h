#ifndef COMPILADOR_2019_3_SEMANTIC_H
#define COMPILADOR_2019_3_SEMANTIC_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <typeinfo>

#include "visitor.h"
#include "ast.h"
#include "symboltable.h"
#include "analyzer.h"


#define numinteiro 1
#define real       2
#define literal    3
#define tipodef    4
#define caracter   5
#define boleano    6


void IniciaSemantico(ProgramNode *ast);
void endSemantic();

class Semantic:public Visitor
{
public:
    Semantic();
    virtual ~Semantic();
    virtual void Visite(ASTNode *a);
    virtual void Visite(TokenNode *a);
    virtual void Visite(ProgramNode *a);
    virtual void Visite(TypeNode *a);
    virtual void Visite(IdListNode *a);
    virtual void Visite(VarDeclNode *a);
    virtual void Visite(TypeDeclNode *a);
    virtual void Visite(FormalListNode *a);
    virtual void Visite(ExpListNode *a);
    virtual void Visite(CallNode *a);
    virtual void Visite(PrimaryNode *a);
    virtual void Visite(FunctionNode *a);
    virtual void Visite(StmtNode *a);
    virtual void Visite(StmtListNode *a);
    virtual void Visite(ReadLnNode *a);
    virtual void Visite(PrintNode *a);
    virtual void Visite(WhileNode *a);
    virtual void Visite(ReturnNode *a);
    virtual void Visite(TryNode *a);
    virtual void Visite(ThrowNode *a);
    virtual void Visite(BreakNode *a);
    virtual void Visite(IfNode *a);
    virtual void Visite(SwitchNode *a);
    virtual void Visite(CaseBlockNode *a);
    virtual void Visite(AdditionOPNode *a);
    virtual void Visite(AddressValNode *a);
    virtual void Visite(AssignNode *a);
    virtual void Visite(BooleanOPNode *a);
    virtual void Visite(NameExpNode *a);
    virtual void Visite(NotNode *a);
    virtual void Visite(PointerValNode *a);
    virtual void Visite(MultiplicationOPNode *a);
    virtual void Visite(SignNode *a);
    virtual void Visite(ArrayCallNode *a);
    virtual void Visite(FunctionListNode *a);
    virtual void Visite(PointerExpNode *a);
};




//#include "Visitor.h"
//#include "symboltable.h"
//
//#define FUNCTION 998
//#define VARIABLE 999
//
//void startSemantic(ProgramNode *program);
//void endSemantic();
//SymbolTable *getTable(int table);
//
//class FillTableVisitor : public Visitor {
//public:
//    FillTableVisitor(ProgramNode *program);
//    virtual ~FillTableVisitor() {}
//
//    virtual void visit(ProgramNode *node);
//    virtual void visit(VarDeclNode *node);
//    virtual void visit(IdListNode *node);
//    virtual void visit(TypeNode *node);
////    virtual void visit(FunctionDeclNode *node);
////    virtual void visit(TypeListNode *node);
//    virtual void visit(PointerNode *node);
//    virtual void visit(StmtListNode *node);
//    virtual void visit(VarStmtNode *node);
////    virtual void visit(VarFuncListNode *node);
//    virtual void visit(IfNode *node);
//    virtual void visit(WhileNode *node);
//    virtual void visit(SwitchNode *node);
//    virtual void visit(BreakNode *node);
//    virtual void visit(PrintNode *node);
//    virtual void visit(ReadLnNode *node);
//    virtual void visit(ReturnNode *node);
//    virtual void visit(CaseBlockNode *node);
//    virtual void visit(ThrowNode *node);
//    virtual void visit(ExpListNode *node);
//    virtual void visit(TryNode *node);
//    virtual void visit(TokenNode *node);
//    virtual void visit(PrimaryNode *node);
//    virtual void visit(CallNode *node);
////    virtual void visit(FieldAccessNode *node);
////    virtual void visit(PointerAccessNode *node);
////    virtual void visit(AddressValueNode *node);
////    virtual void visit(PointerValueNode *node);
////    virtual void visit(ArrayAccessNode *node);
////    virtual void visit(ArrayDeclNode *node);
//    virtual void visit(AssignNode *node);
////    virtual void visit(OpNode *node);
//    virtual void visit(AdditionOPNode *node);
////    virtual void visit(MultiplicationOpNode *node);
////    virtual void visit(BooleanOpNode *node);
////    virtual void visit(BitwiseOpNode *node);
//    virtual void visit(NotNode *node);
//    virtual void visit(SignNode *node);
////    virtual void visit(TrueNode *node);
////    virtual void visit(FalseNode *node);
////    virtual void visit(ThisNode *node);
//    virtual void visit(FormalListNode *node);
//};
//
//class SemanticAnalyzerVisitor : public Visitor {
//public:
//    SemanticAnalyzerVisitor(ProgramNode *program);
//
//    virtual ~SemanticAnalyzerVisitor() {}
//
//    virtual void visit(ProgramNode *node);
//    virtual void visit(VarDeclNode *node);
//    virtual void visit(IdListNode *node);
//    virtual void visit(TypeNode *node);
////    virtual void visit(FunctionDeclNode *node);
////    virtual void visit(TypeListNode *node);
//    virtual void visit(PointerNode *node);
//    virtual void visit(StmtListNode *node);
//    virtual void visit(VarStmtNode *node);
////    virtual void visit(VarFuncListNode *node);
//    virtual void visit(IfNode *node);
//    virtual void visit(WhileNode *node);
//    virtual void visit(SwitchNode *node);
//    virtual void visit(BreakNode *node);
//    virtual void visit(PrintNode *node);
//    virtual void visit(ReadLnNode *node);
//    virtual void visit(ReturnNode *node);
//    virtual void visit(CaseBlockNode *node);
//    virtual void visit(ThrowNode *node);
//    virtual void visit(ExpListNode *node);
//    virtual void visit(TryNode *node);
//    virtual void visit(PrimaryNode *node);
//    virtual void visit(CallNode *node);
////    virtual void visit(FieldAccessNode *node);
////    virtual void visit(PointerAccessNode *node);
////    virtual void visit(AddressValueNode *node);
////    virtual void visit(PointerValueNode *node);
////    virtual void visit(ArrayAccessNode *node);
////    virtual void visit(ArrayDeclNode *node);
//    virtual void visit(AssignNode *node);
////    virtual void visit(RelationalOpNode *node);
////    virtual void visit(AdditionOpNode *node);
////    virtual void visit(MultiplicationOpNode *node);
////    virtual void visit(BooleanOpNode *node);
////    virtual void visit(BitwiseOpNode *node);
////    virtual void visit(NewNode *node);
//    virtual void visit(NotNode *node);
//    virtual void visit(SignNode *node);
////    virtual void visit(TrueNode *node);
////    virtual void visit(FalseNode *node);
////    virtual void visit(ThisNode *node);
//    virtual void visit(FormalListNode *node);
//};

#endif //COMPILADOR_2019_3_SEMANTIC_H
