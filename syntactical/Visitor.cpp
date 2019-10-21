//
// Created by jorao on 17/10/2019.
//

#include <iostream>
#include "Visitor.h"

PrintAST::PrintAST() {
    level = 0;
}

void PrintAST::printAST(const char *node_name) {
    for (unsigned int i = 0; i < this->level; i++) std::cout << "\t";
    std::cout << "->" << node_name << std::endl;
}

void PrintAST::up_level() {
    this->level ++;

}

void PrintAST::down_level() {
    this->level --;

}

/*
 * PrintAST functions
 */

void PrintAST::visit(ProgramNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(ClassListNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(VarDeclNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(IdListNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(TypeNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(FunctionDeclNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(TypeListNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(PointerNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(StatementListNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(VarStmtNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(VarFuncListNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(IfNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(WhileNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(SwitchNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(BreakNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(PrintlnNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(ReadNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(ReturnNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(CaseBlockNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(ThrowNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(ExpListNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(TryNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(IdentifierNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(IntegerNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(RealNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(LiteralNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(CharacterNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(PrimaryNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(FunctionCallNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(FieldAccessNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(PointerAccessNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(AddressValueNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(PointerValueNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(ArrayAccessNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(ArrayDeclNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(AssignNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(RelationalOpNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(AdditionOpNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(MultiplicationOpNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(BooleanOpNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(BitwiseOpNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(NewNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(NotNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(SignNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(TrueNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(FalseNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(ThisNode *node) {
    Visitor::visit(node);
}

void PrintAST::visit(FormalListNode *node) {
    Visitor::visit(node);
}



