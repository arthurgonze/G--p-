#include "ict.h"

CONST::CONST(int i) {
    this->i = i;
}

CONSTF::CONSTF(float j) {
    this->j = j;
}

NAME::NAME(Label *l) {
    this->l = l;
}

NAME::~NAME() {
    delete this->l;
}

TEMP::TEMP(Temp *t) {
    this->t = t;
}

TEMP::~TEMP() {
    delete this->t;
}

BINOP::BINOP(int binop, ExpNode *left, ExpNode *right) {
    this->binop = binop;
    this->left = left;
    this->right = right;
}

BINOP::~BINOP() {
    delete this->left;
    delete this->right;
}

MEM::MEM(ExpNode *e) {
    this->e = e;
}

MEM::~MEM() {
    delete this->e;
}

CALL::CALL(ExpNode *func, ExpList *args) {
    this->func = func;
    this->args = args;
}

CALL::~CALL() {
    delete this->func;
    delete this->args;
}

ESEQ::ESEQ(Stm *s, ExpNode *e) {
    this->s = s;
    this->e = e;
}

ESEQ::~ESEQ() {
    delete this->s;
    delete this->e;
}

MOVE::MOVE(ExpNode *dst, ExpNode *src) {
    this->dst = dst;
    this->src = src;
}

MOVE::~MOVE() {
    delete this->dst;
    delete this->src;
}

EXP::EXP(ExpNode *e) {
    this->e = e;
}

EXP::~EXP() {
    delete this->e;
}

JUMP::JUMP(ExpNode *e, LabelList *targets) {
    this->e = e;
    this->targets = targets;
}

JUMP::~JUMP() {
    delete this->e;
    delete this->targets;
}

CJUMP::CJUMP(int relop, ExpNode *left, ExpNode *right, Label *ifTrue, Label *ifFalse) {
    this->relop = relop;
    this->left = left;
    this->right = right;
    this->ifTrue = ifTrue;
    this->ifFalse = ifFalse;
}

CJUMP::~CJUMP() {
    delete this->left;
    delete this->right;
    delete this->ifTrue;
    delete this->ifFalse;
}

SEQ::SEQ(Stm *left, Stm *right) {
    this->left = left;
    this->right = right;
}

SEQ::~SEQ() {
    delete this->left;
    delete this->right;
}

LABEL::LABEL(Label *l) {
    this->l = l;
}

LABEL::~LABEL() {
    delete this->l;
}

ExpList::ExpList(ExpNode *prim, ExpList *prox) {
    this->prim = prim;
    this->prox = prox;
}

ExpList::~ExpList() {
    delete this->prim;
    delete this->prox;
}

StmList::StmList(Stm *prim, StmList *prox) {
    this->prim = prim;
    this->prox = prox;
}

StmList::~StmList() {
    delete this->prim;
    delete this->prox;
}
