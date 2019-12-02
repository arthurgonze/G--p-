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

#define BYTE_SIZE   4
#define BOOL_TRUE   1
#define BOOL_FALSE  0

void startSemantic(ProgramNode *ast);
void endSemantic();

class SemanticTypes : public Visitor
{
public:
    SemanticTypes() = default;
    void visit(TokenNode *tokenNode) override;
    void visit(ProgramNode *programNode) override;
    void visit(TypeNode *typeNode) override {};
    void visit(IdListNode *idListNode) override {};
    void visit(VarDeclNode *varDeclNode) override {};
    void visit(TypeDeclNode *typeDeclNode) override {};
    void visit(FormalListNode *formalListNode) override {};
    void visit(ExpListNode *expListNode) override;
    void visit(CallNode *callNode);
    void visit(PrimaryNode *primaryNode) override;
    void visit(FunctionNode *functionNode) override;
    void visit(StmtNode *stmtNode) override;
    void visit(StmtListNode *stmtListNode) override;
    void visit(ReadLnNode *a) override {};
    void visit(PrintNode *a) override {};
    void visit(WhileNode *whileNode) override {};
    void visit(ReturnNode *returnNode) override {};
    void visit(TryNode *tryNode) override {};
    void visit(ThrowNode *throwNode) override {};
    void visit(BreakNode *breakNode) override {};
    void visit(IfNode *ifNode) override {};
    void visit(SwitchNode *switchNode) override {};
    void visit(CaseBlockNode *caseBlockNode) override {};
    void visit(AdditionOPNode *additionOPNode) override;
    void visit(AddressValNode *addressValNode) override;
    void visit(AssignNode *assignNode) override;
    void visit(BooleanOPNode *booleanOPNode) override {};
    void visit(NameExpNode *nameExpNode) override;
    void visit(NotNode *notNode) override {};
    void visit(PointerValNode *pointerValNode) override;
    void visit(MultiplicationOPNode *multiplicationOPNode) override {};
    void visit(SignNode *signNode) override {};
    void visit(ArrayCallNode *arrayCallNode) override;
    void visit(FunctionListNode *functionListNode) override;
    void visit(PointerExpNode *pointerExpNode) override {};
    void visit(PointerNode *node) override {}
    void visit(ArrayNode *node) override;
    void visit(VarStmtNode *node) override {}
};


class SemanticTables : public Visitor
{
    public:
        SemanticTables() = default;;

        void visit(TokenNode *tokenNode) override;
        void visit(ProgramNode *programNode) override;
        void visit(TypeNode *typeNode) override {};
        void visit(IdListNode *idListNode) override;
        void visit(VarDeclNode *varDeclNode) override;
        void visit(TypeDeclNode *typeDeclNode) override;
        void visit(FormalListNode *formalListNode) override;
        void visit(ExpListNode *expListNode) override {};
        void visit(CallNode *callNode) override {};
        void visit(PrimaryNode *primaryNode) override {};
        void visit(FunctionNode *functionNode) override;
        void visit(StmtNode *stmtNode) override {};
        void visit(StmtListNode *stmtListNode) override {};
        void visit(ReadLnNode *a) override {};
        void visit(PrintNode *a) override {};
        void visit(WhileNode *whileNode) override {};
        void visit(ReturnNode *returnNode) override {};
        void visit(TryNode *tryNode) override {};
        void visit(ThrowNode *throwNode) override {};
        void visit(BreakNode *breakNode) override {};
        void visit(IfNode *ifNode) override {};
        void visit(SwitchNode *switchNode) override {};
        void visit(CaseBlockNode *caseBlockNode) override {};
        void visit(AdditionOPNode *additionOPNode) override {};
        void visit(AddressValNode *addressValNode) override {};
        void visit(AssignNode *assignNode) override {};
        void visit(BooleanOPNode *booleanOPNode) override {};
        void visit(NameExpNode *nameExpNode) override {};
        void visit(NotNode *notNode) override {};
        void visit(PointerValNode *pointerValNode) override {};
        void visit(MultiplicationOPNode *multiplicationOPNode) override {};
        void visit(SignNode *signNode) override {};
        void visit(ArrayCallNode *arrayCallNode) override {};
        void visit(FunctionListNode *functionListNode) override;
        void visit(PointerExpNode *pointerExpNode) override {};
        void visit(PointerNode *node) override {}
        void visit(ArrayNode *node) override {}
        void visit(VarStmtNode *node) override {}
};
class Semantic : public Visitor
{
public:
    Semantic();

    void visit(TokenNode *tokenNode) override;
    void visit(ProgramNode *programNode) override;
    void visit(TypeNode *typeNode) override {};
    void visit(IdListNode *idListNode) override;
    void visit(VarDeclNode *varDeclNode) override;
    void visit(TypeDeclNode *typeDeclNode) override;
    void visit(FormalListNode *formalListNode) override;
    void visit(ExpListNode *expListNode) override;
    void visit(CallNode *callNode) override;
    void visit(PrimaryNode *primaryNode) override;
    void visit(FunctionNode *functionNode) override;
    void visit(StmtNode *stmtNode) override;
    void visit(StmtListNode *stmtListNode) override;
    void visit(ReadLnNode *a) override;
    void visit(PrintNode *a) override;
    void visit(WhileNode *whileNode) override;
    void visit(ReturnNode *returnNode) override;
    void visit(TryNode *tryNode) override;
    void visit(ThrowNode *throwNode) override {};
    void visit(BreakNode *breakNode) override;
    void visit(IfNode *ifNode) override;
    void visit(SwitchNode *switchNode) override;
    void visit(CaseBlockNode *caseBlockNode) override;
    void visit(AdditionOPNode *additionOPNode) override;
    void visit(AddressValNode *addressValNode) override;
    void visit(AssignNode *assignNode) override;
    void visit(BooleanOPNode *booleanOPNode) override;
    void visit(NameExpNode *nameExpNode) override;
    void visit(NotNode *notNode) override;
    void visit(PointerValNode *pointerValNode) override;
    void visit(MultiplicationOPNode *multiplicationOPNode) override;
    void visit(SignNode *signNode) override;
    void visit(ArrayCallNode *arrayCallNode) override;
    void visit(FunctionListNode *functionListNode) override;
    void visit(PointerExpNode *pointerExpNode) override;
    void visit(PointerNode *node) override {}
    void visit(ArrayNode *node) override {}
    void visit(VarStmtNode *node) override {}
};
#endif //COMPILADOR_2019_3_SEMANTIC_H
