
#include "ast.h"

TokenNode::TokenNode(int tok, const char *lex) {
    this->token = tok;
    this->setLexeme(lex);

    this->setLValue(false);
    this->setPointer(false);
    this->setType(-42);
    this->setTypeLexeme("NotDefined");
    this->setLine(-42);
    this->setArraySize(-42);
    this->setOffset(-42);
}

TokenNode::~TokenNode() {
    //delete this->token;
}

ProgramNode::ProgramNode(FunctionListNode *functions, TypeDeclNode *typelist, VarDeclNode *varlist) {
    this->functions = functions;
    this->typelist = typelist;
    this->varlist = varlist;
}

ProgramNode::~ProgramNode() {
    delete this->varlist;
    delete this->typelist;
    delete this->functions;
}

TryNode::TryNode(StmtNode *tryStmt, StmtNode *catchStmt) {
    this->tryStmt = tryStmt;
    this->catchStmt = catchStmt;
}

TryNode::~TryNode() {
    delete this->tryStmt;
    delete this->catchStmt;
}

StmtListNode::StmtListNode(StmtNode *stmt, StmtListNode *next) {
    this->stmt = stmt;
    this->next = next;
}

StmtListNode::~StmtListNode() {
    delete this->next;
    delete this->stmt;
}

WhileNode::WhileNode(ExpNode *head, StmtNode *body) {
    this->head = head;
    this->body = body;
}

WhileNode::~WhileNode() {
    delete this->head;
    delete this->body;
}

IfNode::IfNode(ExpNode *head, StmtNode *trueStmt, StmtNode *falseStmt) {
    this->head = head;
    this->trueStmt = trueStmt;
    this->falseStmt = falseStmt;
}

IfNode::~IfNode() {
    delete this->head;
    delete this->trueStmt;
    delete this->falseStmt;
}

CaseBlockNode::CaseBlockNode(TokenNode *num, StmtListNode *stmt, CaseBlockNode *next) {
    this->num = num;
    this->stmt = stmt;
    this->next = next;
}

CaseBlockNode::~CaseBlockNode() {
    delete this->num;
    delete this->stmt;
    delete this->next;
}

SwitchNode::SwitchNode(ExpNode *exp, CaseBlockNode *block) {
    this->exp = exp;
    this->block = block;
}

SwitchNode::~SwitchNode() {
    delete this->exp;
    delete this->block;
}

IdListNode::IdListNode(PointerNode *pointer, TokenNode *id, ArrayNode *array, IdListNode *next) {
    this->pointer = pointer;
    this->id = id;
    this->array = array;
    this->next = next;
}

IdListNode::~IdListNode() {
    delete this->pointer;
    delete this->id;
    delete this->array;
    delete this->next;
}

VarDeclNode::VarDeclNode(TypeNode *type, IdListNode *idlist, VarDeclNode *next) {
    this->type = type;
    this->idlist = idlist;
    this->next = next;
}

VarDeclNode::~VarDeclNode() {
    delete this->type;
    delete this->idlist;
    delete this->next;
}

TypeDeclNode::TypeDeclNode(VarDeclNode *dec, TokenNode *id, TypeDeclNode *next) {
    this->dec = dec;
    this->id = id;
    this->next = next;
}

TypeDeclNode::~TypeDeclNode() {
    delete this->dec;
    delete this->id;
    delete this->next;
}

FormalListNode::FormalListNode(TypeNode *type, PointerNode *pointer, TokenNode *id, ArrayNode *array,
                               FormalListNode *next) {
    this->type = type;
    this->pointer = pointer;
    this->id = id;
    this->array = array;
    this->next = next;
}

FormalListNode::~FormalListNode() {
    delete this->type;
    delete this->pointer;
    delete this->id;
    delete this->array;
    delete this->next;
}

ExpListNode::ExpListNode(ExpNode *exp, ExpListNode *next) {
    this->exp = exp;
    this->next = next;
}

ExpListNode::~ExpListNode() {
    delete this->exp;
    delete this->next;
}

CallNode::CallNode(TokenNode *id, ExpListNode *parameters) {
    this->id = id;
    this->parameters = parameters;

    this->setLValue(false);
    this->setPointer(false);
    this->setType(-42);
    this->setTypeLexeme("NotDefined");
    this->setLine(-42);
    this->setArraySize(-42);
    this->setLexeme("NotDefined");
}

CallNode::~CallNode() {
    delete this->id;
    delete this->parameters;
}

PrimaryNode::PrimaryNode(TokenNode *token) {
    this->token = token;
    this->exp = nullptr;

    this->setLValue(false);
    this->setPointer(false);
    this->setType(-42);
    this->setTypeLexeme("NotDefined");
    this->setLine(-42);
    this->setArraySize(-42);
    this->setLexeme("NotDefined");
}

PrimaryNode::PrimaryNode(ExpNode *exp) {
    this->exp = exp;
    this->token = nullptr;

    this->setLValue(false);
    this->setPointer(false);
    this->setType(-42);
    this->setTypeLexeme("NotDefined");
    this->setLine(-42);
    this->setArraySize(-42);
    this->setLexeme("NotDefined");
}

PrimaryNode::~PrimaryNode() {
    delete this->token;
    delete this->exp;
}

AssignNode::AssignNode(ExpNode *exp1, ExpNode *exp2) {
    this->exp1 = exp1;
    this->exp2 = exp2;

    this->setLValue(false);
    this->setPointer(false);
    this->setType(-42);
    this->setTypeLexeme("NotDefined");
    this->setLine(-42);
    this->setArraySize(-42);
    this->setLexeme("NotDefined");
}

AssignNode::~AssignNode() {
    delete this->exp1;
    delete this->exp2;
}

BooleanOPNode::BooleanOPNode(TokenNode *op, ExpNode *exp1, ExpNode *exp2) {
    this->op = op;
    this->exp1 = exp1;
    this->exp2 = exp2;

    this->setLValue(false);
    this->setPointer(false);
    this->setTypeLexeme("NotDefined");
    this->setLine(-42);
    this->setArraySize(-42);
    this->setLexeme("NotDefined");
}

BooleanOPNode::~BooleanOPNode() {
    delete this->op;
    delete this->exp1;
    delete this->exp2;
}

NameExpNode::NameExpNode(ExpNode *exp, TokenNode *id) {
    this->exp = exp;
    this->id = id;

    this->setLValue(false);
    this->setPointer(false);
    this->setType(-42);
    this->setTypeLexeme("NotDefined");
    this->setLine(-42);
    this->setArraySize(-42);
    this->setLexeme("NotDefined");
}

NameExpNode::~NameExpNode() {
    delete this->exp;
    delete this->id;
}

PointerExpNode::PointerExpNode(ExpNode *exp, TokenNode *id) {
    this->exp = exp;
    this->id = id;

    this->setLValue(false);
    this->setPointer(false);
    this->setType(-42);
    this->setTypeLexeme("NotDefined");
    this->setLine(-42);
    this->setArraySize(-42);
    this->setLexeme("NotDefined");
}

PointerExpNode::~PointerExpNode() {
    delete this->id;
    delete this->exp;
}

ArrayCallNode::ArrayCallNode(ExpNode *exp, ExpNode *index) {
    this->exp = exp;
    this->index = index;

    this->setLValue(false);
    this->setPointer(false);
    this->setType(-42);
    this->setTypeLexeme("NotDefined");
    this->setLine(-42);
    this->setArraySize(-42);
    this->setLexeme("NotDefined");
}

ArrayCallNode::~ArrayCallNode() {
    delete this->index;
    delete this->exp;
}

AdditionOPNode::AdditionOPNode(TokenNode *op, ExpNode *exp1, ExpNode *exp2) {
    this->op = op;
    this->exp1 = exp1;
    this->exp2 = exp2;

    this->setLValue(false);
    this->setPointer(false);
    this->setType(-42);
    this->setTypeLexeme("NotDefined");
    this->setLine(-42);
    this->setArraySize(-42);
    this->setLexeme("NotDefined");
}

AdditionOPNode::~AdditionOPNode() {
    delete this->op;
    delete this->exp1;
    delete this->exp2;
}

MultiplicationOPNode::MultiplicationOPNode(TokenNode *op, ExpNode *exp1, ExpNode *exp2) {
    this->op = op;
    this->exp1 = exp1;
    this->exp2 = exp2;

    this->setLValue(false);
    this->setPointer(false);
    this->setType(-42);
    this->setTypeLexeme("NotDefined");
    this->setLine(-42);
    this->setArraySize(-42);
    this->setLexeme("NotDefined");
}

MultiplicationOPNode::~MultiplicationOPNode() {
    delete this->op;
    delete this->exp1;
    delete this->exp2;
}

VarStmtNode::VarStmtNode(VarDeclNode *dec, StmtListNode *body) {
    this->dec = dec;
    this->body = body;
}

VarStmtNode::~VarStmtNode() {
    delete this->dec;
    delete this->body;
}

FunctionNode::FunctionNode(TypeNode *type, PointerNode *pointer, TokenNode *id, FormalListNode *parameters,
                           VarDeclNode *local, StmtListNode *body) {
    this->type = type;
    this->pointer = pointer;
    this->id = id;
    this->parameters = parameters;
    this->local = local;
    this->body = body;
}

FunctionNode::~FunctionNode() {
    delete this->type;
    delete this->pointer;
    delete this->id;
    delete this->parameters;
    delete this->local;
    delete this->body;
}

FunctionListNode::FunctionListNode(FunctionNode *function, FunctionListNode *next) {
    this->function = function;
    this->next = next;
}

FunctionListNode::~FunctionListNode() {
    delete this->function;
    delete this->next;
}

SignNode::SignNode(ExpNode *exp) {
    this->exp = exp;

    this->setLValue(false);
    this->setPointer(false);
    this->setType(-42);
    this->setTypeLexeme("NotDefined");
    this->setLine(-42);
    this->setArraySize(-42);
    this->setLexeme("NotDefined");
}

AddressValNode::AddressValNode(ExpNode *exp) {
    this->exp = exp;

    this->setLValue(false);
    this->setPointer(false);
    this->setType(-42);
    this->setTypeLexeme("NotDefined");
    this->setLine(-42);
    this->setArraySize(-42);
    this->setLexeme("NotDefined");
}

PointerValNode::PointerValNode(ExpNode *exp) {
    this->exp = exp;

    this->setLValue(false);
    this->setPointer(false);
    this->setType(-42);
    this->setTypeLexeme("NotDefined");
    this->setLine(-42);
    this->setArraySize(-42);
    this->setLexeme("NotDefined");
}

NotNode::NotNode(ExpNode *exp) {
    this->exp = exp;

    this->setLValue(false);
    this->setPointer(false);
    this->setType(-42);
    this->setTypeLexeme("NotDefined");
    this->setLine(-42);
    this->setArraySize(-42);
    this->setLexeme("NotDefined");
}
