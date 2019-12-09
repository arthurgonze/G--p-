#include <cstdio>
#include <cstring>
#include "intermediate_code.h"


/*********************FRAGMENT**************************/

Fragment *Fragment::getNext() const {
    return this->next;
}

Fragment::~Fragment() {
    delete this->next;
}

Fragment::Fragment() {
    this->next = nullptr;
}

/*********************PROCEDURE**************************/
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
    delete this->frame;
    delete this->body;
}


Procedure::Procedure(Frame *frame, Stm *body) : frame(frame), body(body) {}

/*********************LITERAL**************************/

Literal::Literal(const char *literal) : literal(literal) {}

const char *Literal::getLiteral() const {
    return literal;
}

void Literal::setLiteral(const char *value) {
    this->literal = value;
}

Literal::~Literal() {
    this->literal = nullptr;
}

/*********************VARIABLE**************************/

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

/*********************TEMP**************************/

Temp::Temp() {
    char name_aux[250];
    sprintf(name_aux, "$%d", num_temps++);
    this->name = new char[strlen(name_aux) + 1];
    strcpy(this->name, name_aux);
}

Temp::Temp(const char *name) {
    this->name = new char[strlen(name) + 1];
    strcpy(this->name, name);
}

Temp::~Temp() {
    // TODO Precisa decrementar o num_temps ?
    this->name = nullptr;
}

char *Temp::getName() const {
    return name;
}

char *Temp::toString() {
    char *temp_name = new char[strlen("temp") + strlen(this->name) + 1];
    strcpy(temp_name, "temp");
    strcat(temp_name, this->name);
    return temp_name;
}

/*********************TEMP_LIST**************************/


TempList::TempList(Temp *temp, TempList *next) : temp(temp), next(next) {}

TempList::~TempList() {
    // TODO Precisa decrementar o num_temps ?
    delete this->temp;
    delete this->next;
}

Temp *TempList::getTemp() const {
    return temp;
}

TempList *TempList::getNext() const {
    return next;
}


/*********************LABEL**************************/

Label::Label() {
    char name_aux[250];
    sprintf(name_aux, "Label$%d", num_labels++);
    this->name = new char[strlen(name_aux) + 1];
    strcpy(this->name, name_aux);
}

Label::Label(const char *name) {
    this->name = new char[strlen(name) + 1];
    strcpy(this->name, name);
}

Label::~Label() {
    // TODO Precisa decrementar o num_labels ?
    this->name = nullptr;
}

char *Label::getName() const {
    return this->name;
}

char *Label::toString() {
    return this->name;
}

/*********************LABEL_LIST**************************/

LabelList::LabelList(Label *label, LabelList *next) : label(label), next(next) {}

LabelList::~LabelList() {
    // TODO Precisa decrementar o num_labels ?
    delete this->label;
    delete this->next;
}

Label *LabelList::getLabel() const {
    return label;
}

LabelList *LabelList::getNext() const {
    return next;
}
