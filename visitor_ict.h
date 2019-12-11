#ifndef COMPILADOR_2019_3_VISITOR_IR_H
#define COMPILADOR_2019_3_VISITOR_IR_H


//ICT NODES
class ICTNode;

class ExprNode;

class ExprNode;

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

class VisitorICT {
public:
    virtual ~VisitorICT() = default;

    virtual void visit(ICTNode *node) = 0;

    virtual void visit(ExprNode *node) = 0;

    virtual void visit(ExprNode *node) = 0;

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

};

class PrintICT : public VisitorICT {
private:
    unsigned int level;

    void printIR(const char *node_name);

    void printLexemeIR(const char *node_name, const char *aux);

    void up_level();

    void down_level();

public:
    PrintICT(ICTNode *node);

    virtual ~PrintICT();

    void visit(ICTNode *node) override;

    void visit(ExprNode *node) override;

    void visit(ExprNode *node) override;

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


#endif //COMPILADOR_2019_3_VISITOR_IR_H
