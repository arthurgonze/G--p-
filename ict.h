#ifndef COMPILADOR_2019_3_ICT_H
#define COMPILADOR_2019_3_ICT_H

#include "intermediate_code.h"
#include "visitor.h"

//forward declarations
class ICTNode;

class ExpNode;

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

    virtual void accept(Visitor *visitor) = 0;
};

class ExpNode : public ICTNode {
private:
public:
    void accept(Visitor *visitor) override = 0;
};

class StmNode : public ICTNode {
private:
public:
    void accept(Visitor *visitor) override = 0;
};

class CONST : public ExpNode {
private:
    int i; // int constant i
public:
    explicit CONST(int i);

    ~CONST() override = 0;

    inline int getI() const { return i; }

    inline void accept(Visitor *visitor) override { /*TODO visitor->visit(this);*/ }
};

class CONSTF : public ExpNode {
private:
    float j; // float constant j
public:
    explicit CONSTF(float j);

    ~CONSTF() override = 0;

    inline float getJ() const { return j; }

    inline void accept(Visitor *visitor) override { /*TODO visitor->visit(this);*/ }
};

class NAME : public ExpNode {
private:
    Label *l;
public:
    explicit NAME(Label *l);

    ~NAME() override;

    inline Label *getL() const { return l; }

    inline void accept(Visitor *visitor) override { /*TODO visitor->visit(this);*/ }
};

class TEMP : public ExpNode {
private:
    Temp *t;
public:
    explicit TEMP(Temp *t);

    virtual ~TEMP();

    inline Temp *getT() const { return t; }

    inline void accept(Visitor *visitor) override { /*TODO visitor->visit(this);*/ }
};

class BINOP : public ExpNode {
private:
    int binop;
    ExpNode *left, *right;
public:
    BINOP(int binop, ExpNode *left, ExpNode *right);

    ~BINOP() override;

    inline int getBinop() const { return binop; }

    inline ExpNode *getLeft() const { return left; }

    inline ExpNode *getRight() const { return right; }

    inline void accept(Visitor *visitor) override { /*TODO visitor->visit(this);*/ }
};

class MEM : public ExpNode {
private:
    ExpNode *e;
public:
    explicit MEM(ExpNode *e);

    ~MEM() override;

    inline ExpNode *getE() const { return e; }

    inline void accept(Visitor *visitor) override { /*TODO visitor->visit(this);*/ }
};


class CALL : public ExpNode {
private:
    ExpNode *func;
    ExpList *args;
public:
    CALL(ExpNode *func, ExpList *args);

    ~CALL() override;

    inline ExpNode *getFunc() const { return func; }

    inline ExpList *getArgs() const { return args; }

    inline void accept(Visitor *visitor) override { /*TODO visitor->visit(this);*/ }
};

class ESEQ : public ExpNode {
private:
    Stm *s;
    ExpNode *e;
public:
    ESEQ(Stm *s, ExpNode *e);

    ~ESEQ() override;

    inline Stm *getS() const { return s; }

    inline ExpNode *getE() const { return e; }

    inline void accept(Visitor *visitor) override { /*TODO visitor->visit(this);*/ }
};

class MOVE : public StmNode {
private:
    ExpNode *dst, *src;
public:
    MOVE(ExpNode *dst, ExpNode *src);

    ~MOVE() override;

    inline ExpNode *getDst() const { return dst; }

    inline ExpNode *getSrc() const { return src; }

    inline void accept(Visitor *visitor) override { /*TODO visitor->visit(this);*/ }
};

class EXP : public StmNode {
private:
    ExpNode *e;
public:
    explicit EXP(ExpNode *e);

    ~EXP() override;

    inline ExpNode *getE() const { return e; }

    inline void accept(Visitor *visitor) override { /*TODO visitor->visit(this);*/ }
};

class JUMP : public StmNode {
private:
    ExpNode *e;
    LabelList *targets;
public:
    JUMP(ExpNode *e, LabelList *targets);

    ~JUMP() override;

    inline ExpNode *getE() const { return e; }

    inline LabelList *getTargets() const { return targets; }

    inline void accept(Visitor *visitor) override { /*TODO visitor->visit(this);*/ }
};

class CJUMP : public StmNode {
private:
    int relop;
    ExpNode *left, *right;
    Label *ifTrue, *ifFalse;
public:
    CJUMP(int relop, ExpNode *left, ExpNode *right, Label *ifTrue, Label *ifFalse);

    ~CJUMP() override;

    inline int getRelop() const { return relop; }

    inline ExpNode *getLeft() const { return left; }

    inline ExpNode *getRight() const { return right; }

    inline Label *getIfTrue() const { return ifTrue; }

    inline Label *getIfFalse() const { return ifFalse; }

    inline void accept(Visitor *visitor) override { /*TODO visitor->visit(this);*/ }
};

class SEQ : public StmNode {
private:
    Stm *left, *right;
public:
    SEQ(Stm *left, Stm *right);

    ~SEQ() override;

    inline Stm *getLeft() const { return left; }

    inline Stm *getRight() const { return right; }

    inline void accept(Visitor *visitor) override { /*TODO visitor->visit(this);*/ }
};

class LABEL : public StmNode {
private:
    Label *l;
public:
    explicit LABEL(Label *l);

    ~LABEL() override;

    inline Label *getL() const { return l; }

    inline void accept(Visitor *visitor) override { /*TODO visitor->visit(this);*/ }
};

// Outras classes
class ExpList {
private:
    ExpNode *prim;
    ExpList *prox;
public:
    ExpList(ExpNode *prim, ExpList *prox);

    virtual ~ExpList();

    inline ExpNode *getPrim() const { return prim; }

    inline ExpList *getProx() const { return prox; }

    inline void accept(Visitor *visitor) { /*TODO visitor->visit(this);*/ }
};

class StmList {
private:
    Stm *prim;
    StmList *prox;
public:
    StmList(Stm *prim, StmList *prox);

    virtual ~StmList();

    inline Stm *getPrim() const { return prim; }

    inline StmList *getProx() const { return prox; }

    inline void accept(Visitor *visitor) { /*TODO visitor->visit(this);*/ }
};

#endif //COMPILADOR_2019_3_ICT_H
