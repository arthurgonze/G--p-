#include "AST.h"

/**
 * Module with constructors and destructors of some classes from the AST.
 * Only those that could not be inlined were defined here.
 */

TokenNode::TokenNode(int token, char *lex)
{
    this->token = token;
    this->lex = lex;
}

TokenNode::~TokenNode()
{
    delete this->token;
    delete this->lex;
}

ProgramNode::ProgramNode(FunctionListNode *functionList, TypeListNode *typeList, VarListNode *varList)
{
    this->functionList = functionList;
    this->typeList = typeList;
    this->varList = varList;
}

ProgramNode::~ProgramNode()
{
    delete this->functionList;
    delete this->typeList;
    delete this->varList;
}

VarListNode::VarListNode(NameDeclNode *nameDecl, VarListNode *varList)
{
    this->nameDecl = nameDecl;
    this->next = varList;
}

VarListNode::~VarListNode()
{
    delete this->nameDecl;
    delete this->next;
}

NameDeclNode::NameDeclNode(TypeNode *type, TokenNode *id)
{
    this->type = type;
    this->id = id;
}

NameDeclNode::~NameDeclNode()
{
    delete this->id;
    delete this->type;
}

FunctionListNode::FunctionListNode(TypeNode *type, TokenNode *id, VarListNode *varList, StmtListNode *stmtList, FunctionListNode *functionList)
{
    this->type = type;
    this->id = id;
    this->varList = varList;
    this->stmtList = stmtList;
    this->next = functionList;
}

FunctionListNode::~FunctionListNode() {
    delete this->type;
    delete this->id;
    delete this->varList;
    delete this->stmtList;
    delete this->next;
}

TypeListNode::TypeListNode(VarListNode *var_list, TokenNode *id, TypeListNode *next)
{
    this->varList = var_list;
    this->id = id;
    this->next = next;
}

TypeListNode::~TypeListNode()
{
    delete this->varList;
    delete this->id;
    delete this->next;
}

StmtListNode::StmtListNode(StmtNode *statement, StmtListNode *statement_list)
{
    this->stmt = statement;
    this->next = statement_list;
}

StmtListNode::~StmtListNode()
{
    delete this->stmt;
    delete this->next;
}

IfNode::IfNode(ExpNode *exp, StmtNode *if_stmt, StmtNode *else_stmt)
{
    this->exp = exp;
    this->ifStmt = if_stmt;
    this->elseStmt = else_stmt;
}

IfNode::~IfNode()
{
    delete this->exp;
    delete this->ifStmt;
    delete this->elseStmt;
}

WhileNode::WhileNode(ExpNode *exp, StmtNode *statement)
{
    this->exp = exp;
    this->stmt = statement;
}

WhileNode::~WhileNode()
{
    delete this->exp;
    delete this->stmt;
}

SwitchNode::SwitchNode(ExpNode *exp, CaseBlockNode *case_block)
{
    this->exp = exp;
    this->caseBlock = case_block;
}

SwitchNode::~SwitchNode()
{
    delete this->exp;
    delete this->caseBlock;
}

CaseBlockNode::CaseBlockNode(TokenNode *num, StmtListNode *statement_list, CaseBlockNode *case_block)
{
    this->num = num;
    this->stmt = statement_list;
    this->next = case_block;
}

CaseBlockNode::~CaseBlockNode()
{
    delete this->num;
    delete this->stmt;
    delete this->next;
}

ExpListNode::ExpListNode(ExpNode *exp, ExpListNode *exp_list)
{
    this->exp = exp;
    this->next = exp_list;
}

ExpListNode::~ExpListNode()
{
    delete this->exp;
    delete this->next;
}

TryNode::TryNode(StmtNode *try_stmt, StmtNode *catch_stmt)
{
    this->tryStmt = try_stmt;
    this->catchStmt = catch_stmt;
}

TryNode::~TryNode()
{
    delete this->tryStmt;
    delete this->catchStmt;
}

AssignNode::AssignNode(ExpNode *left_exp, ExpNode *right_exp)
{
    this->exp1 = left_exp;
    this->exp2 = right_exp;
}

AssignNode::~AssignNode()
{
    delete this->exp1;
    delete this->exp2;
}

NameExpNode::NameExpNode(ExpNode *exp, TokenNode *id)
{
    this->exp = exp;
    this->id = id;
}

NameExpNode::~NameExpNode()
{
    delete this->exp;
    delete this->id;
}

PointerValueExpNode::PointerValueExpNode(ExpNode *exp, TokenNode *id)
{
    this->exp = exp;
    this->id = id;
}

PointerValueExpNode::~PointerValueExpNode()
{
    delete this->exp;
    delete this->id;
}

CallNode::CallNode(TokenNode *id, ExpListNode *expList)
{
    this->id = id;
    this->expList = expList;
}

CallNode::~CallNode()
{
    delete this->id;
    delete this->expList;
}

RelationalOPNode::RelationalOPNode(TokenNode *op, ExpNode *exp1, ExpNode *exp2)
{
    this->op = op;
    this->exp1 = exp1;
    this->exp2 = exp2;
}

RelationalOPNode::~RelationalOPNode()
{
    delete this->exp1;
    delete this->exp1;
    delete this->op;
}

AdditionOPNode::AdditionOPNode(TokenNode *op, ExpNode *exp1, ExpNode *exp2)
{
    this->op = op;
    this->exp1 = exp1;
    this->exp2 = exp2;;
}

AdditionOPNode::~AdditionOPNode()
{
    delete this->exp1;
    delete this->exp1;
    delete this->op;
}

MultiplicationOPNode::MultiplicationOPNode(TokenNode *op, ExpNode *exp1, ExpNode *exp2)
{
    this->op = op;
    this->exp1 = exp1;
    this->exp2 = exp2;;
}

MultiplicationOPNode::~MultiplicationOPNode()
{
    delete this->exp1;
    delete this->exp1;
    delete this->op;
}

BooleanOPNode::BooleanOPNode(TokenNode *op, ExpNode *exp1, ExpNode *exp2)
{
    this->op = op;
    this->exp1 = exp1;
    this->exp2 = exp2;;
}

BooleanOPNode::~BooleanOPNode()
{
    delete this->exp1;
    delete this->exp1;
    delete this->op;
}

BitwiseOPNode::BitwiseOPNode(TokenNode *op, ExpNode *exp1, ExpNode *exp2)
{
    this->op = op;
    this->exp1 = exp1;
    this->exp2 = exp2;
}

BitwiseOPNode::~BitwiseOPNode()
{
    delete this->exp1;
    delete this->exp1;
    delete this->op;
}