//
// Created by jorao on 17/10/2019.
//

#include <iostream>
#include "Visitor.h"
#include "AST.h"
#include "../lexical/analyzer.h"


/*
 * TO DO:
 *
 * MUDAR TOKEN -> token_id_name
 * MUDAR LEXEME para node->get_lexeme
 *
 * AJUSTAR CADA NO PARA FAZER A FUNCAO
 */

PrintAST::PrintAST() {
    level = 0;
}

void PrintAST::printAST(const char *node_name, const char *aux = "") {
    for (unsigned int i = 0; i < this->level; i++) std::cout << "\t";
    std::cout << "->" << node_name << aux << std::endl;
}

void PrintAST::up_level() {
    this->level++;

}

void PrintAST::down_level() {
    this->level--;

}

/*
 * PrintAST functions
 */

void PrintAST::visit(ProgramNode *node) {
    std::cout << "\n------------------------------" << std::endl;
    std::cout << "---- ABSTRACT SYNTAX TREE ----" << std::endl;
    std::cout << "------------------------------\n" << std::endl;
    this->printAST("PROGRAM");override;
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(ClassListNode *node) {
    this->printAST("CLASS_LIST");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(VarDeclNode *node) {
    this->printAST("VAR_DECL");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(FunctionDeclNode *node) {
    this->printAST("FUNCTION_DECL");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(IdListNode *node) {
    this->printAST("ID_LIST");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(TypeListNode *node) {
    this->printAST("TYPE_LIST");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}


void PrintAST::visit(StatementListNode *node) {
    this->printAST("STATEMENT_LIST");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();;
}

void PrintAST::visit(VarStmtNode *node) {
    this->printAST("VAR_STMT");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(VarFuncListNode *node) {
    this->printAST("VAR_FUNC_LIST");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(IfNode *node) {
    this->printAST("IF");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(WhileNode *node) {
    this->printAST("WHILE");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(SwitchNode *node) {
    this->printAST("SWITCH");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(PrintlnNode *node) {
    this->printAST("PRINT_LN");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(ReadNode *node) {
    this->printAST("READ");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(ReturnNode *node) {
    this->printAST("RETURN");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(CaseBlockNode *node) {
    this->printAST("CASE_BLOCK");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(ExpListNode *node) {
    this->printAST("EXP_LIST");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(TryNode *node) {
    this->printAST("TRY");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}


void PrintAST::visit(PrimaryNode *node) {
    this->printAST("PRIMARY");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(FunctionCallNode *node) {
    this->printAST("FUNCTIONAL_CALL");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(FieldAccessNode *node) {
    this->printAST("FIELD_ACESS");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(PointerAccessNode *node) {
    this->printAST("POINTER_ACESS");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(AddressValueNode *node) {
    this->printAST("ADDRESS_VALUE");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(PointerValueNode *node) {
    this->printAST("POINTER_VALUE");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(ArrayAccessNode *node) {
    this->printAST("ARRAY_ACESS");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(ArrayDeclNode *node) {
    this->printAST("ARRAY_DECL");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(AssignNode *node) {
    this->printAST("ASSIGN");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(RelationalOpNode *node) {
    this->printAST("TRY");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(AdditionOpNode *node) {
    /*
     * PEGAR O NOME DO OPERADOR
     */
    this->printAST(token_id_to_name(node->op));
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(MultiplicationOpNode *node) {
    /*
     * PEGAR O NOME DO OPERADOR
     */
    this->printAST(token_id_to_name(node->op));
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(BooleanOpNode *node) {
    /*
     * PEGAR O NOME DO OPERADOR
     */
    this->printAST(token_id_to_name(node->op));
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(BitwiseOpNode *node) {
    /*
     * PEGAR O NOME DO OPERADOR
     */
    this->printAST(token_id_to_name(node->op));
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(NewNode *node) {

    this->printAST("NEW");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(NotNode *node) {

    this->printAST("NOT");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(SignNode *node) {

    this->printAST("SIGN");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(FormalListNode *node) {
    this->printAST("FORMAL_LIST");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

/*
 * TERMINAL NODES
 */

void PrintAST::visit(PointerNode *node) {
    this->printAST("POINTER");
}


void PrintAST::visit(BreakNode *node) {
    this->printAST("BREAK");
}


void PrintAST::visit(TrueNode *node) {

    this->printAST("TRUE");
}

void PrintAST::visit(FalseNode *node) {
    this->printAST("FALSE");
}

void PrintAST::visit(ThisNode *node) {
    this->printAST("THIS");
}

void PrintAST::visit(TypeNode *node) {
    if (node->get_lexeme() != NULL) {
        this->printAST("TOKEN", "LEXEME");
    } else this->printAST("TOKEN");

}

void PrintAST::visit(IdentifierNode *node) {
    this->printAST("ID.", "LEXEME");

}

void PrintAST::visit(IntegerNode *node) {
    this->printAST("INTEGER.", "LEXEME");
}

void PrintAST::visit(RealNode *node) {
    this->printAST("REAL.", "LEXEME");
}

void PrintAST::visit(LiteralNode *node) {
    this->printAST("LITERAL.", "LEXEME");
}

void PrintAST::visit(CharacterNode *node) {
    this->printAST("CHARACTER.", "LEXEME");
}

void PrintAST::visit(ThrowNode *node) {
    this->printAST("THROW");
}

