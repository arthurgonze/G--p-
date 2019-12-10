
#include "visitor_ir.h"
#include "ict.h"
#include "analyzer.h"

#include <iostream>

PrintIR::PrintIR(ICTNode *node) {
    this->level = 0;
    std::cout << "\n------------------------------" << std::endl;
    std::cout << "---- INTERMEDIATE CANONICAL TREE ----" << std::endl;
    std::cout << "------------------------------\n" << std::endl;
    if (node != NULL) node->accept(this);
}

void PrintIR::printIR(const char *node_name) {
    for (unsigned int i = 0; i < this->level; i++)
        std::cout << "    ";
    std::cout << "->" << node_name << std::endl;
}

void PrintIR::printLexemeIR(const char *node_name, const char *aux) {
    for (unsigned int i = 0; i < this->level; i++)
        std::cout << "    ";
    std::cout << "->" << node_name << "." << aux << std::endl;
}

void PrintIR::up_level() {
    this->level++;

}
void PrintIR::down_level() {
    this->level--;

}

void PrintIR::visit(ICTNode *node) {
    printIR("ICTNode");
    if (node != NULL) node->accept(this);
}

void PrintIR::visit(ExprNode *node) {
    printIR("ExprNode");
    if (node != NULL) node->accept(this);
}

void PrintIR::visit(ExpList *node) {
    printIR("ExpList");
    up_level();
    if (node->getFirst() != NULL) node->getFirst()->accept(this);
    if (node->getNext() != NULL) node->getNext()->accept(this);
    down_level();
}

void PrintIR::visit(CONST *node) {
    for (unsigned int i = 0; i < this->level; i++)
        std::cout << "    ";
    std::cout << "->" << "CONST" << "." << node->getI() << std::endl;
}

void PrintIR::visit(CONSTF *node) {
    for (unsigned int i = 0; i < this->level; i++)
        std::cout << "    ";
    std::cout << "->" << "CONSTF" << "." << node->getJ() << std::endl;

}

void PrintIR::visit(NAME *node) {
    if (node->getL() != NULL) {
        printLexemeIR("NAME", node->getL()->getName());
    }
}

void PrintIR::visit(TEMP *node) {
    if (node->getT() != NULL) {
        printLexemeIR("TEMP", node->getT()->getName());
    }
}

void PrintIR::visit(BINOP *node) {
    printLexemeIR("BINOP", token_id_to_name(node->getBinop()));
    up_level();
    if (node->getLeft() != NULL) node->getLeft()->accept(this);
    if (node->getRight() != NULL) node->getRight()->accept(this);
    down_level();
}

void PrintIR::visit(MEM *node) {
    printIR("MEM");
    up_level();
    if (node->getE() != NULL) node->getE()->accept(this);
    down_level();
}

void PrintIR::visit(CALL *node) {
    printIR("CALL");
    up_level();
    if (node->getFunc() != NULL) node->getFunc()->accept(this);
    if (node->getArgs() != NULL) node->getArgs()->accept(this);
    down_level();
}

void PrintIR::visit(ESEQ *node) {
    printIR("ESEQ");
    up_level();
    if (node->getS() != NULL) node->getS()->accept(this);
    if (node->getE() != NULL) node->getE()->accept(this);
    down_level();
}

void PrintIR::visit(StmNode *node) {
    if (node != NULL) node->accept(this);
}

void PrintIR::visit(StmList *node) {
    printIR("StmtList");
    up_level();
    if (node->getFirst() != NULL) node->getFirst()->accept(this);
    if (node->getNext() != NULL) node->getNext()->accept(this);
    down_level();
}

void PrintIR::visit(MOVE *node) {
    printIR("MOVE");
    up_level();
    if (node->getDst() != NULL) node->getDst()->accept(this);
    if (node->getSrc() != NULL) node->getSrc()->accept(this);
    down_level();
}

void PrintIR::visit(EXP *node) {
    printIR("EXP");
    up_level();
    if (node->getE() != NULL) node->getE()->accept(this);
    down_level();
}

void PrintIR::visit(JUMP *node) {
    printIR("JUMP");
    up_level();
     // TODO ver se isso aqui ta certo pq o getTargets nao tem accpet por ser um LabelLIst
    if (node->getE() != NULL) node->getE()->accept(this);
    down_level();
}

void PrintIR::visit(CJUMP *node) {
    printLexemeIR("CJUMP", token_id_to_name(node->getRelop()));
    up_level();
    if (node->getLeft() != NULL) node->getLeft()->accept(this);
    if (node->getRight() != NULL) node->getRight()->accept(this);
    if (node->getIfTrue() != NULL) node->getIfTrue()->accept(this);
    if (node->getIfFalse() != NULL) node->getIfTrue()->accept(this);
    down_level();
}

void PrintIR::visit(SEQ *node) {
    printIR("SEQ");
    up_level();
    if (node->getLeft() != NULL) node->getLeft()->accept(this);
    if (node->getRight() != NULL) node->getRight()->accept(this);
    down_level();
}

void PrintIR::visit(LABEL *node) {
    if (node->getL() != NULL) {
        printLexemeIR("LABEL", node->getL()->getName());
    }
}

PrintIR::~PrintIR() = default;
