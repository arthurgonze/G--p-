//
// Created by jorao on 09/12/2019.
//

#include "translator.h"

Fragment *Fragment::getNext() const {
    return next;
}

void Fragment::setNext(Fragment *next) {
    Fragment::next = next;
}

/*****************************************/
Frame *Procedure::getFrame() const {
    return frame;
}

void Procedure::setFrame(Frame *frame) {
    Procedure::frame = frame;
}

Stm *Procedure::getBody() const {
    return body;
}

void Procedure::setBody(Stm *body) {
    Procedure::body = body;
}

Procedure::~Procedure() {

}

Procedure::Procedure(Frame *frame, Stm *body) : frame(frame), body(body) {}

/*****************************************/

Literal::Literal(const char *literal) : literal(literal) {}

const char *Literal::getLiteral() const {
    return literal;
}

void Literal::setLiteral(const char *value) {
    Literal::literal = literal;
}

/*****************************************/

Variable::Variable(int type, int nbytes) : type(type), nbytes(nbytes) {}

Variable::~Variable() {

}

int Variable::getType() const {
    return type;
}

void Variable::setType(int type) {
    Variable::type = type;
}

int Variable::getNbytes() const {
    return nbytes;
}

void Variable::setNbytes(int nbytes) {
    Variable::nbytes = nbytes;
}
