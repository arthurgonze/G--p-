#ifndef COMPILADOR_2019_3_AST_H
#define COMPILADOR_2019_3_AST_H

#include "visitor_ast.h"
#include "translator.h"
// Abstracts roots
class ASTNode;

// Tokens
class TokenNode;

// PDF Nodes
class ProgramNode;

class VarDeclNode;

class IdListNode;

class FunctionListNode;

class FunctionNode;

class FormalListNode;

class TypeDeclNode;

class TypeNode;

class PointerNode;

class StmtListNode;

class StmtNode;

class IfNode;

class WhileNode;

class SwitchNode;

class BreakNode;

class PrintNode;

class ReadLnNode;

class ReturnNode;

class CaseBlockNode;

class ThrowNode;

class ExpListNode;

class TryNode;

class ExpNode;

class AssignNode;

class NameExpNode;

class PointerExpNode;

class AddressValNode;

class PointerValNode;

class ArrayNode;

class ArrayCallNode;

class CallNode;

class AdditionOPNode;

class MultiplicationOPNode;

class BooleanOPNode;

class NotNode;

class SignNode;

class VarStmtNode;

class PrimaryNode;

class ASTNode {
protected:
    int line;
public:
    virtual ~ASTNode() = default;

    inline int getLine() { return line; }

    virtual inline void setLine(int line) { this->line = line; }

    virtual void accept(VisitorAST *visitor) = 0;
};

class StmtNode : public ASTNode {
private:
    bool returnAtt;
    ASTNode *stmt;
public:
    inline ASTNode *getStmt() { return stmt; }

    inline bool isReturn() const { return returnAtt; }

    inline void setReturn(bool returnAtt) { this->returnAtt = returnAtt; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }

    virtual StmNode * accept(Translator *visitor) = 0;
};


class ExpNode : public StmtNode {
private:
    const char *lexeme;
    bool lValue, pointer;
    int type, arraySize;
    const char *typeLexeme;
public:
    inline bool isLValue() const { return lValue; }

    inline bool isPointer() const { return pointer; }

    inline int getType() const { return type; }

    inline int getArraySize() const { return arraySize; }

    inline const char *getTypeLexeme() const { return typeLexeme; }

    inline const char *getLexeme() const { return lexeme; }

    inline void setLValue(bool lValue) { this->lValue = lValue; }

    inline void setPointer(bool pointer) { this->pointer = pointer; }

    inline void setType(int type) { this->type = type; }

    inline void setArraySize(int arraySize) { this->arraySize = arraySize; }

    inline void setTypeLexeme(const char *typeLexeme) { this->typeLexeme = typeLexeme; }

    inline void setLexeme(const char *lexeme) { this->lexeme = lexeme; }

    void accept(VisitorAST *visitor) override = 0;
    virtual ExprNode * accept(Translator *visitor) = 0;
};

class BreakNode : public StmtNode {
private:
public:
    BreakNode() = default;

    ~BreakNode() override = default;

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline StmNode * accept(Translator *visitor) override { return visitor->visit(this); }
};

class ThrowNode : public StmtNode {
public:
    ThrowNode() = default;

    ~ThrowNode() override = default;

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline StmNode * accept(Translator *visitor) override { return visitor->visit(this); }
};

class PointerNode : public ASTNode {
public:
    explicit PointerNode() = default;

    ~PointerNode() override = default;

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline void accept(Translator *visitor) { visitor->visit(this); }
};

class TokenNode : public ExpNode {
private:
    int token;
    int size;
public:
    TokenNode(int tok, const char *lex);

    inline int getToken() { return token; }

    inline int getSize() const { return size; }

    inline void setToken(int token) { this->token = token; }

    inline void setSize(int size) { this->size = size; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline ExprNode * accept(Translator *visitor) override { return visitor->visit(this); };
};

class ProgramNode : public ASTNode {
private:
    FunctionListNode *functions;
    TypeDeclNode *typelist;
    VarDeclNode *varlist;
public:
    ProgramNode(FunctionListNode *functions, TypeDeclNode *typelist, VarDeclNode *varlist);

    ~ProgramNode() override;

    inline FunctionListNode *getFunctions() { return functions; }

    inline TypeDeclNode *getTypeList() { return typelist; }

    inline VarDeclNode *getVarList() { return varlist; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline StmNode * accept(Translator *visitor)  { visitor->visit(this); }
};

class ExpListNode : public ASTNode {
private:
    ExpNode *exp;
    ExpListNode *next;
public:
    ExpListNode(ExpNode *exp, ExpListNode *next);

    ~ExpListNode() override;

    inline ExpNode *getExp() { return exp; }

    inline ExpListNode *getNext() { return next; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline void accept(Translator *visitor)  { visitor->visit(this); }
};

class PrintNode : public StmtNode {
private:
    ExpListNode *list;
public:
    explicit PrintNode(ExpListNode *list) { this->list = list; }

    ~PrintNode() override { delete this->list; }

    inline ExpListNode *getExpList() { return list; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline StmNode * accept(Translator *visitor) override { visitor->visit(this); }
};

class ReadLnNode : public StmtNode {
private:
    ExpNode *exp;
public:
    explicit ReadLnNode(ExpNode *exp) { this->exp = exp; }

    ~ReadLnNode() override { delete this->exp; }

    inline ExpNode *getExp() { return exp; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline StmNode * accept(Translator *visitor) override { visitor->visit(this); }
};


class TryNode : public StmtNode {
private:
    StmtNode *tryStmt;
    StmtNode *catchStmt;
public:
    TryNode(StmtNode *tryStmt, StmtNode *catchStmt);

    ~TryNode() override;

    inline StmtNode *getTry() { return tryStmt; }

    inline StmtNode *getCatch() { return catchStmt; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline StmNode * accept(Translator *visitor) override { visitor->visit(this); }
};


class ReturnNode : public StmtNode {
private:
    ExpNode *exp;
public:
    explicit ReturnNode(ExpNode *exp) { this->exp = exp; }

    ~ReturnNode() override { delete this->exp; }

    inline ExpNode *getExp() { return exp; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline StmNode * accept(Translator *visitor) override { visitor->visit(this); }
};

class StmtListNode : public StmtNode {
private:
    bool returnAtt;
    StmtNode *stmt;
    StmtListNode *next;
public :
    StmtListNode(StmtNode *stmt, StmtListNode *next);

    ~StmtListNode() override;

    inline StmtNode *getStmt() { return stmt; }

    inline StmtListNode *getNext() { return next; }

    inline bool isReturn() const { return returnAtt; }

    inline void setReturn(bool returnAtt) { this->returnAtt = returnAtt; }

    inline void setStmt(StmtNode *stmt) { this->stmt = stmt; }

    inline void setNext(StmtListNode *next) { this->next = next; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline StmNode * accept(Translator *visitor) override { visitor->visit(this); }
};

class WhileNode : public StmtNode {
private:
    ExpNode *head;
    StmtNode *body;
public:
    WhileNode(ExpNode *head, StmtNode *body);

    ~WhileNode() override;

    inline ExpNode *getHead() { return head; }

    inline StmtNode *getBody() { return body; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline StmNode * accept(Translator *visitor) override { visitor->visit(this); }
};

class IfNode : public StmtNode {
private:
    ExpNode *head;
    StmtNode *trueStmt;
    StmtNode *falseStmt;
public:
    IfNode(ExpNode *head, StmtNode *trueStmt, StmtNode *falseStmt);

    ~IfNode() override;

    inline ExpNode *getHead() { return head; }

    inline StmtNode *getTrueStmt() { return trueStmt; }

    inline StmtNode *getFalseStmt() { return falseStmt; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline StmNode * accept(Translator *visitor) override { visitor->visit(this); }
};

class CaseBlockNode : public ASTNode {
private:
    TokenNode *num;
    StmtListNode *stmt;
    CaseBlockNode *next;
public:
    CaseBlockNode(TokenNode *num, StmtListNode *stmt, CaseBlockNode *next);

    ~CaseBlockNode() override;

    inline TokenNode *getNum() { return num; }

    inline StmtListNode *getStmt() { return stmt; }

    inline CaseBlockNode *getNext() { return next; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline void accept(Translator *visitor)  { visitor->visit(this); }
};

class SwitchNode : public StmtNode {
private:
    ExpNode *exp;
    CaseBlockNode *block;
public:
    SwitchNode(ExpNode *exp, CaseBlockNode *block);

    ~SwitchNode() override;

    inline ExpNode *getExp() { return exp; }

    inline CaseBlockNode *getBlock() { return block; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline StmNode * accept(Translator *visitor) override { visitor->visit(this); }
};

class TypeNode : public ASTNode {
private:
    TokenNode *id;
    const char *lexeme;
public:
    explicit TypeNode(TokenNode *id) { this->id = id; }

    TypeNode(TokenNode *id, const char *lexeme) : id(id), lexeme(lexeme) {}

    ~TypeNode() override { delete this->id; }

    inline TokenNode *getId() { return id; }

    inline const char *getLexeme() const { return lexeme; }

    inline int getType() { return id->getToken(); }

    inline const char *getTypeLexeme() { return id->getTypeLexeme(); }

    inline void setId(TokenNode *id) { this->id = id; }

    inline void setLexeme(const char *lexeme) { this->lexeme = lexeme; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline void accept(Translator *visitor)  { visitor->visit(this); }
};

class ArrayNode : public ExpNode {
private:
    TokenNode *numInt;
public:
    explicit ArrayNode(TokenNode *numInt) {
        this->numInt = numInt;
        if (numInt) setLexeme(numInt->getLexeme());
    }

    ~ArrayNode() override { delete this->numInt; }

    inline TokenNode *getNumInt() { return numInt; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline ExprNode* accept(Translator *visitor) override { visitor->visit(this); }
};

class IdListNode : public ASTNode {
private:
    PointerNode *pointer;
    TokenNode *id;
    ArrayNode *array;
    IdListNode *next;
public:
    IdListNode(PointerNode *pointer, TokenNode *id, ArrayNode *array, IdListNode *next);

    ~IdListNode() override;

    inline PointerNode *getPointer() { return pointer; }

    inline TokenNode *getId() { return id; }

    inline ArrayNode *getArray() { return array; }

    inline IdListNode *getNext() { return next; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline void accept(Translator *visitor)  { visitor->visit(this); }
};

class VarDeclNode : public ASTNode {
private:
    TypeNode *type;
    IdListNode *idlist;
    VarDeclNode *next;
public:
    VarDeclNode(TypeNode *type, IdListNode *idlist, VarDeclNode *next);

    ~VarDeclNode() override;

    inline TypeNode *getType() { return type; }

    inline IdListNode *getIdList() { return idlist; }

    inline VarDeclNode *getNext() { return next; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline void accept(Translator *visitor)  { visitor->visit(this); }
};

class TypeDeclNode : public ASTNode {
private:
    VarDeclNode *dec;
    TokenNode *id;
    TypeDeclNode *next;
public:
    TypeDeclNode(VarDeclNode *dec, TokenNode *id, TypeDeclNode *next);

    ~TypeDeclNode() override;

    inline VarDeclNode *getDecl() { return dec; }

    inline TokenNode *getId() { return id; }

    inline TypeDeclNode *getNext() { return next; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline void accept(Translator *visitor)  { visitor->visit(this); }
};

class FormalListNode : public ASTNode {
private:
    TypeNode *type;
    PointerNode *pointer;
    TokenNode *id;
    ArrayNode *array;
    FormalListNode *next;
public:
    FormalListNode(TypeNode *type, PointerNode *pointer, TokenNode *id, ArrayNode *array, FormalListNode *next);

    ~FormalListNode() override;

    inline TypeNode *getType() { return type; }

    inline PointerNode *getPointer() { return pointer; }

    inline TokenNode *getId() { return id; }

    inline ArrayNode *getArray() { return array; }

    inline FormalListNode *getNext() { return next; }

    inline int getParameterType() { return type->getType(); }

    inline const char *getParameterTypeLexeme() { return type->getTypeLexeme(); }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline void accept(Translator *visitor)  { visitor->visit(this); }
};

class CallNode : public ExpNode {
private:
    TokenNode *id;
    ExpListNode *parameters;
public:
    CallNode(TokenNode *id, ExpListNode *parameters);

    ~CallNode() override;

    inline TokenNode *getId() { return id; }

    inline ExpListNode *getParameters() { return parameters; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline ExprNode* accept(Translator *visitor) override { visitor->visit(this); }
};

class PrimaryNode : public ExpNode {
private:
    TokenNode *token;
    ExpNode *exp;
public:
    explicit PrimaryNode(TokenNode *token);

    explicit PrimaryNode(ExpNode *exp);

    ~PrimaryNode() override;

    inline TokenNode *getTokenNode() { return token; }

    inline ExpNode *getExp() { return exp; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline ExprNode* accept(Translator *visitor) override { visitor->visit(this); }
};

class AssignNode : public ExpNode {
private:
    ExpNode *exp1;
    ExpNode *exp2;
public:
    AssignNode(ExpNode *exp1, ExpNode *exp2);

    ~AssignNode() override;

    inline ExpNode *getExp1() { return exp1; }

    inline ExpNode *getExp2() { return exp2; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline ExprNode* accept(Translator *visitor) override { visitor->visit(this); }
};

class BooleanOPNode : public ExpNode {
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

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline ExprNode* accept(Translator *visitor) override { visitor->visit(this); }
};

class SignNode : public ExpNode {
private:
    ExpNode *exp;
public:
    explicit SignNode(ExpNode *exp);

    ~SignNode() override { delete this->exp; }

    inline ExpNode *getExp() { return exp; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline ExprNode* accept(Translator *visitor) override { visitor->visit(this); }
};

class AddressValNode : public ExpNode {
private:
    ExpNode *exp;
public:
    explicit AddressValNode(ExpNode *exp);

    ~AddressValNode() override { delete this->exp; }

    inline ExpNode *getExp() { return exp; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline ExprNode* accept(Translator *visitor) override { visitor->visit(this); }
};

class PointerValNode : public ExpNode {
private:
    ExpNode *exp;
public:
    explicit PointerValNode(ExpNode *exp);

    ~PointerValNode() override { delete this->exp; }

    inline ExpNode *getExp() { return exp; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline ExprNode* accept(Translator *visitor) override { visitor->visit(this); }
};

class NotNode : public ExpNode {
private:
    ExpNode *exp;
public:
    explicit NotNode(ExpNode *exp);

    ~NotNode() override { delete this->exp; }

    inline ExpNode *getExp() { return exp; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline ExprNode* accept(Translator *visitor) override { visitor->visit(this); }
};

class NameExpNode : public ExpNode {
private:
    ExpNode *exp;
    TokenNode *id;
public:
    NameExpNode(ExpNode *exp, TokenNode *id);

    ~NameExpNode() override;

    inline ExpNode *getExp() { return exp; }

    inline TokenNode *getId() { return id; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline ExprNode* accept(Translator *visitor) override { visitor->visit(this); }
};

class PointerExpNode : public ExpNode {
private:
    ExpNode *exp;
    TokenNode *id;
public:
    PointerExpNode(ExpNode *exp, TokenNode *id);

    ~PointerExpNode() override;

    inline ExpNode *getExp() { return exp; }

    inline TokenNode *getId() { return id; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline ExprNode* accept(Translator *visitor) override { visitor->visit(this); }
};

class ArrayCallNode : public ExpNode {
private:
    ExpNode *exp;
    ExpNode *index;
public:
    ArrayCallNode(ExpNode *exp, ExpNode *index);

    ~ArrayCallNode() override;

    inline ExpNode *getExp() { return exp; }

    inline ExpNode *getIndex() { return index; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline ExprNode* accept(Translator *visitor) override { visitor->visit(this); }
};

class AdditionOPNode : public ExpNode {
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

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline ExprNode* accept(Translator *visitor) override { visitor->visit(this); }
};

class MultiplicationOPNode : public ExpNode {
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

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline ExprNode* accept(Translator *visitor) override { visitor->visit(this); }
};

class VarStmtNode : public ASTNode {
private:
    VarDeclNode *dec;
    StmtListNode *body;
public:
    VarStmtNode(VarDeclNode *dec, StmtListNode *body);

    ~VarStmtNode() override;

    inline VarDeclNode *getDecl() { return dec; }

    inline StmtListNode *getBody() { return body; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline void accept(Translator *visitor)  { visitor->visit(this); }
};

class FunctionNode : public ASTNode {
private:
    TypeNode *type;
    PointerNode *pointer;
    TokenNode *id;
    FormalListNode *parameters;
    VarDeclNode *local;
    StmtListNode *body;
public:
    FunctionNode(TypeNode *type, PointerNode *pointer, TokenNode *id, FormalListNode *parameters, VarDeclNode *local,
                 StmtListNode *body);

    ~FunctionNode() override;

    inline TypeNode *getType() { return type; }

    inline PointerNode *getPointer() { return pointer; }

    inline TokenNode *getId() { return id; }

    inline FormalListNode *getParameters() { return parameters; }

    inline VarDeclNode *getLocal() { return local; }

    inline StmtListNode *getBody() { return body; }

    inline int getReturnType() { return type->getType(); }

    inline const char *getReturnTypeLexeme() { return type->getTypeLexeme(); }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline void accept(Translator *visitor)  { visitor->visit(this); }
};

class FunctionListNode : public ASTNode {
private:
    FunctionNode *function;
    FunctionListNode *next;
public:
    FunctionListNode(FunctionNode *function, FunctionListNode *next);

    ~FunctionListNode() override;

    inline FunctionNode *getFunction() { return function; }

    inline FunctionListNode *getNext() { return next; }

    inline void accept(VisitorAST *visitor) override { visitor->visit(this); }
    inline void accept(Translator *visitor)  { visitor->visit(this); }
};

#endif //COMPILADOR_2019_3_AST_H