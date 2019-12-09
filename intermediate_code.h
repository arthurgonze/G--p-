#ifndef COMPILADOR_2019_3_INTERMEDIATE_CODE_H
#define COMPILADOR_2019_3_INTERMEDIATE_CODE_H

int num_labels = 0;
int num_temps = 0;

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


#endif //COMPILADOR_2019_3_INTERMEDIATE_CODE_H
