#ifndef COMPILADOR_2019_3_INTERMEDIATE_CODE_H
#define COMPILADOR_2019_3_INTERMEDIATE_CODE_H

int num_labels = 0;
int num_temps = 0;

//forward declarations
class LocalAccess;

class Fragment;

class Frame;

class Stm;

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

class Fragment {
private:
    Fragment *next;
public:
    Fragment();

    virtual ~Fragment();

    virtual Fragment *getNext() const;
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
    virtual LocalAccess addParam(bool escape, int bytesSize)= 0;

    virtual LocalAccess addLocal(bool escape, int bytesSize)= 0;
};

/**
 * Contém o código intermediário relativo ao procedimento (sem prólogo e epílogo para a ativação)
 */
class Stm {

};

/**
 * Determina o frame e o corpo de um procedimento (qualquer procedimento
 * ou função declarado e o programa principal).
 */
class Procedure : public Fragment {
private:
    Frame *frame;
    Stm *body;
public:
    Procedure(Frame *frame, Stm *body);

    ~Procedure() override;

    Frame *getFrame() const;

    Stm *getBody() const;

    void setFrame(Frame *frame);

    void setBody(Stm *body);
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
    Literal(const char *literal);

    ~Literal() override;

    const char *getLiteral() const;

    void setLiteral(const char *value);
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

    int getType() const;

    int getNbytes() const;

    void setType(int type);

    void setNbytes(int nbytes);
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

    Temp(const char *name);

    virtual ~Temp();

    char *getName() const;

    char *toString();
};

class TempList {
private:
    Temp *temp;
    TempList *next;
public:
    TempList(Temp *temp, TempList *next);

    virtual ~TempList();

    Temp *getTemp() const;

    TempList *getNext() const;
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

    Label(const char *name);

    virtual ~Label();

    char *getName() const;

    char *toString();
};

class LabelList {
private:
    Label *label;
    LabelList *next;
public:
    LabelList(Label *label, LabelList *next);

    virtual ~LabelList();

    Label *getLabel() const;

    LabelList *getNext() const;
};

/**
 * A classe LocalAccess indica como um dado local é acessado.
 * Ela deve gerar a seqüência de instruções necessárias para acessar o nome.
 * Cada parâmetro e variável local deve ter seu objeto LocalAccess alocado no frame
 */
class LocalAccess {
public:
    virtual Stm accessCode()= 0; // retorna o código de máquina p/ acessar o nome
};

class AccessList {
private:
    LocalAccess *local;
    AccessList *next;
public:
    AccessList(LocalAccess *local, AccessList *next);

    inline LocalAccess *getLocal() const { return local; }

    inline AccessList *getNext() const { return next; }
};

class InFrame : public LocalAccess {
private:
    int offset;
    //TODO ...
public:
};

class InReg : public LocalAccess {
private:
    Temp temp;
    //TODO ...
public:
};

//TODO Temp FP("fp"); // Temp único que representa o registrador FP (ponteiro do frame)

class FrameMIPS : public Frame {
private:
    Label label;// Rótulo para o inicio do código do procedimento (nome)
    Temp returnValue;// Temporario que contém o valor de retorno da função
    AccessList localData;// Lista de acessos locais (parâmetros e variáveis locais)
public:
    // Essas funções decidem se o dado estará em um registrador ou no frema.
    // uma variável escapa, ela tem que ser colocada no frame.
    virtual LocalAccess addParam(bool escape, int bytesSize);

    virtual LocalAccess addLocal(bool escape, int bytesSize);
};


#endif //COMPILADOR_2019_3_INTERMEDIATE_CODE_H
