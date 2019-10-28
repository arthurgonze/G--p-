#ifndef COMPILADOR_2019_3_AST_H
#define COMPILADOR_2019_3_AST_H

#include "Visitor.h"

// Forward Declarations, 41 nodes
// ABSTRACTS ROOTS
class ASTNode;
class StmtNode;
class ExpNode;

// TOKENS
class TokenNode;

// PDF NODES
class ProgramNode;
class VarListNode;
class NameDeclNode;
class FunctionListNode;
class TypeListNode;
class TypeNode;// TODO (id, size), (primitive, size), (pointer)
class PointerNode;
class StmtListNode;
class IfNode;
class WhileNode;
class SwitchNode;
class BreakNode;
class PrintLnNode;
class ReadNode;
class ReturnNode;
class CaseBlockNode;
class ThrowNode;
class ExpListNode;
class TryNode;
class AssignNode;
class NameExpNode;
class PointerValueExpNode;
class AdressValueNode;
class PointerValueNode;
class ArrayNode; // TODO (exp, exp)
class CallNode;
class RelationalOPNode;
class AdditionOPNode;
class MultiplicationOPNode;
class BooleanOPNode;
class BitwiseOPNode;
class TrueNode;
class FalseNode;
class NotNode;
class SignNode;

class ASTNode
{
private:
public:
    virtual ~ASTNode() = default;
    virtual void accept(Visitor *visitor) = 0;
};

class StmtNode : public ASTNode
{
private:
public:
    virtual ~StmtNode() = default;
    virtual void accept(Visitor *visitor) = 0;
};

class ExpNode : public StmtNode
{
private:
public:
    virtual ~ExpNode() = default;
    virtual void accept(Visitor *visitor) = 0;
};

class TokenNode : public ASTNode
{
private:
    int token;
    char *lex;
public:
    TokenNode(int token, char *lex);
    ~TokenNode() override;
    inline int getToken() { return token; }
    inline char *getLex() { return lex; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class ProgramNode : public ASTNode
{
private:
    FunctionListNode *functionList;
    TypeListNode *typeList;
    VarListNode *varList;
public:
    ProgramNode(FunctionListNode *functionList, TypeListNode *typeList, VarListNode *varList);
    ~ProgramNode() override;

    inline TypeListNode *getTypeList() { return typeList; }
    inline VarListNode *getVarList() { return varList; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class VarListNode : public ASTNode
{
private:
    NameDeclNode *nameDecl;
    VarListNode *next;
public:
    VarListNode(NameDeclNode *nameDecl, VarListNode *varList);
    ~VarListNode() override;

    inline NameDeclNode *getNameDecl() { return nameDecl; }
    inline VarListNode *getVarList() { return next; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class NameDeclNode : public ASTNode
{
private:
    TypeNode *type;
    TokenNode *id;
public:
    NameDeclNode(TypeNode *type, TokenNode *id);
    ~NameDeclNode() override;

    inline TypeNode *getType() { return type; }
    inline TokenNode *getVarList() { return id; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class FunctionListNode : public ASTNode
{
private:
    TypeNode *type;
    TokenNode *id;
    VarListNode *varList;
    StmtListNode *stmtList;

    FunctionListNode *next;
public:
    FunctionListNode(TypeNode *type, TokenNode *id, VarListNode *varList, StmtListNode *stmtList, FunctionListNode *functionList);
    ~FunctionListNode() override;

    inline TypeNode *getType() { return type; }
    inline TokenNode *getId() { return id; }
    inline VarListNode *getVarList() { return varList; }
    inline StmtListNode *getStmtList() { return stmtList; }
    inline FunctionListNode *getFunctionList() { return next; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class TypeListNode : public ASTNode
{
private:
    VarListNode *varList;
    TokenNode *id;
    TypeListNode *next;
public:
    TypeListNode(VarListNode *varList, TokenNode *id, TypeListNode *typeList);
    ~TypeListNode() override;

    inline VarListNode *getVarList() { return varList; }
    inline TokenNode *getId() { return id; }
    inline TypeListNode *getTypeList() { return next; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class TypeNode : public ASTNode
{
private:
    TokenNode *id;
public:
    explicit TypeNode(TokenNode *id) { this->id = id; }
    // TODO (id, size)
    // TODO (primitive, size)
    // TODO (pointer)
    ~TypeNode() override { delete this->id; }

    inline TokenNode *getId() { return id; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class PointerNode : public ASTNode
{
private:
    TypeNode *type;
public:
    explicit PointerNode(TypeNode *type) { this->type = type; }
    ~PointerNode() override { delete this->type; }

    inline TypeNode *getType() { return type; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class StmtListNode : public StmtNode
{
private:
    StmtNode *stmt;
    StmtListNode *next;
public:
    StmtListNode(StmtNode *stmt, StmtListNode *stmtList);
    ~StmtListNode() override;

    inline StmtNode *getStmtNode() { return stmt; }
    inline StmtListNode *getStmtList() { return next; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class IfNode : public StmtNode
{
private:
    ExpNode *exp;
    StmtNode *ifStmt;
    StmtNode *elseStmt;
public:
    IfNode(ExpNode *exp, StmtNode *ifStmt, StmtNode *elseStmt);
    ~IfNode() override;

    inline ExpNode *getExp() { return exp; }
    inline StmtNode *getIf() { return ifStmt; }
    inline StmtNode *getElse() { return elseStmt; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class WhileNode : public StmtNode
{
private:
    ExpNode *exp;
    StmtNode *stmt;
public:
    WhileNode(ExpNode *exp, StmtNode *stmt);
    ~WhileNode() override;

    inline ExpNode *getExp() { return exp; }
    inline StmtNode *getStmt() { return stmt; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class SwitchNode : public StmtNode
{
private:
    ExpNode *exp;
    CaseBlockNode *caseBlock;
public:
    SwitchNode(ExpNode *exp, CaseBlockNode *caseBlock);
    ~SwitchNode() override;

    inline ExpNode *getExp() { return exp; }
    inline CaseBlockNode *getCaseBlock() { return caseBlock; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class BreakNode : public StmtNode
{
private:
public:
    BreakNode() = default;
    ~BreakNode() override = default;
};

class PrintLnNode : public StmtNode
{
private:
    ExpListNode *expList;
public:
    explicit PrintLnNode(ExpListNode *expList) { this->expList = expList; }
    ~PrintLnNode() override { delete this->expList; }

    inline ExpListNode *getExpList() { return expList; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class ReadNode : public StmtNode
{
private:
    ExpNode *exp;
public:
    explicit ReadNode(ExpNode *exp) { this->exp = exp; }
    ~ReadNode() override { delete this->exp; }

    inline ExpNode *getExp() { return exp; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class ReturnNode : public StmtNode
{
private:
    ExpNode *exp;
public:
    explicit ReturnNode(ExpNode *exp) { this->exp = exp; }
    ~ReturnNode() override { delete this->exp; }

    inline ExpNode *getExp() { return exp; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class CaseBlockNode : public ASTNode
{
private:
    TokenNode *num;
    StmtListNode *stmt;
    CaseBlockNode *next;
public:
    CaseBlockNode(TokenNode *num, StmtListNode *stmt, CaseBlockNode *next);
    ~CaseBlockNode() override;

    inline TokenNode *getNum() { return num; }
    inline StmtListNode *getStmtList() { return stmt; }
    inline CaseBlockNode *getCaseBlock() { return next; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class ThrowNode : public StmtNode
{
private:
public:
    ThrowNode() = default;
    ~ThrowNode() override = default;

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class ExpListNode : public ExpNode
{
private:
    ExpNode *exp;
    ExpListNode *next;
public:
    ExpListNode(ExpNode *exp, ExpListNode *next);
    ~ExpListNode() override;

    inline ExpNode *getExp() { return exp; }
    inline ExpListNode *getExpList() { return next; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class TryNode : public StmtNode
{
private:
    StmtNode *tryStmt;
    StmtNode *catchStmt;
public:
    TryNode(StmtNode *tryStmt, StmtNode *catchStmt);
    ~TryNode() override;

    inline StmtNode *getTry() { return tryStmt; }
    inline StmtNode *getCatch() { return catchStmt; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class AssignNode : public ExpNode
{
private:
    ExpNode *exp1;
    ExpNode *exp2;
public:
    AssignNode(ExpNode *exp1, ExpNode *exp2);
    ~AssignNode() override;

    inline ExpNode *getExp1() { return exp1; };
    inline ExpNode *getExp2() { return exp2; };

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class NameExpNode : public ExpNode
{
private:
    ExpNode *exp;
    TokenNode *id;
public:
    NameExpNode(ExpNode *exp, TokenNode *id);
    ~NameExpNode() override;

    inline ExpNode *getExp() { return exp; }
    inline TokenNode *getId() { return id; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class PointerValueExpNode : public ExpNode
{
private:
    ExpNode *exp;
    TokenNode *id;
public:
    PointerValueExpNode(ExpNode *exp, TokenNode *id);
    ~PointerValueExpNode() override;

    inline ExpNode *getExp() { return exp; }
    inline TokenNode *getId() { return id; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class AdressValueNode : public ExpNode
{
private:
    ExpNode *exp;
public:
    explicit AdressValueNode(ExpNode *exp) { this->exp = exp; }
    ~AdressValueNode() override { delete this->exp; }

    inline ExpNode *getExp() { return exp; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class PointerValueNode : public ExpNode
{
private:
    ExpNode *exp;
public:
    explicit PointerValueNode(ExpNode *exp) { this->exp = exp; }
    ~PointerValueNode() override { delete this->exp; }

    inline ExpNode *getExp() { return exp; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class ArrayNode : public ExpNode
{
private:
    TokenNode *numInt;
public:
    explicit ArrayNode(TokenNode *numInt) { this->numInt = numInt; }
    // TODO (exp, exp)
    ~ArrayNode() override { delete this->numInt; }

    inline TokenNode *getNumInt() { return numInt; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class CallNode : public ExpNode
{
private:
    TokenNode *id;
    ExpListNode *expList;
public:
    CallNode(TokenNode *id, ExpListNode *expList);
    ~CallNode() override;

    inline TokenNode *getId() { return id; }
    inline ExpListNode *getExpList() { return expList; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class RelationalOPNode : public ExpNode
{
private:
    TokenNode *op;
    ExpNode *exp1;
    ExpNode *exp2;
public:
    RelationalOPNode(TokenNode *op, ExpNode *exp1, ExpNode *exp2);
    ~RelationalOPNode() override;

    inline TokenNode *getOp() { return op; }
    inline ExpNode *getExp1() { return exp1; }
    inline ExpNode *getExp2() { return exp2; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class AdditionOPNode : public ExpNode
{
private:
    TokenNode *op;
    ExpNode *exp1;
    ExpNode *exp2;
public:
    AdditionOPNode(TokenNode *op, ExpNode *exp1, ExpNode *exp2);
    ~AdditionOPNode() override;

    inline TokenNode *getOp() { return op; }
    inline ExpNode *getExp1() { return exp1; }
    inline ExpNode *getExp2() { return exp2; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class MultiplicationOPNode : public ExpNode
{
private:
    TokenNode *op;
    ExpNode *exp1;
    ExpNode *exp2;
public:
    MultiplicationOPNode(TokenNode *op, ExpNode *exp1, ExpNode *exp2);
    ~MultiplicationOPNode() override;

    inline TokenNode *getOp() { return op; }
    inline ExpNode *getExp1() { return exp1; }
    inline ExpNode *getExp2() { return exp2; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class BooleanOPNode : public ExpNode
{
private:
    TokenNode *op;
    ExpNode *exp1;
    ExpNode *exp2;
public:
    BooleanOPNode(TokenNode *op, ExpNode *exp1, ExpNode *exp2);
    ~BooleanOPNode() override;

    inline TokenNode *getOp() { return op; }
    inline ExpNode *getExp1() { return exp1; }
    inline ExpNode *getExp2() { return exp2; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class BitwiseOPNode : public ExpNode
{
private:
    TokenNode *op;
    ExpNode *exp1;
    ExpNode *exp2;
public:
    BitwiseOPNode(TokenNode *op, ExpNode *exp1, ExpNode *exp2);
    ~BitwiseOPNode() override;

    inline TokenNode *getOp() { return op; }
    inline ExpNode *getExp1() { return exp1; }
    inline ExpNode *getExp2() { return exp2; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class TrueNode : public ExpNode
{
private:
public:
    TrueNode() = default;
    ~TrueNode() override = default;

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class FalseNode : public ExpNode
{
private:
public:
    FalseNode() = default;
    ~FalseNode() override = default;

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class NotNode : public ExpNode
{
private:
    ExpNode *exp;
public:
    explicit NotNode(ExpNode *exp) { this->exp = exp; }
    ~NotNode() override { delete this->exp; }

    inline ExpNode *getExp() { return exp; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

class SignNode : public ExpNode
{
private:
    ExpNode *exp;
public:
    explicit SignNode(ExpNode *exp) { this->exp = exp; }
    ~SignNode() override { delete this->exp; }

    inline ExpNode *getExp() { return exp; }

    void accept(Visitor *visitor) override { visitor->visit(this); }
};

#endif //COMPILADOR_2019_3_AST_H
