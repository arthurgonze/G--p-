
#ifndef COMPILADOR_2019_3_INTERMEDIATE_CODE_H
#define COMPILADOR_2019_3_INTERMEDIATE_CODE_H



int num_labels = 0;
int num_temps = 0;

class Fragment {
private:
    Fragment * next;
public:
    Fragment();
    virtual ~Fragment();
    virtual Fragment *getNext() const;
};

class Frame{

};

class Stm{

};


class Procedure : public Fragment {
private:
    Frame *frame;
    Stm * body;
public:

    Procedure(Frame *frame, Stm *body);
    ~Procedure() override;
    Frame *getFrame() const;
    void setFrame(Frame *frame);
    Stm *getBody() const;
    void setBody(Stm *body);
};


class Literal : public Fragment {
private:
    const char *literal;
public:

    Literal(const char *literal);
    ~Literal() override;
    const char *getLiteral() const;
    void setLiteral(const char *value);
};

class Variable : public Fragment {
private:
    int type;
    int nbytes;
public:

    Variable(int type, int nbytes);
    ~Variable() override;
    int getType() const;
    void setType(int type);
    int getNbytes() const;
    void setNbytes(int nbytes);
};

class Temp {
private:
    char *name;
public:

    Temp();
    Temp(const char *name);
    virtual ~Temp();
    char *getName() const;
    char * toString();
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

class Label {
private:
    char *name;
public:
    Label();
    Label(const char *name);
    virtual ~Label();
    char *toString();
    char *getName() const;
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
