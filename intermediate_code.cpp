#include "intermediate_code.h"

int num_labels = 0;
int num_temps = 0;
Temp *FP = new Temp("fp"); // Temp único que representa o registrador FP (ponteiro do frame)
Temp *RV = new Temp("rv"); // Temp único que representa o registrador RV (retorno de função)

/*********************FRAGMENT**************************/
Fragment::Fragment() {
    this->next = nullptr;
}

Fragment::~Fragment() {
    delete this->next;
}

/*********************PROCEDURE**************************/
Procedure::Procedure(Frame *frame, StmNode *body) : frame(frame), body(body) {}

void Procedure::setBody(StmNode *body) {
    Procedure::body = body;
}

Procedure::~Procedure() {
    delete this->frame;
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

ExprNode *InFrame::accessCode() {
    return new MEM(new BINOP(PLUS, new TEMP(FP), new CONST(this->getOffset())));
}

/*********************IN REG**************************/
InReg::InReg(Temp *temp) {
    this->temp = temp;
}

InReg::~InReg() {
    delete this->temp;
}

ExprNode *InReg::accessCode() {
    return new TEMP(this->getTemp());
}

/*********************FRAME MIPS**************************/
FrameMIPS::FrameMIPS(Label *label, Temp *returnValue, AccessList *localData) {
    this->label = label;
    this->returnValue = returnValue;
    this->localData = localData;
    frameSize = 0;
    paramSize = 0;
    regSize = 4;
}

FrameMIPS::~FrameMIPS() {
    delete this->label;
    delete this->returnValue;
    delete this->localData;
    frameSize = 0;
    paramSize = 0;
}

LocalAccess *FrameMIPS::addParam(bool escape, int bytesSize) {
    if (escape || bytesSize > regSize) {
        paramSize += bytesSize;
        InFrame *inFrame = new InFrame(paramSize);
        this->localData = new AccessList(inFrame, this->localData);
        return inFrame;
    } else {
        InReg *inReg = new InReg(new Temp());
        this->localData = new AccessList(inReg, this->localData);
        return inReg;
    }
}

LocalAccess *FrameMIPS::addLocal(bool escape, int bytesSize) {
    if (escape || bytesSize > regSize) {
        frameSize -= bytesSize;
        InFrame *inFrame = new InFrame(frameSize);
        this->localData = new AccessList(inFrame, this->localData);
        return inFrame;
    } else {
        InReg *inReg = new InReg(new Temp());
        this->localData = new AccessList(inReg, this->localData);
        return inReg;
    }
}


/*********************ICT**************************/
CONST::CONST(int i) {
    this->i = i;
    this->setTypeStm(V_CONST);
}

CONSTF::CONSTF(float j) {
    this->j = j;
    this->setTypeStm(V_CONSTF);

}

NAME::NAME(Label *l) {
    this->l = l;
    this->setTypeStm(V_NAME);

}

NAME::~NAME() {
    delete this->l;
}

TEMP::TEMP(Temp *t) {
    this->t = t;
    this->setTypeStm(V_TEMP);

}

TEMP::~TEMP() {
    delete this->t;
}

BINOP::BINOP(int binop, ExprNode *left, ExprNode *right) {
    this->binop = binop;
    this->left = left;
    this->right = right;
    this->setTypeStm(V_BINOP);

}

BINOP::~BINOP() {
    delete this->left;
    delete this->right;
}

void BINOP::setLeft(ExprNode *left) {
    BINOP::left = left;
}

void BINOP::setRight(ExprNode *right) {
    BINOP::right = right;
}

MEM::MEM(ExprNode *e) {
    this->e = e;
    this->setTypeStm(V_MEM);

}

MEM::~MEM() {
    delete this->e;
}

void MEM::setE(ExprNode *e) {
    MEM::e = e;
}

CALL::CALL(ExprNode *func, ExpList *args) {
    this->func = func;
    this->args = args;
    this->setTypeStm(V_CALL);

}

CALL::~CALL() {
    delete this->func;
    delete this->args;
}

ESEQ::ESEQ(StmNode *s, ExprNode *e) {
    this->s = s;
    this->e = e;
    this->setTypeStm(V_ESEQ);
}

ESEQ::~ESEQ() {
    delete this->s;
    delete this->e;
}

void ESEQ::setS(StmNode *s) {
    ESEQ::s = s;
}

void ESEQ::setE(ExprNode *e) {
    ESEQ::e = e;
}

MOVE::MOVE(ExprNode *dst, ExprNode *src) {
    this->dst = dst;
    this->src = src;
    this->setTypeStm(V_MOVE);

}

MOVE::~MOVE() {
    delete this->dst;
    delete this->src;
}

void MOVE::setDst(ExprNode *dst) {
    MOVE::dst = dst;
}

void MOVE::setSrc(ExprNode *src) {
    MOVE::src = src;
}

EXP::EXP(ExprNode *e) {
    this->e = e;
    this->setTypeStm(V_EXP);

}

EXP::~EXP() {
    delete this->e;
}

JUMP::JUMP(ExprNode *e, LabelList *targets) {
    this->e = e;
    this->targets = targets;
    this->setTypeStm(V_JUMP);

}

JUMP::JUMP(ExprNode *e) {
    this->e = e;
    this->targets = nullptr;
    this->setTypeStm(V_JUMP);

}

JUMP::~JUMP() {
    delete this->e;
    delete this->targets;
}

void JUMP::setE(ExprNode *e) {
    JUMP::e = e;
}

void JUMP::setTargets(LabelList *targets) {
    JUMP::targets = targets;
}

CJUMP::CJUMP(int relop, ExprNode *left, ExprNode *right, Label *ifTrue, Label *ifFalse) {
    this->relop = relop;
    this->left = left;
    this->right = right;
    this->ifTrue = ifTrue;
    this->ifFalse = ifFalse;
    this->setTypeStm(V_CJUMP);

}

CJUMP::~CJUMP() {
    delete this->left;
    delete this->right;
    delete this->ifTrue;
    delete this->ifFalse;
}

void CJUMP::setLeft(ExprNode *left) {
    CJUMP::left = left;
}

void CJUMP::setRight(ExprNode *right) {
    CJUMP::right = right;
}

SEQ::SEQ(StmNode *left, StmNode *right) {
    this->left = left;
    this->right = right;
    this->setTypeStm(V_SEQ);
}

SEQ::~SEQ() {
    delete this->left;
    delete this->right;
}

void SEQ::setS1(StmNode *left) {
    SEQ::left = left;
}

void SEQ::setS2(StmNode *right) {
    SEQ::right = right;
}

LABEL::LABEL(Label *l) {
    this->l = l;
    this->setTypeStm(V_LABEL);

}

LABEL::~LABEL() {
    delete this->l;
}

ExpList::ExpList(ExprNode *first, ExpList *next) {
    this->first = first;
    this->next = next;
    this->setTypeStm(V_EXP_LIST);

}

ExpList::~ExpList() {
    delete this->first;
    delete this->next;
}

void ExpList::setFirst(ExprNode *first) {
    ExpList::first = first;
}

void ExpList::setNext(ExpList *next) {
    ExpList::next = next;
}

StmList::StmList(StmNode *first, StmList *next) {
    this->first = first;
    this->next = next;
    this->setTypeStm(V_STM_LIST);

}

StmList::~StmList() {
    delete this->first;
    delete this->next;
}

void StmList::setFirst(StmNode *first) {
    StmList::first = first;
}

void StmList::setNext(StmList *next) {
    StmList::next = next;
}

int StmNode::getTypeStm() const {
    return typeStm;
}

void StmNode::setTypeStm(int typeStm) {
    StmNode::typeStm = typeStm;
}
