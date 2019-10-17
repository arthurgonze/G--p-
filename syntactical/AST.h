#ifndef COMPILADOR_2019_3_AST_H
#define COMPILADOR_2019_3_AST_H

// Forward Declarations
class AST;
class IdNode;
class ProgramNode;
class TypeDeclNode;
class VarDeclNode;
class IdListNode;
// class IdExprNode;
class PointerNode;
class ArrayNode;
class FormaListNode;
// class FormalRestNode;
class TypeNode;
class StmtListNode;
class StmtNode;
class IfExprNode;
class CaseBlockNode;
class ExprListNode;
class ExprAssignNode;
class ExprOrNode;
class ExprAndNode;
class ExprEqualityNode;
class ExprRelationalNode;
class ExprAdditiveNode;
class ExprMultiplicativeNode;
class ExprUnaryNode;
class PrimaryNode;
class PostFixExprNode;// TODO segmentar igual Stmt
class BreakNode;
class PrintNode;
class ReadlnNode;
class ReturnNode;
class ThrowNode;
class TryCatchNode;
class WhileNode;
class SwitchNode;

class AST
{
public:
    virtual ~AST();
};

class IdNode : public AST
{
private:
    int token;
    char *lex;
public:
    IdNode(int token, char *lex);
    virtual ~IdNode();
    inline int getToken() { return token; }
    inline char *getLex() { return lex; }
};

class ProgramNode : public AST
{
private:
    TypeDeclNode *typeDecl;
    VarDeclNode *varDecl;
public:
    ProgramNode(TypeDeclNode *typeDecl, VarDeclNode *varDecl);
    virtual ~ProgramNode();

    inline TypeDeclNode *getTypeDecl() { return typeDecl; }
    inline VarDeclNode *getVarDecl() { return varDecl; }
};

class TypeDeclNode : public AST
{
private:
    VarDeclNode *varDecl;
    IdNode *id;
    TypeDeclNode *next;
public:
    TypeDeclNode(VarDeclNode *varDecl, IdNode *id, TypeDeclNode *next);
    virtual ~TypeDeclNode();
};

class VarDeclNode : public AST
{
private:
    TypeNode *type;
    IdListNode *idList;
    VarDeclNode *next;
public:
    VarDeclNode(TypeNode *type, IdListNode *idlist, VarDeclNode *varDecl);
    virtual ~VarDeclNode();
    inline TypeNode *getType() { return type; }
    inline IdListNode *getIdList() { return idList; }
    inline VarDeclNode *getVarDecl() { return next; }
};

class IdListNode : public AST
{
private:
    PointerNode *pointer;
    IdNode *id;
    ArrayNode *array;
    IdListNode *next;
public:
    IdListNode(PointerNode *pointer, IdNode *id, ArrayNode *array, IdListNode *idList);
    virtual ~IdListNode();
    inline PointerNode *getPointer() { return pointer; }
    inline IdNode *getId() { return id; }
    inline ArrayNode *getArray() { return array; }
    inline IdListNode *getIdList() { return next; }
};

class PointerNode : public AST
{
public:
    virtual ~PointerNode();
};

class ArrayNode : public AST
{
private:
    IdNode *index;
public:
    ArrayNode(IdNode *integer);
    virtual ~ArrayNode();
    inline IdNode *getIndex() { return index; }
};

class FormaListNode : public AST
{
private:
    TypeNode *type;
    PointerNode *pointer;
    IdNode *id;
    ArrayNode *array;
    FormaListNode *next;
public:
    FormaListNode(TypeNode *type, PointerNode *pointer, IdNode *id, ArrayNode *array, FormaListNode *formalList);
    virtual ~FormaListNode();

    inline TypeNode *getType() { return type; }
    inline PointerNode *getPointer() { return pointer; }
    inline IdNode *getId() { return id; }
    inline ArrayNode *getArray() { return array; }
    inline FormaListNode *getFormalList() { return next; }
};

class TypeNode : public AST
{
private:
    IdNode *id;
public:
    TypeNode(IdNode *id);
    virtual ~TypeNode();
    inline IdNode *getId() { return id; }
};

class StmtListNode : public AST
{
private:
    StmtNode *stmt;
    StmtListNode *next;
public:
    StmtListNode(StmtNode *statement, StmtListNode *stmtList);
    virtual ~StmtListNode();

    inline StmtNode *getStmt() { return stmt; }
    inline StmtListNode *getStmtList() { return next; }
};

class StmtNode : public AST
{
private:
    AST *stmt;
    StmtNode(StmtListNode *stmlist);
    StmtNode(IfExprNode *aux);
    StmtNode(WhileNode *aux);
    StmtNode(ExprAssignNode *aux);
    StmtNode(BreakNode *aux);
    StmtNode(PrintNode *aux);
    StmtNode(ReadlnNode *aux);
    StmtNode(ReturnNode *aux);
    StmtNode(ThrowNode *aux);
    StmtNode(TryCatchNode *aux);
    StmtNode(SwitchNode *aux);
public:
    virtual ~StmtNode();

    inline AST *getStmt() { return stmt; }
};

class IfExprNode : public AST
{
private:
    ExprAssignNode *exprAssign;
    StmtNode *ifStmt;
    StmtNode *elseStmt;
public:
    IfExprNode(ExprAssignNode *exprAssign, StmtNode *ifStmt, StmtNode *elseStmt);
    virtual ~IfExprNode();

    inline ExprAssignNode *getExprAssign() { return exprAssign; }
    inline StmtNode *getIf() { return ifStmt; }
    inline StmtNode *getElse() { return elseStmt; }
};

class CaseBlockNode : public AST
{
private:
    IdNode *numInt; // TODO integer node?
    StmtListNode *stmtList;
    CaseBlockNode *next;
public:
    CaseBlockNode(IdNode *numInt, StmtListNode *stmtList, CaseBlockNode *caseBlock);
    virtual ~CaseBlockNode();

    inline IdNode *getNum() { return numInt; }
    inline StmtListNode *getStmtList() { return stmtList; }
    inline CaseBlockNode *getCaseBlock() { return next; }
};

class ExprListNode : public AST
{
private:
    ExprAssignNode *exprAssign;
    ExprListNode *next;
public:
    ExprListNode(ExprAssignNode *exprAssign, ExprListNode *exprList);
    virtual ~ExprListNode();

    inline ExprAssignNode *getExprAssign() { return exprAssign; }
    inline ExprListNode *getExprList() { return next; }
};

class ExprAssignNode : public AST
{
private:
    ExprOrNode *leftExpr;
    ExprOrNode *rightExpr;
    int *op;//operator
public:
    ExprAssignNode(int *op, ExprOrNode *leftExpr, ExprOrNode *rightExpr);
    virtual ~ExprAssignNode();

    inline ExprOrNode *getLeftExpr() { return leftExpr; }
    inline ExprOrNode *getRightExpr() { return rightExpr; }
    inline int *getOperator() { return op; }
};

class ExprOrNode : public AST
{
private:
    ExprAndNode *leftExpr;
    ExprAndNode *rightExpr;
    int *op;//operator
public:
    ExprOrNode(int *op, ExprAndNode *leftExpr, ExprAndNode *rightExpr);
    virtual ~ExprAssignNode();

    inline ExprAndNode *getLeftExpr() { return leftExpr; }
    inline ExprAndNode *getRightExpr() { return rightExpr; }
    inline int *getOperator() { return op; }
};

class ExprAndNode : public AST
{
private:
    ExprEqualityNode *leftExpr;
    ExprEqualityNode *rightExpr;
    int *op;//operator
public:
    ExprAndNode(int *op, ExprEqualityNode *leftExpr, ExprEqualityNode *rightExpr);
    virtual ~ExprAssignNode();

    inline ExprEqualityNode *getLeftExpr() { return leftExpr; }
    inline ExprEqualityNode *getRightExpr() { return rightExpr; }
    inline int *getOperator() { return op; }
};

class ExprEqualityNode : public AST
{
private:
    ExprRelationalNode *leftExpr;
    ExprRelationalNode *rightExpr;
    int *op;//operator
public:
    ExprEqualityNode(int *op, ExprRelationalNode *leftExpr, ExprRelationalNode *rightExpr);
    virtual ~ExprAssignNode();

    inline ExprRelationalNode *getLeftExpr() { return leftExpr; }
    inline ExprRelationalNode *getRightExpr() { return rightExpr; }
    inline int *getOperator() { return op; }
};

class ExprRelationalNode : public AST
{
private:
    ExprAdditiveNode *leftExpr;
    ExprAdditiveNode *rightExpr;
    int *op;//operator
public:
    ExprRelationalNode(int *op, ExprAdditiveNode *leftExpr, ExprAdditiveNode *rightExpr);
    virtual ~ExprAssignNode();

    inline ExprAdditiveNode *getLeftExpr() { return leftExpr; }
    inline ExprAdditiveNode *getRightExpr() { return rightExpr; }
    inline int *getOperator() { return op; }
};

class ExprAdditiveNode : public AST
{
private:
    ExprMultiplicativeNode *leftExpr;
    ExprMultiplicativeNode *rightExpr;
    int *op;//operator
public:
    ExprAdditiveNode(int *op, ExprMultiplicativeNode *leftExpr, ExprMultiplicativeNode *rightExpr);
    virtual ~ExprAssignNode();

    inline ExprMultiplicativeNode *getLeftExpr() { return leftExpr; }
    inline ExprMultiplicativeNode *getRightExpr() { return rightExpr; }
    inline int *getOperator() { return op; }
};

class ExprMultiplicativeNode : public AST
{
private:
    ExprUnaryNode *leftExpr;
    ExprUnaryNode *rightExpr;
    int *op;//operator
public:
    ExprMultiplicativeNode(int *op, ExprUnaryNode *leftExpr, ExprUnaryNode *rightExpr);
    virtual ~ExprAssignNode();

    inline ExprUnaryNode *getLeftExpr() { return leftExpr; }
    inline ExprUnaryNode *getRightExpr() { return rightExpr; }
    inline int *getOperator() { return op; }
};

class ExprUnaryNode : public AST
{
private:
    PostFixExprNode *expr;
    int *op;//operator
public:
    ExprUnaryNode(int *op, ExprUnaryNode *expr);
    virtual ~ExprAssignNode();

    inline PostFixExprNode *getExpr() { return expr; }
    inline int *getOperator() { return op; }
};

class PrimaryNode : public AST
{
private:
    IdNode *token;
    ExprAssignNode *exprAssign;
public:
    PrimaryNode(IdNode *token, ExprAssignNode *exprAssign);
    virtual ~PrimaryNode();

    inline IdNode *getToken() { return token; }
    inline ExprAssignNode *getExpr() { return exprAssign; }
};

class PostFixExprNode : public AST
{
private:
    PrimaryNode *primary;
    IdNode *token;
    ExprAssignNode *exprAssign;
    ExprListNode *exprList;
    PostFixExprNode *next;
public:
    PostFixExprNode();
    virtual ~PostFixExprNode();

    inline PrimaryNode *getPrimary() { return primary; }
    inline IdNode *getToken() { return token; }
    inline ExprAssignNode *getExprAssign() { return exprAssign; }
    inline ExprListNode *getExprList() { return exprList; }
    inline PostFixExprNode *getPostFixExpr() { return next; }

};

class BreakNode : public AST
{
private:
public:
};

class PrintNode : public AST
{
private:
    ExprListNode *exprList;
public:
    PrintNode(ExprListNode *exprList);
    virtual ~PrintNode();

    inline ExprListNode* getExprList(){ return exprList;}
};

class ReadlnNode : public AST
{
private:
    ExprAssignNode *exprAssign;
public:
    ReadlnNode(ExprAssignNode *exprAssign);
    virtual ~ReadlnNode();
    inline ExprAssignNode* getExprAssign(){return exprAssign;}
};

class returnNode : public AST
{
private:
    ExprAssignNode *exprAssign;
public:
    returnNode(ExprAssignNode *exprAssign);
    virtual ~returnNode();

    inline ExprAssignNode* getExprAssign(){return exprAssign;}
};

class ThrowNode : public AST
{
private:
public:
};

class TryCatchNode : public AST
{
private:
    StmtNode *tryExpr;
    StmtNode *catchExpr;
public:
    TryCatchNode(StmtNode *tryExpr, StmtNode *catchExpr);
    virtual ~TryCatchNode();

    inline StmtNode* getTry(){return tryExpr;}
    inline StmtNode* getCatch(){return catchExpr;}
};

class WhileNode : public AST
{
private:
    ExprAssignNode *exprAssign;
    StmtNode *stmt;
public:
    WhileNode(ExprAssignNode *exprAssgin, StmtNode *stmt);
    virtual ~NoWhile();

    inline ExprAssignNode* getExprAssign(){return exprAssign;}
    inline StmtNode* getStmt(){return stmt;}
};

class SwitchNode : public AST
{
private:
    ExprAssignNode *exprAssign;
    CaseBlockNode *caseBlock;
public:
    SwitchNode(ExprAssignNode *exprAssign, CaseBlockNode *caseBlock);
    virtual ~NoSwitch();

    inline ExprAssignNode* getExprAssign(){return exprAssign;}
    inline CaseBlockNode* getCaseBlock(){return caseBlock;}
};

#endif //COMPILADOR_2019_3_AST_H
