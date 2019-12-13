#ifndef COMPILADOR_2019_3_VISITOR_IR_H
#define COMPILADOR_2019_3_VISITOR_IR_H


#include <iostream>
//ICT NODES
class ICTNode;

class ExprNode;

class StmNode;

class CONST;

class CONSTF;

class NAME;

class TEMP;

class BINOP;

class MEM;

class CALL;

class ESEQ;

class MOVE;

class EXP;

class JUMP;

class CJUMP;

class SEQ;

class LABEL;

class ExpList;

class StmList;

class Fragment;

class Procedure;

class Variable;

class Literal;

class VisitorICT {
public:
    virtual ~VisitorICT() = default;

    virtual void visit(ICTNode *node) = 0;

    virtual void visit(ExprNode *node) = 0;

    virtual void visit(StmNode *node) = 0;

    virtual void visit(CONST *node) = 0;

    virtual void visit(CONSTF *node) = 0;

    virtual void visit(NAME *node) = 0;

    virtual void visit(TEMP *node) = 0;

    virtual void visit(BINOP *node) = 0;

    virtual void visit(MEM *node) = 0;

    virtual void visit(CALL *node) = 0;

    virtual void visit(ESEQ *node) = 0;

    virtual void visit(MOVE *node) = 0;

    virtual void visit(EXP *node) = 0;

    virtual void visit(JUMP *node) = 0;

    virtual void visit(CJUMP *node) = 0;

    virtual void visit(SEQ *node) = 0;

    virtual void visit(LABEL *node) = 0;

    virtual void visit(ExpList *node) = 0;

    virtual void visit(StmList *node) = 0;

    virtual void visit(Procedure *node) = 0;

    virtual void visit(Literal *node) = 0;

    virtual void visit(Variable *node) = 0;

    virtual void visit(Fragment *node) = 0;



};

class PrintICT : public VisitorICT {
private:
    unsigned int level;

    void printIR(const char *node_name);

    void printLexemeIR(const char *node_name, const char *aux);

    void up_level();

    void down_level();

public:
    PrintICT();

    virtual ~PrintICT();

    void visit(Fragment *node) override;

    void visit(Procedure *node) override;

    void visit(Literal *node) override;

    void visit(Variable *node) override;


    void visit(ICTNode *node) override;

    void visit(ExprNode *node) override;

    void visit(StmNode *node) override;

    void visit(CONST *node) override;

    void visit(CONSTF *node) override;

    void visit(NAME *node) override;

    void visit(TEMP *node) override;

    void visit(BINOP *node) override;

    void visit(MEM *node) override;

    void visit(CALL *node) override;

    void visit(ESEQ *node) override;

    void visit(MOVE *node) override;

    void visit(EXP *node) override;

    void visit(JUMP *node) override;

    void visit(CJUMP *node) override;

    void visit(SEQ *node) override;

    void visit(LABEL *node) override;

    void visit(ExpList *node) override;

    void visit(StmList *node) override;

};


class VisitorCICT {
public:
    virtual ~VisitorCICT() = default;

    virtual Fragment * visit(Fragment * fragment) = 0;

    virtual Procedure *visit(Procedure *fragment) =0;

    virtual Literal *visit(Literal *fragment) = 0;

    virtual Variable *visit(Variable *fragment) = 0;

    virtual StmNode * visit(StmList *node) = 0;

    virtual StmNode * visit(ExprNode *node) = 0;

    virtual StmNode * visit(MOVE *node) = 0;

    virtual StmNode * visit(JUMP *node) = 0;

    virtual StmNode * visit(CJUMP *node) = 0;

    virtual StmNode * visit(SEQ *node) = 0;

    virtual StmNode * visit(LABEL *node) = 0;

    virtual StmNode * visit(EXP *node) = 0;

    virtual ExprNode * visit(CONST *node) = 0;

    virtual ExprNode * visit(CONSTF *node) = 0;

    virtual ExprNode * visit(NAME *node) = 0;

    virtual ExprNode * visit(TEMP *node) = 0;

    virtual ExprNode * visit(BINOP *node) = 0;

    virtual ExprNode * visit(MEM *node) = 0;

    virtual ExprNode * visit(CALL *node) = 0;

    virtual ExprNode * visit(ESEQ *node) = 0;

    virtual ExprNode * visit(ExpList *node) = 0;

};


class Canonization : public VisitorCICT{
private:

public:
    Canonization();

    virtual ~Canonization();

    Fragment * visit(Fragment * fragment) override ;

    Procedure *visit(Procedure *fragment) override;

    Literal *visit(Literal *fragment) override;

    Variable *visit(Variable *fragment) override;

    StmNode * visit(StmList *node) override;

    StmNode * visit(ExprNode *node) override;

    StmNode * visit(MOVE *node) override;

    StmNode * visit(JUMP *node) override;

    StmNode * visit(CJUMP *node) override;

    StmNode * visit(SEQ *node) override;

    StmNode * visit(LABEL *node) override;

    StmNode * visit(EXP *node) override;

    ExprNode * visit(CONST *node) override;

    ExprNode * visit(CONSTF *node) override;

    ExprNode * visit(NAME *node) override;

    ExprNode * visit(TEMP *node) override;

    ExprNode * visit(BINOP *node) override;

    ExprNode * visit(MEM *node) override;

    ExprNode * visit(CALL *node) override;

    ExprNode * visit(ESEQ *node) override;

    ExprNode * visit(ExpList *node) override;

};

#endif //COMPILADOR_2019_3_VISITOR_IR_H
