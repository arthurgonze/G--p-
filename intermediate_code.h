#ifndef COMPILADOR_2019_3_INTERMEDIATE_CODE_H
#define COMPILADOR_2019_3_INTERMEDIATE_CODE_H

#include <cstdio>
#include <cstring>
#include "ict.h"
#include "token.h"

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



class Fragment {
private:
    Fragment *next;
public:
    Fragment();

    virtual ~Fragment();

    inline Fragment *getNext() const { return next; }
};


/**
 * Objeto com as informações necessárias para a ativação do procedimento. Deve conter:
 * a localização de cada parâmetro (no frame ou em um registrador),
 * número de variáveis locais alocados até o momento,
 * o rótulo no qual o código de máquina do procedimento começa,
 * e as instruções necessárias para realizar o deslocamento do frame.
 */
class Frame {
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
    ExprNode *body;
public:
    Procedure(Frame *frame, ExprNode *body);

    ~Procedure() override = 0;

    inline Frame *getFrame() const { return frame; }

    inline ExprNode *getBody() const { return body; }
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
    virtual ExprNode * accessCode()= 0; // retorna o código de máquina p/ acessar o nome
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
    virtual ~InFrame() = 0;

    inline int getOffset() const { return offset; }

    ExprNode *accessCode() override;
};

class InReg : public LocalAccess {
private:
    Temp *temp;
    //TODO ...
public:
    explicit InReg(Temp *temp);
    virtual ~InReg()=0;

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
public:
    FrameMIPS(Label* label, Temp* returnValue, AccessList *localData);

    ~FrameMIPS();

    // Essas funções decidem se o dado estará em um registrador ou no frema.
    // uma variável escapa, ela tem que ser colocada no frame.
    LocalAccess *addParam(bool escape, int bytesSize) override;

    LocalAccess *addLocal(bool escape, int bytesSize) override;

    inline Label *getLabel() const { return label; }

    inline Temp *getReturnValue() const { return returnValue; }

    inline AccessList *getLocalData() const { return localData; }

};


#endif //COMPILADOR_2019_3_INTERMEDIATE_CODE_H
