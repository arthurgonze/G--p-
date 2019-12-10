#include "intermediate_code.h"

int num_labels = 0;
int num_temps = 0;
/*********************FRAGMENT**************************/
Fragment::Fragment() {
    this->next = nullptr;
}

Fragment::~Fragment() {
    delete this->next;
}

/*********************PROCEDURE**************************/
Procedure::Procedure(Frame *frame, StmNode *body) : frame(frame), body(body) {}

Procedure::~Procedure() {
//    delete this->frame;
    delete this->body;
}

/*********************LITERAL**************************/
Literal::Literal(const char *literal) : literal(literal) {}

Literal::~Literal() {
    this->literal = nullptr;
}

/*********************VARIABLE**************************/
Variable::Variable(int type, int nbytes) : type(type), nbytes(nbytes) {}

Variable::~Variable() {

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

/*********************LABEL_LIST**************************/
LabelList::LabelList(Label *label, LabelList *next) : label(label), next(next) {}

LabelList::~LabelList() {
    // TODO Precisa decrementar o num_labels ?
    delete this->label;
    delete this->next;
}

/*********************ACCESS_LIST**************************/
AccessList::AccessList(LocalAccess *local, AccessList *next) {
    this->local = local;
    this->next = next;
}

AccessList::~AccessList() {
//    delete this->local;
    delete this->next;
}

/*********************IN FRAME**************************/
InFrame::InFrame(int offset) {
    this->offset = offset;
}

/*********************IN REG**************************/
InReg::InReg(Temp *temp) {
    this->temp = temp;
}

InReg::~InReg() {
    delete this->temp;
}

/*********************FRAME MIPS**************************/
FrameMIPS::FrameMIPS(Label *label, Temp *returnValue, AccessList *localData) {
    this->label = label;
    this->returnValue = returnValue;
    this->localData = localData;
}

FrameMIPS::~FrameMIPS() {
    delete this->label;
    delete this->returnValue;
    delete this->localData;
}

LocalAccess *FrameMIPS::addParam(bool escape, int bytesSize) {
    // TODO ...
    return nullptr;
}

LocalAccess *FrameMIPS::addLocal(bool escape, int bytesSize) {
    // TODO ...
    return nullptr;
}

