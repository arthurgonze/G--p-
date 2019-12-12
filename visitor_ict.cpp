#include "visitor_ict.h"
#include "lexycal.h"


PrintICT::PrintICT(ICTNode *node) {
    this->level = 0;
}

void PrintICT::printIR(const char *node_name) {
    for (unsigned int i = 0; i < this->level; i++)
        std::cout << "    ";
    std::cout << "->" << node_name << std::endl;
}

void PrintICT::printLexemeIR(const char *node_name, const char *aux) {
    for (unsigned int i = 0; i < this->level; i++)
        std::cout << "    ";
    std::cout << "->" << node_name << "." << aux << std::endl;
}

void PrintICT::up_level() {
    this->level++;
}

void PrintICT::down_level() {
    this->level--;

}

void PrintICT::visit(Procedure *node) {
    if (node != NULL) {
        node->accept(this);
    }
}
void PrintICT::visit(Literal *node) {

    printLexemeIR("LITERAL_",node->getLiteral());

    if(node->getNext()) node->getNext()->accept(this);
}

void PrintICT::visit(Variable *node) {

    printLexemeIR("VARIABLE_", token_id_to_name(node->getType()));

    if(node->getNext()) node->getNext()->accept(this);
}



void PrintICT::visit(ICTNode *node) {
    printIR("ICTNode");
    if (node != NULL) node->accept(this);
}

void PrintICT::visit(ExprNode *node) {
    printIR("ExprNode");
    if (node != NULL) node->accept(this);
}

void PrintICT::visit(ExpList *node) {
    printIR("ExpList");
    up_level();
    if (node->getFirst() != NULL) node->getFirst()->accept(this);
    if (node->getNext() != NULL) node->getNext()->accept(this);
    down_level();
}

void PrintICT::visit(CONST *node) {
    for (unsigned int i = 0; i < this->level; i++)
        std::cout << "    ";
    std::cout << "->" << "CONST" << "." << node->getI() << std::endl;
}

void PrintICT::visit(CONSTF *node) {
    for (unsigned int i = 0; i < this->level; i++)
        std::cout << "    ";
    std::cout << "->" << "CONSTF" << "." << node->getJ() << std::endl;

}

void PrintICT::visit(NAME *node) {
    if (node->getL() != NULL) {
        printLexemeIR("NAME", node->getL()->getName());
    }
}

void PrintICT::visit(TEMP *node) {
    if (node->getT() != NULL) {
        printLexemeIR("TEMP", node->getT()->getName());
    }
}

void PrintICT::visit(BINOP *node) {
    printLexemeIR("BINOP", token_id_to_name(node->getBinop()));
    up_level();
    if (node->getLeft() != NULL) node->getLeft()->accept(this);
    if (node->getRight() != NULL) node->getRight()->accept(this);
    down_level();
}

void PrintICT::visit(MEM *node) {
    printIR("MEM");
    up_level();
    if (node->getE() != NULL) node->getE()->accept(this);
    down_level();
}

void PrintICT::visit(CALL *node) {
    printIR("CALL");
    up_level();
    if (node->getFunc() != NULL) node->getFunc()->accept(this);
    if (node->getArgs() != NULL) node->getArgs()->accept(this);
    down_level();
}

void PrintICT::visit(ESEQ *node) {
    printIR("ESEQ");
    up_level();
    if (node->getS() != NULL) node->getS()->accept(this);
    if (node->getE() != NULL) node->getE()->accept(this);
    down_level();
}

void PrintICT::visit(StmNode *node) {
    if (node != NULL) node->accept(this);
}

void PrintICT::visit(StmList *node) {
    printIR("StmtList");
    up_level();
    if (node->getFirst() != NULL) node->getFirst()->accept(this);
    if (node->getNext() != NULL) node->getNext()->accept(this);
    down_level();
}

void PrintICT::visit(MOVE *node) {
    printIR("MOVE");
    up_level();
    if (node->getDst() != NULL) node->getDst()->accept(this);
    if (node->getSrc() != NULL) node->getSrc()->accept(this);
    down_level();
}

void PrintICT::visit(EXP *node) {
    printIR("EXP");
    up_level();
    if (node->getE() != NULL) node->getE()->accept(this);
    down_level();
}

void PrintICT::visit(JUMP *node) {
    printIR("JUMP");
    up_level();
    if (node->getE() != NULL) node->getE()->accept(this);
    down_level();
}

void PrintICT::visit(CJUMP *node) {
    printLexemeIR("CJUMP", token_id_to_name(node->getRelop()));
    up_level();
    if (node->getLeft() != NULL) node->getLeft()->accept(this);
    if (node->getRight() != NULL) node->getRight()->accept(this);
    // TODO class Label’ has no member named ‘accept’
//    if (node->getIfTrue() != NULL) node->getIfTrue()->accept(this);
//    if (node->getIfFalse() != NULL) node->getIfTrue()->accept(this);
    down_level();
}

void PrintICT::visit(SEQ *node) {
    printIR("SEQ");
    up_level();
    if (node->getLeft() != NULL) node->getLeft()->accept(this);
    if (node->getRight() != NULL) node->getRight()->accept(this);
    down_level();
}

void PrintICT::visit(LABEL *node) {
    if (node->getL() != NULL) {
        printLexemeIR("LABEL", node->getL()->getName());
    }
}

PrintICT::~PrintICT() = default;

Fragment *Canonization::visit(Fragment *fragment) {
    if (fragment->getNext() != nullptr){
        fragment->setNext(fragment->getNext()->accept(this));
    }
    return fragment;
}

Procedure *Canonization::visit(Procedure *fragment) {
    if (fragment->getBody() != nullptr){
        fragment->setBody(fragment->getBody()->accept(this));
    }
    if (fragment->getNext() != nullptr){
        fragment->setNext(fragment->getNext()->accept(this));
    }
    return fragment;
}

Literal *Canonization::visit(Literal *fragment) {
    if (fragment->getNext() != nullptr){
        fragment->setNext(fragment->getNext()->accept(this));
    }
    return fragment;
}

Variable *Canonization::visit(Variable *fragment) {
    if (fragment->getNext() != nullptr) {
        fragment->setNext(fragment->getNext()->accept(this));
    }
    return fragment;
}


/**********************

ExprNode *Canonization::visit(CONSTF *node) {
    return node;
}

ExprNode *Canonization::visit(CONST *node) {
    return node;
}

ExprNode *Canonization::visit(NAME *node) {
    return node;
}

ExprNode *Canonization::visit(TEMP *node) {
    return node;
}

StmNode *Canonization::visit(LABEL*node) {
    return node;
}

ExprNode *Canonization::visit(ESEQ *node) {
    ExprNode * eseq = node->getE();
    return node;
}

StmNode *Canonization::visit(ExprNode *node) {
    return nullptr;
}
