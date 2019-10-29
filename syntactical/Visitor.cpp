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
    this->printAST("PROGRAM","aux");//override; //TODO ARRUMAR O CHAR AUX
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(VarListNode *node) {
    this->printAST("VAR_LIST","c");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(FunctionListNode *node) {
    this->printAST("FUNCTION_LIST", "aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(IdListNode *node) {
    this->printAST("ID_LIST", "aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(TypeListNode *node) {
    this->printAST("TYPE_LIST","aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}


void PrintAST::visit(StmtListNode *node) {
    this->printAST("STATEMENT_LIST","aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();;
}

void PrintAST::visit(VarStmtNode *node) {
    this->printAST("VAR_STMT","aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(VarFuncListNode *node) {
    this->printAST("VAR_FUNC_LIST","aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(IfNode *node) {
    this->printAST("IF","aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(WhileNode *node) {
    this->printAST("WHILE","aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(SwitchNode *node) {
    this->printAST("SWITCH","aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(PrintLnNode *node) {
    this->printAST("PRINT_LN","aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(ReadNode *node) {
    this->printAST("READ","aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(ReturnNode *node) {
    this->printAST("RETURN","aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(CaseBlockNode *node) {
    this->printAST("CASE_BLOCK","aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(ExpListNode *node) {
    this->printAST("EXP_LIST","aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(TryNode *node) {
    this->printAST("TRY","aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}


void PrintAST::visit(PrimaryNode *node) {
    this->printAST("PRIMARY","aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(CallNode *node) {
    this->printAST("FUNCTIONAL_CALL","aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}


void PrintAST::visit(AdressValueNode *node) {
    this->printAST("ADDRESS_VALUE","aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(PointerValueNode *node) {
    this->printAST("POINTER_VALUE","aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(PointerValueExpNode *node) {
    this->printAST("POINTER_VALUE_EXP","aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(ArrayAccessNode *node) {
    this->printAST("ARRAY_ACESS","aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(ArrayNode *node) {
    this->printAST("ARRAY_DECL","aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(AssignNode *node) {
    this->printAST("ASSIGN","aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(RelationalOPNode *node) {
    this->printAST("TRY","aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(AdditionOPNode *node) {
    /*
     * PEGAR O NOME DO OPERADOR
     */
    this->printAST(token_id_to_name(node->getOp()->getToken()),"aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(MultiplicationOPNode *node) {
    /*
     * PEGAR O NOME DO OPERADOR
     */
    this->printAST(token_id_to_name(node->getOp()->getToken()),"aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(BooleanOPNode *node) {
    /*
     * PEGAR O NOME DO OPERADOR
     */
    this->printAST(token_id_to_name(node->getOp()->getToken()),"aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(BitwiseOPNode *node) {
    /*
     * PEGAR O NOME DO OPERADOR
     */
    this->printAST(token_id_to_name(node->getOp()->getToken()),"aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}


void PrintAST::visit(NotNode *node) {

    this->printAST("NOT","aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(SignNode *node) {

    this->printAST("SIGN","aux");
    up_level();
    /*
     * Fazer a busca na AST
     */
    down_level();
}

void PrintAST::visit(FormalListNode *node) {
    this->printAST("FORMAL_LIST","aux");
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
    this->printAST("POINTER","aux");
}


void PrintAST::visit(BreakNode *node) {
    this->printAST("BREAK","aux");
}


void PrintAST::visit(TrueNode *node) {

    this->printAST("TRUE","aux");
}

void PrintAST::visit(FalseNode *node) {
    this->printAST("FALSE","aux");
}

void PrintAST::visit(TypeNode *node) {
    if (node->getId()->getLex() != NULL) {
        this->printAST("TOKEN", "LEXEME");
    } else this->printAST("TOKEN","aux");

}

void PrintAST::visit(TokenNode *node) {
    this->printAST("ID.", "LEXEME");

}

void PrintAST::visit(ThrowNode *node) {
    this->printAST("THROW","aux");
}

