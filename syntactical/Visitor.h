//
// Created by jorao on 17/10/2019.
//

#ifndef COMPILADOR_2019_3_VISITOR_H
#define COMPILADOR_2019_3_VISITOR_H

#ifndef VISITOR_H_INCLUDED
#define VISITOR_H_INCLUDED

#include "AST.h"

class Visitor
{
public:
    virtual ~Visitor() = default;;
    virtual void visit(ProgramNode *node) {};
    virtual void visit(VarListNode *node) {};
    virtual void visit(NameDeclNode *node) {};
    virtual void visit(TypeNode *node) {};
    virtual void visit(FunctionListNode *node) {};
    virtual void visit(TypeListNode *node) {};
    virtual void visit(PointerNode *node) {};
    virtual void visit(StmtListNode *node) {};
    virtual void visit(IfNode *node) {};
    virtual void visit(WhileNode *node) {};
    virtual void visit(SwitchNode *node) {};
    virtual void visit(BreakNode *node) {};
    virtual void visit(PrintLnNode *node) {};
    virtual void visit(ReadNode *node) {};
    virtual void visit(ReturnNode *node) {};
    virtual void visit(CaseBlockNode *node) {};
    virtual void visit(ThrowNode *node) {};
    virtual void visit(ExpListNode *node) {};
    virtual void visit(TryNode *node) {};
    virtual void visit(TokenNode *node) {};
    virtual void visit(PointerValueNode *node) {};
    virtual void visit(AssignNode *node) {};
    virtual void visit(NameExpNode *node) {};
    virtual void visit(PointerValueExpNode *node) {};
    virtual void visit(AdressValueNode *node) {};
    virtual void visit(ArrayNode *node) {};
    virtual void visit(CallNode *node) {};
    virtual void visit(RelationalOPNode *node) {};
    virtual void visit(AdditionOPNode *node) {};
    virtual void visit(MultiplicationOPNode *node) {};
    virtual void visit(BooleanOPNode *node) {};
    virtual void visit(BitwiseOPNode *node) {};
    virtual void visit(NotNode *node) {};
    virtual void visit(SignNode *node) {};
    virtual void visit(TrueNode *node) {};
    virtual void visit(FalseNode *node) {};
};

class PrintAST : public Visitor
{
private:
    unsigned int level;

public:
    PrintAST();

    void visit(ProgramNode *node) override;
    void visit(VarListNode *node) override;
    void visit(NameDeclNode *node) override;
    void visit(TypeNode *node) override;
    void visit(FunctionListNode *node) override;
    void visit(TypeListNode *node) override;
    void visit(PointerNode *node) override;
    void visit(StmtListNode *node) override;
    void visit(IfNode *node) override;
    void visit(WhileNode *node) override;
    void visit(SwitchNode *node) override;
    void visit(BreakNode *node) override;
    void visit(PrintLnNode *node) override;
    void visit(ReadNode *node) override;
    void visit(ReturnNode *node) override;
    void visit(CaseBlockNode *node) override;
    void visit(ThrowNode *node) override;
    void visit(ExpListNode *node) override;
    void visit(TryNode *node) override;
    void visit(TokenNode *node) override;
    void visit(PointerValueNode *node) override;
    void visit(AssignNode *node) override;
    void visit(NameExpNode *node) override;
    void visit(PointerValueExpNode *node) override;
    void visit(AdressValueNode *node) override;
    void visit(ArrayNode *node) override;
    void visit(CallNode *node) override;
    void visit(RelationalOPNode *node) override;
    void visit(AdditionOPNode *node) override;
    void visit(MultiplicationOPNode *node) override;
    void visit(BooleanOPNode *node) override;
    void visit(BitwiseOPNode *node) override;
    void visit(NotNode *node) override;
    void visit(SignNode *node) override;
    void visit(TrueNode *node) override;
    void visit(FalseNode *node) override;
};

#endif //VISITOR_H_INCLUDED

#endif //COMPILADOR_2019_3_VISITOR_H
