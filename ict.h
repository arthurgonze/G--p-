#ifndef COMPILADOR_2019_3_ICT_H
#define COMPILADOR_2019_3_ICT_H

#include "intermediate_code.h"
#include "visitor_ir.h"

//forward declarations
class ICTNode;
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


class ICTNode {
private:
public:
    virtual ~ICTNode() = default;

    virtual void accept(VisitorIR *visitor) = 0;
};

class ExprNode : public ICTNode {
private:
public:
    void accept(VisitorIR *visitor) override = 0;
};

/**
 * Contém o código intermediário relativo ao procedimento (sem prólogo e epílogo para a ativação)
 */


class StmNode : public ICTNode {
private:
public:
    void accept(VisitorIR *visitor) override = 0;
};

class CONST : public ExprNode {
private:
    int i; // int constant i
public:
    explicit CONST(int i);

    ~CONST() override = 0;

    inline int getI() const { return i; }

    inline void accept(VisitorIR *visitor) override { visitor->visit(this); }
};

class CONSTF : public ExprNode {
private:
    float j; // float constant j
public:
    explicit CONSTF(float j);

    ~CONSTF() override = 0;

    inline float getJ() const { return j; }

    inline void accept(VisitorIR *visitor) override { visitor->visit(this); }
};

class NAME : public ExprNode {
private:
    Label *l;
public:
    explicit NAME(Label *l);

    ~NAME() override;

    inline Label *getL() const { return l; }

    inline void accept(VisitorIR *visitor) override { visitor->visit(this);}
};

class TEMP : public ExprNode {
private:
    Temp *t;
public:
    explicit TEMP(Temp *t);

    ~TEMP() override;

    inline Temp *getT() const { return t; }

    inline void accept(VisitorIR *visitor) override {  visitor->visit(this); }
};

class BINOP : public ExprNode {
private:
    int binop;
    ExprNode *left, *right;
public:
    BINOP(int binop, ExprNode *left, ExprNode *right);

    ~BINOP() override;

    inline int getBinop() const { return binop; }

    inline ExprNode *getLeft() const { return left; }

    inline ExprNode *getRight() const { return right; }

    inline void accept(VisitorIR *visitor) override { visitor->visit(this); }
};

class MEM : public ExprNode {
private:
    ExprNode *e;
public:
    explicit MEM(ExprNode *e);

    ~MEM() override;

    inline ExprNode *getE() const { return e; }

    inline void accept(VisitorIR *visitor) override { visitor->visit(this); }
};


class CALL : public ExprNode {
private:
    ExprNode *func;
    ExpList *args;
public:
    CALL(ExprNode *func, ExpList *args);

    ~CALL() override;

    inline ExprNode *getFunc() const { return func; }

    inline ExpList *getArgs() const { return args; }

    inline void accept(VisitorIR *visitor) override { visitor->visit(this);}
};

class ESEQ : public ExprNode {
private:
    StmNode *s;
    ExprNode *e;
public:
    ESEQ(StmNode *s, ExprNode *e);

    ~ESEQ() override;

    inline StmNode *getS() const { return s; }

    inline ExprNode *getE() const { return e; }

    inline void accept(VisitorIR *visitor) override { visitor->visit(this);}
};

class MOVE : public StmNode {
private:
    ExprNode *dst, *src;
public:
    MOVE(ExprNode *dst, ExprNode *src);

    ~MOVE() override;

    inline ExprNode *getDst() const { return dst; }

    inline ExprNode *getSrc() const { return src; }

    inline void accept(VisitorIR *visitor) override { visitor->visit(this);}
};

class EXP : public StmNode {
private:
    ExprNode *e;
public:
    explicit EXP(ExprNode *e);

    ~EXP() override;

    inline ExprNode *getE() const { return e; }

    inline void accept(VisitorIR *visitor) override {  visitor->visit(this);}
};

class JUMP : public StmNode {
private:
    ExprNode *e;
    LabelList *targets;
public:
    JUMP(ExprNode *e, LabelList *targets);

    ~JUMP() override;

    inline ExprNode *getE() const { return e; }

    inline LabelList *getTargets() const { return targets; }

    inline void accept(VisitorIR *visitor) override {  visitor->visit(this);}
};

class CJUMP : public StmNode {
private:
    int relop;
    ExprNode *left, *right, *ifTrue, *ifFalse;
public:
    CJUMP(int relop, ExprNode *left, ExprNode *right, ExprNode *ifTrue, ExprNode *ifFalse);

    ~CJUMP() override;

    inline int getRelop() const { return relop; }

    inline ExprNode *getLeft() const { return left; }

    inline ExprNode *getRight() const { return right; }

    inline ExprNode *getIfTrue() const { return ifTrue; }

    inline ExprNode *getIfFalse() const { return ifFalse; }

    inline void accept(VisitorIR *visitor) override { visitor->visit(this);}
};

class SEQ : public StmNode {
private:
    StmNode *left, *right;
public:
    SEQ(StmNode *left, StmNode *right);

    ~SEQ() override;

    inline StmNode *getLeft() const { return left; }

    inline StmNode *getRight() const { return right; }

    inline void accept(VisitorIR *visitor) override {  visitor->visit(this);}
};

class LABEL : public StmNode {
private:
    Label *l;
public:
    explicit LABEL(Label *l);

    ~LABEL() override;

    inline Label *getL() const { return l; }

    inline void accept(VisitorIR *visitor) override { visitor->visit(this); }
};

// Outras classes
class ExpList {
private:
    ExprNode *first;
    ExpList *next;
public:
    ExpList(ExprNode *first, ExpList *next);

    virtual ~ExpList();

    inline ExprNode *getFirst() const { return first; }

    inline ExpList *getNext() const { return next; }

    inline void accept(VisitorIR *visitor) {  visitor->visit(this); }
};

class StmList {
private:
    StmNode *first;
    StmList *next;
public:
    StmList(StmNode *first, StmList *next);

    virtual ~StmList();

    inline StmNode *getFirst() const { return first; }

    inline StmList *getNext() const { return next; }

    inline void accept(VisitorIR *visitor) {  visitor->visit(this); }
};

#endif //COMPILADOR_2019_3_ICT_H
