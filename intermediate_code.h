#ifndef COMPILADOR_2019_3_INTERMEDIATE_CODE_H
#define COMPILADOR_2019_3_INTERMEDIATE_CODE_H

#include <cstdio>
#include <cstring>
#include "token.h"
#include "visitor_ict.h"

//STM_NODES
#define V_CONST 0
#define V_CONSTF 1
#define V_NAME 2
#define V_TEMP 3
#define V_BINOP 4
#define V_MEM 5
#define V_CALL 6
#define V_ESEQ 7

//EXP_NODES
#define V_MOVE 8
#define V_EXP 9
#define V_JUMP 10
#define V_CJUMP 11
#define V_SEQ 12
#define V_LABEL 13

//LIST_NODES
#define V_STM_LIST 14
#define V_EXP_LIST 15



extern int num_labels;
extern int num_temps;

//forward declarations
class LocalAccess;

class Fragment;

class Frame;

class Procedure;

class Literal;

class Variable;

class Temp;

class TempList;

class Label;

class LabelList;

class LocalAccess;

class AccessList;

class InReg;

class InFrame;

class FrameMIPS;

class StmNode;

class ExprNode;

class Fragment {
private:
    Fragment *next;
public:
    Fragment();

    virtual ~Fragment();

    inline Fragment *getNext() const { return next; }

    inline void setNext(Fragment *next) { this->next = next; }

    virtual void accept(PrintICT *visitor) = 0;

    virtual Fragment *accept(Canonicalizer *visitor) = 0;

};

/**
 * Objeto com as informações necessárias para a ativação do procedimento. Deve conter:
 * a localização de cada parâmetro (no frame ou em um registrador),
 * número de variáveis locais alocados até o momento,
 * o rótulo no qual o código de máquina do procedimento começa,
 * e as instruções necessárias para realizar o deslocamento do frame.
 */
class Frame {
private:
    // O frame decide se o dado local estará em um registrador ou em um temporário
    // A classe Frame deve ser abstrata porque seu layout e sua estrutura variam de máquina para máquina
public:
    virtual LocalAccess *addParam(bool escape, int bytesSize)= 0;

    virtual LocalAccess *addLocal(bool escape, int bytesSize)= 0;
};

/**
 * Determina o frame e o corpo de um procedimento (qualquer procedimento
 * ou função declarado e o programa principal).
 */
class Procedure : public Fragment {
private:
    Frame *frame;
    StmNode *body;
public:
    Procedure(Frame *frame, StmNode *body);

    ~Procedure() override;

    inline Frame *getFrame() const { return frame; }

    inline StmNode *getBody() const { return body; }

    void setBody(StmNode *body);

    inline void accept(PrintICT *visitor) override { return visitor->visit(this); }

    inline Procedure *accept(Canonicalizer *visitor) override { return visitor->visit(this); }

};

/**
 * Uma string literal deve ter seu espaço alocado pelo compilador.
 * O acesso a ela deverá ser feito através de um rótulo que será gerado automaticamente na construção desse objeto.
 * Constantes inteiras e reais serão representadas diretamente no código alvo
 */
class Literal : public Fragment {
private:
    const char *literal;
public:
    explicit Literal(const char *literal);

    ~Literal() override;

    inline const char *getLiteral() const { return literal; }

    inline void accept(PrintICT *visitor) override { return visitor->visit(this); }

    inline Literal *accept(Canonicalizer *visitor) override { return visitor->visit(this); }

};

/**
 * O parâmetro nbytes indica o tamanho em bytes do tipo que pode ser primitivo ou composto.
 * O acesso a dado deverá ser feito através de um rótulo cujo nome é
 * exatamente o da variável correspondente no código fonte
 */
class Variable : public Fragment {
private:
    int type;
    int nbytes;
public:
    Variable(int type, int nbytes);

    ~Variable() override;

    inline int getType() const { return type; }

    inline int getNbytes() const { return nbytes; }

    inline void accept(PrintICT *visitor) override { return visitor->visit(this); }

    inline Variable *accept(Canonicalizer *visitor) override { return visitor->visit(this); }

};

/**
 * Temp é um nome abstrato para variáveis locais
 * Um new Temp() deve retornar um novo temporário de um conjunto infinito de temporários.
 * Um new Temp(“r”) deve retornar um temporário associado ao registrador r específico da máquina
 */
class Temp {
private:
    char *name;
public:
    Temp();

    explicit Temp(const char *name);

    virtual ~Temp();

    inline char *getName() const { return name; }

    inline char *toString();
};

class TempList {
private:
    Temp *temp;
    TempList *next;
public:
    TempList(Temp *temp, TempList *next);

    virtual ~TempList();

    inline Temp *getTemp() const { return temp; }

    inline TempList *getNext() const { return next; }
};

/**
 * Label é um nome abstrato para endereços estáticos de memória (dados ou código)
 * Um new Label() deve retornar um novo rótulo de um conjunto infinito de rótulos.
 * Um new Label(s) deve retornar um novo rótulo cujo nome em linguagem assembly é s
 */
class Label {
private:
    char *name;
public:
    Label();

    explicit Label(const char *name);

    virtual ~Label();

    inline char *getName() const { return name; }
};

class LabelList {
private:
    Label *label;
    LabelList *next;
public:
    LabelList(Label *label, LabelList *next);

    virtual ~LabelList();

    inline Label *getLabel() const { return label; }

    inline LabelList *getNext() const { return next; }
};

/**
 * A classe LocalAccess indica como um dado local é acessado.
 * Ela deve gerar a seqüência de instruções necessárias para acessar o nome.
 * Cada parâmetro e variável local deve ter seu objeto LocalAccess alocado no frame
 */
class LocalAccess {
public:
    virtual ExprNode *accessCode()= 0; // retorna o código de máquina p/ acessar o nome
};

class AccessList {
private:
    LocalAccess *local;
    AccessList *next;
public:
    AccessList(LocalAccess *local, AccessList *next);

    virtual ~AccessList();

    inline LocalAccess *getLocal() const { return local; }

    inline AccessList *getNext() const { return next; }
};

class InFrame : public LocalAccess {
private:
    int offset;
    //TODO ...
public:
    explicit InFrame(int offset);

    virtual ~InFrame() = default;

    inline int getOffset() const { return offset; }

    ExprNode *accessCode() override;
};

class InReg : public LocalAccess {
private:
    Temp *temp;
    //TODO ...
public:
    explicit InReg(Temp *temp);

    virtual ~InReg();

    inline Temp *getTemp() const { return temp; }

    ExprNode *accessCode() override;
};

extern Temp *FP; // Temp único que representa o registrador FP (ponteiro do frame)
extern Temp *RV; // Temp único que representa o registrador RV (retorno de função)

class FrameMIPS : public Frame {
private:
    Label *label;// Rótulo para o inicio do código do procedimento (nome)
    Temp *returnValue;// Temporario que contém o valor de retorno da função
    AccessList *localData;// Lista de acessos locais (parâmetros e variáveis locais)
    int frameSize, regSize, paramSize;
public:
    FrameMIPS(Label *label, Temp *returnValue, AccessList *localData);

    ~FrameMIPS();

    // Essas funções decidem se o dado estará em um registrador ou no frema.
    // uma variável escapa, ela tem que ser colocada no frame.
    LocalAccess *addParam(bool escape, int bytesSize) override;

    LocalAccess *addLocal(bool escape, int bytesSize) override;

    inline Label *getLabel() const { return label; }

    inline Temp *getReturnValue() const { return returnValue; }

    inline AccessList *getLocalData() const { return localData; }

    inline void setLabel(Label *label) { FrameMIPS::label = label; }

    inline void setReturnValue(Temp *returnValue) { FrameMIPS::returnValue = returnValue; }

    inline void setLocalData(AccessList *localData) { FrameMIPS::localData = localData; }

    inline void setFrameSize(int frameSize) { FrameMIPS::frameSize = frameSize; }

    inline void setRegSize(int regSize) { FrameMIPS::regSize = regSize; }

    inline void setParamSize(int paramSize) { FrameMIPS::paramSize = paramSize; }

};


/*********************ICT**************************/
class ICTNode {
private:
public:
    virtual ~ICTNode() = default;

    virtual void accept(VisitorICT *visitor) = 0;
};

/**
 * Contém o código intermediário relativo ao procedimento (sem prólogo e epílogo para a ativação)
 */
class StmNode : public ICTNode {
private:
    int typeStm;

public:
    ~StmNode() override = default;

    void accept(VisitorICT *visitor) override = 0;

    virtual StmNode *accept(Canonicalizer *visitor)  = 0;

    int getTypeStm() const;

    void setTypeStm(int typeStm);

    virtual inline StmNode * getS() {return nullptr ;}

    virtual inline ExprNode * getE() {return nullptr ;}

    virtual inline StmNode * getS1() {return nullptr ;}

    virtual inline StmNode * getS2() {return nullptr ;}

};

class ExprNode : public StmNode {
private:
public:
    ~ExprNode() override = default;

    void accept(VisitorICT *visitor) override = 0;

    virtual ExprNode *accept(Canonicalizer *visitor)  = 0;

};

class CONST : public ExprNode {
private:
    int i; // int constant i
public:
    explicit CONST(int i);

    ~CONST() override = default;

    inline int getI() const { return i; }

    inline void accept(VisitorICT *visitor) override { visitor->visit(this); }

    inline ExprNode *accept(Canonicalizer *visitor) override { return visitor->visit(this); }

};

class CONSTF : public ExprNode {
private:
    float j; // float constant j
public:
    explicit CONSTF(float j);

    ~CONSTF() override = default;

    inline float getJ() const { return j; }

    inline void accept(VisitorICT *visitor) override { visitor->visit(this); }

    inline ExprNode *accept(Canonicalizer *visitor) override { return visitor->visit(this); }

};

class NAME : public ExprNode {
private:
    Label *l;
public:
    explicit NAME(Label *l);

    ~NAME() override;

    inline Label *getL() const { return l; }

    inline void accept(VisitorICT *visitor) override { visitor->visit(this); }

    inline ExprNode *accept(Canonicalizer *visitor) override { return visitor->visit(this); }

};

class TEMP : public ExprNode {
private:
    Temp *t;
public:
    explicit TEMP(Temp *t);

    ~TEMP() override;

    inline Temp *getT() const { return t; }

    inline void accept(VisitorICT *visitor) override { visitor->visit(this); }

    inline ExprNode *accept(Canonicalizer *visitor) override { return visitor->visit(this); }

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

    inline void accept(VisitorICT *visitor) override { visitor->visit(this); }

    inline ExprNode *accept(Canonicalizer *visitor) override { return visitor->visit(this); }

    void setLeft(ExprNode *left);

    void setRight(ExprNode *right);

};

class MEM : public ExprNode {
private:
    ExprNode *e;
public:
    explicit MEM(ExprNode *e);

    ~MEM() override;

    inline ExprNode *getE() override  { return e; }

    inline void accept(VisitorICT *visitor) override { visitor->visit(this); }

    inline ExprNode *accept(Canonicalizer *visitor) override { return visitor->visit(this); }

    void setE(ExprNode *e);

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

    inline void accept(VisitorICT *visitor) override { visitor->visit(this); }

    inline ExprNode *accept(Canonicalizer *visitor) override { return visitor->visit(this); }

};

class ESEQ : public ExprNode {
private:
    StmNode *s;
    ExprNode *e;
public:
    ESEQ(StmNode *s, ExprNode *e);

    ~ESEQ() override;

    inline StmNode *getS() override { return s; }

    inline ExprNode *getE() override { return e; }

    inline void accept(VisitorICT *visitor) override { visitor->visit(this); }

    inline ExprNode *accept(Canonicalizer *visitor) override { return visitor->visit(this); }

    void setS(StmNode *s);

    void setE(ExprNode *e);

};

class MOVE : public StmNode {
private:
    ExprNode *dst, *src;
public:
    MOVE(ExprNode *dst, ExprNode *src);

    ~MOVE() override;

    inline ExprNode *getDst() const { return dst; }

    inline ExprNode *getSrc() const { return src; }

    inline void accept(VisitorICT *visitor) override { visitor->visit(this); }

    inline StmNode *accept(Canonicalizer *visitor) override { return visitor->visit(this); }

    void setDst(ExprNode *dst);

    void setSrc(ExprNode *src);

};

class EXP : public StmNode {
private:
    ExprNode *e;
public:
    explicit EXP(ExprNode *e);

    ~EXP() override;

    inline ExprNode *getE() override { return e; }

    inline void accept(VisitorICT *visitor) override { visitor->visit(this); }

    inline StmNode *accept(Canonicalizer *visitor) override { return visitor->visit(this); }

};

class JUMP : public StmNode {
private:
    ExprNode *e;
    LabelList *targets;
public:
    JUMP(ExprNode *e, LabelList *targets);
    JUMP(ExprNode *e);

    ~JUMP() override;

    inline ExprNode *getE() override { return e; }

    inline LabelList *getTargets() const { return targets; }

    inline void accept(VisitorICT *visitor) override { visitor->visit(this); }

    inline StmNode *accept(Canonicalizer *visitor) override { return visitor->visit(this); }

    void setE(ExprNode *e);

    void setTargets(LabelList *targets);
};

class CJUMP : public StmNode {
private:
    int relop;
    ExprNode *left, *right;
    Label *ifTrue, *ifFalse;
public:
    CJUMP(int relop, ExprNode *left, ExprNode *right, Label *ifTrue, Label *ifFalse);

    ~CJUMP() override;

    inline int getRelop() const { return relop; }

    inline ExprNode *getLeft() const { return left; }

    inline ExprNode *getRight() const { return right; }

    inline Label *getIfTrue() const { return ifTrue; }

    inline Label *getIfFalse() const { return ifFalse; }

    inline void accept(VisitorICT *visitor) override { visitor->visit(this); }

    inline StmNode *accept(Canonicalizer *visitor) override { return visitor->visit(this); }

    void setLeft(ExprNode *left);

    void setRight(ExprNode *right);

};

class SEQ : public StmNode {
private:
    StmNode *left, *right;
public:
    SEQ(StmNode *left, StmNode *right);

    ~SEQ() override;

    inline StmNode *getS1() override { return left; }

    inline StmNode *getS2() override { return right; }

    inline void accept(VisitorICT *visitor) override { visitor->visit(this); }

    inline StmNode *accept(Canonicalizer *visitor) override { return visitor->visit(this); }

    void setS1(StmNode *left);

    void setS2(StmNode *right);


};

class LABEL : public StmNode {
private:
    Label *l;
public:
    explicit LABEL(Label *l);

    ~LABEL() override;

    inline Label *getL() const { return l; }

    inline void accept(VisitorICT *visitor) override { visitor->visit(this); }

    inline StmNode *accept(Canonicalizer *visitor) override { return visitor->visit(this); }

};

// Outras classes
class ExpList : public ExprNode {
private:
    ExprNode *first;
    ExpList *next;
public:
    ExpList(ExprNode *first, ExpList *next);

    ~ExpList() override;

    inline ExprNode *getFirst() const { return first; }

    inline ExpList *getNext() const { return next; }

    inline void accept(VisitorICT *visitor) override { visitor->visit(this); }

    inline ExpList *accept(Canonicalizer *visitor) override { return visitor->visit(this); }

    void setFirst(ExprNode *first);

    void setNext(ExpList *next);

};

class StmList : public StmNode {
private:
    StmNode *first;
    StmList *next;
public:
    StmList(StmNode *first, StmList *next);

    ~StmList() override;

    inline StmNode *getFirst() const { return first; }

    inline StmList *getNext() const { return next; }

    inline void accept(VisitorICT *visitor) override { visitor->visit(this); }

    inline StmList *accept(Canonicalizer *visitor) override { return visitor->visit(this); }

    void setFirst(StmNode *first);

    void setNext(StmList *next);

};

#endif //COMPILADOR_2019_3_INTERMEDIATE_CODE_H
