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

BINOP::BINOP(int binop, ExprNode *left, ExprNode *right) {
    this->binop = binop;
    this->left = left;
    this->right = right;
}

BINOP::~BINOP() {
    delete this->left;
    delete this->right;
}

MEM::MEM(ExprNode *e) {
    this->e = e;
}

MEM::~MEM() {
    delete this->e;
}

CALL::CALL(ExprNode *func, ExpList *args) {
    this->func = func;
    this->args = args;
}

CALL::~CALL() {
    delete this->func;
    delete this->args;
}

ESEQ::ESEQ(StmNode *s, ExprNode *e) {
    this->s = s;
    this->e = e;
}

ESEQ::~ESEQ() {
    delete this->s;
    delete this->e;
}

MOVE::MOVE(ExprNode *dst, ExprNode *src) {
    this->dst = dst;
    this->src = src;
}

MOVE::~MOVE() {
    delete this->dst;
    delete this->src;
}

EXP::EXP(ExprNode *e) {
    this->e = e;
}

EXP::~EXP() {
    delete this->e;
}

JUMP::JUMP(ExprNode *e, LabelList *targets) {
    this->e = e;
    this->targets = targets;
}

JUMP::~JUMP() {
    delete this->e;
    delete this->targets;
}

CJUMP::CJUMP(int relop, ExprNode *left, ExprNode *right, ExprNode *ifTrue, ExprNode *ifFalse) {
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

SEQ::SEQ(StmNode *left, StmNode *right) {
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

ExpList::ExpList(ExprNode *first, ExpList *next) {
    this->first = first;
    this->next = next;
}

ExpList::~ExpList() {
    delete this->first;
    delete this->next;
}

StmList::StmList(StmNode *first, StmList *next) {
    this->first = first;
    this->next = next;
}

StmList::~StmList() {
    delete this->first;
    delete this->next;
}
