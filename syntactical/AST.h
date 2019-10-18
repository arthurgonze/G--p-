#ifndef COMPILADOR_2019_3_AST_H
#define COMPILADOR_2019_3_AST_H

// Forward Declarations
class ProgramNode;
class StmtRootNode
class ExpRootNode;
class VarListNode;
class NameDeclNode;
class FunctionListNode;
class TypeListNode;
class TypeNode;
class PointerNode;
class StmtListNode;
class StmtNode;
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
class ExpNode;
class AssignNode;
class NameExpNode;
class PointerValueExpNode;
class AdressValueNode;
class PointerValueNode;
class ArrayNode;
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
};

class StmtRootNode : public ASTNode
{
private:
public:
    virtual ~ASTNode() = default;
};

class ExpRootNode : public StmtRootNode
{
private:
public:
    virtual ~ASTNode() = default;
};

class TokenNode : public ASTNode
{
private:
    int token;
    char *lex;
public:
    TokenNode(int token, char *lex);
    virtual ~TokenNode();
    inline int getToken() { return token; }
    inline char *getLex() { return lex; }
};

class ProgramNode : public ASTNode
{
private:
    TypeListNode *typeList;
    VarListNode *varList;
    FunctionListNode *functionList;
public:
    ProgramNode(FunctionListNode *functionList, TypeListNode *typeList, VarListNode *varList);
    virtual ~ProgramNode();

    inline TypeListNode *getTypeList() { return typeList; }
    inline VarListNode *getVarList() { return varList; }
};

class VarListNode : public ASTNode
{
private:
    NameDeclNode *nameDecl;
    VarListNode *next;
public:
    VarListNode(NameDeclNode *nameDecl, VarListNode *varList);
    virtual ~VarListNode();

    inline NameDeclNode *getNameDecl() { return nameDecl; }
    inline VarListNode *getVarList() { return next; }
};

class NameDeclNode : public ASTNode
{
private:
    TypeNode *type;
    TokenNode *id;
public:
    NameDeclNode(TypeNode *type, TokenNode *id);
    virtual ~NameDeclNode();

    inline TypeNode *getType() { return type; }
    inline TokenNode *getVarList() { return id; }
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
    virtual ~FunctionListNode();

    inline TypeNode *getType() { return type; }
    inline TokenNode *getId() { return id; }
    inline VarListNode *getVarList() { return varList; }
    inline StmtListNode *getStmtList() { return stmtList; }
    inline FunctionListNode *getFunctionList() { return next; }
};

class TypeListNode : public ASTNode
{
private:
    VarListNode *varList;
    TokenNode *id;
    TypeListNode *next;
public:
    TypeListNode(VarListNode *varList, TokenNode *id, TypeListNode *typeList);
    virtual ~TypeListNode();

    inline VarListNode *getVarList() { return varList; }
    inline TokenNode *getId() { return id; }
    inline TypeListNode *getTypeList() { return next; }
};

class TypeNode : public ASTNode
{
private:
    TokenNode *id;
public:
    TypeNode(TokenNode *id) { this->id = id; }
    virtual ~TypeNode();

    inline TokenNode *getId() { return id; }
};

class PointerNode : public ASTNode
{
private:
    TypeNode *type;
public:
    PointerNode(TypeNode *type) { this->type = type; }
    virtual ~PointerNode();

    inline TypeNode *getType() { return type; }
};

class StmtListNode : public StmtRootNode
{
private:
    StmtNode *stmt;
    StmtListNode *next;
public:
    StmtListNode(StmtNode *stmt, StmtListNode *stmtList);
    virtual ~StmtListNode();

    inline StmtNode *getStmtNode() { return stmt; }
    inline StmtListNode *getStmtList() { return next; }
};

class StmtNode : public StmtRootNode
{
private:
    ASTNode *stmt;
public:
    StmtNode(IfNode *stmt);
    StmtNode(WhileNode *stmt);
    StmtNode(SwitchNode *stmt);
    StmtNode(BreakNode *stmt);
    StmtNode(PrintLnNode *stmt);
    StmtNode(ReadNode *stmt);
    StmtNode(ReturnNode *stmt);
    StmtNode(ThrowNode *stmt);
    StmtNode(StmtListNode *stmt);
    StmtNode(CallNode *stmt);
    StmtNode(TryNode *stmt);
    StmtNode(ExpNode *stmt);
    virtual ~StmtNode();

    inline ASTNode *getStmt() { return stmt; }
};

class IfNode : public StmtRootNode
{
private:
    ExpNode *exp;
    StmtNode *ifStmt;
    StmtNode *elseStmt;
public:
    IfNode(ExpNode *exp, StmtNode *ifStmt, StmtNode *elseStmt);
    virtual ~IfNode();

    inline ExpNode *getExp() { return exp; }
    inline StmtNode *getIf() { return ifStmt; }
    inline StmtNode *getElse() { return elseStmt; }
};

class WhileNode : public StmtRootNode
{
private:
    ExpNode *exp;
    StmtNode *stmt;
public:
    WhileNode(ExpNode *exp, StmtNode *stmt);
    virtual ~WhileNode();

    inline ExpNode *getExp() { return exp; }
    inline StmtNode *getStmt() { return stmt; }
};

class SwitchNode : public StmtRootNode
{
private:
    ExpNode *exp;
    CaseBlockNode *caseBlock;
public:
    SwitchNode(ExpNode *exp, CaseBlockNode *caseBlock);
    virtual ~SwitchNode();

    inline ExpNode *getExp() { return exp; }
    inline CaseBlockNode *getCaseBlock() { return caseBlock; }
};

class BreakNode : public StmtRootNode
{
private:
public:
    BreakNode() = default;
    virtual ~BreakNode() = default;
};

class PrintLnNode : public StmtRootNode
{
private:
    ExpListNode *expList;
public:
    PrintLnNode(ExpListNode *expList) { this->expList = expList; }
    virtual ~PrintLnNode();

    inline ExpListNode *getExpList() { return expList; }
};

class ReadNode : public StmtRootNode
{
private:
    ExpNode *exp;
public:
    ReadNode(ExpNode *exp) { this->exp = exp; }
    virtual ~ReadNode();

    inline ExpNode *getExp() { return exp; }
};

class ReturnNode : public StmtRootNode
{
private:
    ExpNode *exp;
public:
    ReturnNode(ExpNode *exp) { this->exp = exp; }
    virtual ~ReturnNode();

    inline ExpNode getExp() { return exp; }
};

class CaseBlockNode : public ASTNode
{
private:
    TokenNode *num; // TODO NUMINT ?
    StmtListNode *stmt;
    CaseBlockNode *next;
public:
    CaseBlockNode(TokenNode *num, StmtListNode *stmt, CaseBlockNode *next);
    virtual ~CaseBlockNode();

    inline TokenNode *getNum() { return num; }
    inline StmtListNode *getStmtList() { return stmt; }
    inline CaseBlockNode *getCaseBlock() { return next; }
};

class ThrowNode : public StmtRootNode
{
private:
public:
    ThrowNode() = default;
    ~ThrowNode() override = default;
};

class ExpListNode : public ExpRootNode
{
private:
    ExpNode *exp;
    ExpListNode *next;
public:
    ExpListNode(ExpNode *exp, ExpListNode *next);
    virtual ~ExpListNode();

    inline ExpNode *getExp() { return exp; }
    inline ExpListNode *getExpList() { return next; }
};

class TryNode : public StmtRootNode
{
private:
    StmtNode *tryStmt;
    StmtNode *catchStmt;
public:
    TryNode(StmtNode *tryStmt, StmtNode *catchStmt);
    virtual ~TryNode();

    inline StmtNode *getTry() { return tryStmt; }
    inline StmtNode *getCatch() { return catchStmt; }
};

class ExpNode : public ExpRootNode
{
private:
    ASTNode *exp;
public:
    ExpNode(TokenNode *exp);
    ExpNode(CallNode *exp);
    ExpNode(NameExpNode *exp);
    ExpNode(PointerValueExpNode *exp);
    ExpNode(AdressValueNode *exp);
    ExpNode(PointerValueNode *exp);
    ExpNode(ArrayNode *exp);
    ExpNode(AssignNode *exp);
    ExpNode(RelationalOPNode *exp);
    ExpNode(AdditionOPNode *exp);
    ExpNode(MultiplicationOPNode *exp);
    ExpNode(BooleanOPNode *exp);
    ExpNode(BitwiseOPNode *exp);
    ExpNode(NotNode *exp);
    ExpNode(SignNode *exp);
    ExpNode(TrueNode *exp);
    ExpNode(FalseNode *exp);
    virtual ~ExpNode();

    inline ASTNode *getExp() { return exp; }
};

class AssignNode : public ExpRootNode
{
private:
    ExpNode *exp1;
    ExpNode *exp2;
public:
    AssignNode(ExpNode *exp1, ExpNode *exp2);
    virtual ~AssignNode();

    inline ExpNode *getExp1() { return exp1; };
    inline ExpNode *getExp2() { return exp2; };
};

class NameExpNode : public ExpRootNode
{
private:
    ExpNode *exp;
    TokenNode *id;
public:
    NameExpNode(ExpNode *exp, TokenNode *id);
    virtual ~NameExpNode();

    inline ExpNode *getExp() { return exp; }
    inline TokenNode *getId() { return id; }
};

class PointerValueExpNode : public ExpRootNode
{
private:
    ExpNode *exp;
    TokenNode *id;
public:
    PointerValueExpNode(ExpNode *exp, TokenNode *id);
    virtual ~PointerValueExpNode();

    inline ExpNode *getExp() { return exp; }
    inline TokenNode *getId() { return id; }
};

class AdressValueNode : public ExpRootNode
{
private:
    ExpNode *exp;
public:
    AdressValueNode(ExpNode *exp) { this->exp = exp; }
    virtual ~AdressValueNode();

    inline ExpNode *getExp() { return exp; }
};

class PointerValueNode : public ExpRootNode
{
private:
    ExpNode *exp;
public:
    PointerValueNode(ExpNode *exp) { this->exp = exp; }
    virtual ~PointerValueNode();

    inline ExpNode *getExp() { return exp; }
};

class ArrayNode : public ExpRootNode
{
private:
    TokenNode *numInt;
public:
    ArrayNode(TokenNode *numInt) { this->numInt = numInt; }
    virtual ~ArrayNode();

    inline TokenNode *getNumInt() { return numInt; }
};

class CallNode : public ExpRootNode
{
private:
    TokenNode *id;
    ExpListNode *expList;
public:
    CallNode(TokenNode *id, ExpListNode *expList);
    virtual ~CallNode();

    inline TokenNode *getId() { return id; }
    inline ExpListNode *getExpList() { return expList; }
};

class RelationalOPNode : public ExpRootNode
{
private:
    TokenNode *op;
    ExpNode *exp1;
    ExpNode *exp2;
public:
    RelationalOPNode(TokenNode *op, ExpNode *exp1, ExpNode *exp2);
    virtual ~RelationalOPNode();

    inline TokenNode *getOp() { return op; }
    inline ExpNode *getExp1() { return exp1; }
    inline ExpNode *getExp2() { return exp2; }
};

class AdditionOPNode : public ExpRootNode
{
private:
    TokenNode *op;
    ExpNode *exp1;
    ExpNode *exp2;
public:
    AdditionOPNode(TokenNode *op, ExpNode *exp1, ExpNode *exp2);
    virtual ~AdditionOPNode();

    inline TokenNode *getOp() { return op; }
    inline ExpNode *getExp1() { return exp1; }
    inline ExpNode *getExp2() { return exp2; }
};

class MultiplicationOPNode : public ExpRootNode
{
private:
    TokenNode *op;
    ExpNode *exp1;
    ExpNode *exp2;
public:
    MultiplicationOPNode(TokenNode *op, ExpNode *exp1, ExpNode *exp2);
    virtual ~MultiplicationOPNode();

    inline TokenNode *getOp() { return op; }
    inline ExpNode *getExp1() { return exp1; }
    inline ExpNode *getExp2() { return exp2; }
};

class BooleanOPNode : public ExpRootNode
{
private:
    TokenNode *op;
    ExpNode *exp1;
    ExpNode *exp2;
public:
    BooleanOPNode(TokenNode *op, ExpNode *exp1, ExpNode *exp2);
    virtual ~BooleanOPNode();

    inline TokenNode *getOp() { return op; }
    inline ExpNode *getExp1() { return exp1; }
    inline ExpNode *getExp2() { return exp2; }
};

class BitwiseOPNode : public ExpRootNode
{
private:
    TokenNode *op;
    ExpNode *exp1;
    ExpNode *exp2;
public:
    BitwiseOPNode(TokenNode *op, ExpNode *exp1, ExpNode *exp2);
    virtual ~BitwiseOPNode();

    inline TokenNode *getOp() { return op; }
    inline ExpNode *getExp1() { return exp1; }
    inline ExpNode *getExp2() { return exp2; }
};

class TrueNode : public ExpRootNode
{
private:
public:
    TrueNode() = default;
    virtual ~TrueNode() = default;

};

class FalseNode : public ExpRootNode
{
private:
public:
    FalseNode() = default;
    virtual ~FalseNode() = default;
};

class NotNode : public ExpRootNode
{
private:
    ExpNode *exp;
public:
    NotNode(ExpNode *exp) { this->exp = exp; }
    virtual ~NotNode();

    inline ExpNode *getExp() { return exp; }
};

class SignNode : public ExpRootNode
{
private:
    ExpNode *exp;
public:
    SignNode(ExpNode *exp) { this->exp = exp; }
    virtual ~SignNode();

    inline ExpNode *getExp() { return exp; }
};

#endif //COMPILADOR_2019_3_AST_H
