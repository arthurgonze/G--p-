//
// Created by jorao on 09/12/2019.
//

#ifndef COMPILADOR_2019_3_TRANSLATOR_H
#define COMPILADOR_2019_3_TRANSLATOR_H


class Fragment {
private:
    Fragment * next;
public:
    virtual ~Fragment() = default;
    Fragment *getNext() const;
    void setNext(Fragment *next);
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

    virtual ~Procedure();

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

    const char *getLiteral() const;

    void setLiteral(const char *value);
};

class Variable : public Fragment {
private:
    int type;
    int nbytes;
public:
    Variable(int type, int nbytes);

    virtual ~Variable();

    int getType() const;

    void setType(int type);

    int getNbytes() const;

    void setNbytes(int nbytes);
};
#endif //COMPILADOR_2019_3_TRANSLATOR_H
