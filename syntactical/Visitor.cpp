//
// Created by jorao on 17/10/2019.
//

#include "Visitor.h"

PrintAST::PrintAST()
{
    level = 0;
}

void PrintAST::visit(ProgramNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(VarListNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(NameDeclNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(TypeNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(FunctionListNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(TypeListNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(PointerNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(StmtListNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(IfNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(WhileNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(SwitchNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(BreakNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(PrintLnNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(ReadNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(ReturnNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(CaseBlockNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(ThrowNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(ExpListNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(TryNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(TokenNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(PointerValueNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(AssignNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(NameExpNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(PointerValueExpNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(AdressValueNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(ArrayNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(CallNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(RelationalOPNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(AdditionOPNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(MultiplicationOPNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(BooleanOPNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(BitwiseOPNode *node)
{
    Visitor::visit(node);
}


void PrintAST::visit(NotNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(SignNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(TrueNode *node)
{
    Visitor::visit(node);
}

void PrintAST::visit(FalseNode *node)
{
    Visitor::visit(node);
}
