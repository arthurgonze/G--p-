#include "translator.h"
#include "ast.h"
#include "symbol_table.h"

void startTranslator(ProgramNode *ast, Translator *translator) {
    translator->visit(ast);
    fprintf(stderr, "\n");
}

void endTranslator(Translator *translator) {
    translator->deleteTables();
    translator->printFragmentList();
}

/********** TRANSLATOR **********/
void Translator::visit(ProgramNode *programNode) {
    if (programNode->getVarList() != nullptr) {
        programNode->getVarList()->accept(this);
    }
    if (programNode->getTypeList() != nullptr) {
        programNode->getTypeList()->accept(this);
    }
    if (programNode->getFunctions() != nullptr) {
        programNode->getFunctions()->accept(this);
    }
}

void Translator::visit(FunctionListNode *functionListNode) {
    if (functionListNode->getNext() != nullptr) {
        functionListNode->getNext()->accept(this);
    }
    if (functionListNode->getFunction() != nullptr) {
        functionListNode->getFunction()->accept(this);
    }
}

void Translator::visit(IdListNode *idListNode) {}

StmNode *Translator::visit(StmtListNode *stmtListNode) {
    if (stmtListNode && stmtListNode->getStmt()) {
        if (stmtListNode->getNext() != nullptr) {
            return new SEQ(stmtListNode->getStmt()->accept(this), stmtListNode->getNext()->accept(this));
        } else {
            return stmtListNode->getStmt()->accept(this);
        }
    } else {
        return NULL;
    }
}

void Translator::visit(VarStmtNode *varStmtNode) {}

StmNode *Translator::visit(IfNode *ifNode) {
    Label *thenLabel = new Label();
    Label *elseLabel = new Label();
    Label *endLabel = new Label();

    // sem else
    if (ifNode->getFalseStmt() == nullptr && ifNode->getTrueStmt() != nullptr) {
        return new SEQ(new CJUMP(NE, ifNode->getHead()->accept(this), new CONST(0), thenLabel, endLabel),
                       new SEQ(new LABEL(thenLabel),
                               new SEQ(ifNode->getTrueStmt()->accept(this),
                                       new LABEL(endLabel))));
    } else if (ifNode->getFalseStmt() != nullptr && ifNode->getTrueStmt() != nullptr) {// com else
        return new SEQ(new CJUMP(NE, ifNode->getHead()->accept(this), new CONST(0), thenLabel, elseLabel),
                       new SEQ(new LABEL(thenLabel),
                               new SEQ(new SEQ(ifNode->getTrueStmt()->accept(this),
                                               // TODO eu to passando nullptr pra todos os targets dos jumps
                                               new JUMP(new NAME(endLabel), nullptr)),
                                       new SEQ(new LABEL(elseLabel),
                                               new SEQ(ifNode->getFalseStmt()->accept(this),
                                                       new LABEL(endLabel))))));
    } else {
        return NULL;
    }
}

StmNode *Translator::visit(WhileNode *whileNode) {
    Label *testLabel = new Label();
    Label *startLabel = new Label();
    // TODO fazer um label de fim do while? e empilhar e desempilhar esses labels?
    Label *endLabel = new Label();

    return new SEQ(new LABEL(testLabel),
                   new SEQ(new CJUMP(NE, whileNode->getHead()->accept(this), new CONST(0), startLabel, endLabel),
                           new SEQ(new LABEL(startLabel),
                                   new SEQ(whileNode->getBody()->accept(this),
                                           new SEQ(new JUMP(new NAME(testLabel), nullptr),
                                                   new LABEL(endLabel))))));
}

StmNode *Translator::visit(SwitchNode *switchNode) {// TODO ...
    if (switchNode->getExp() != nullptr) {
        switchNode->getExp()->accept(this);
    }
    if (switchNode->getBlock() != nullptr) {
        switchNode->getBlock()->accept(this);
    }
    return NULL;
}

StmNode *Translator::visit(PrintNode *printNode) {// TODO...
    if (printNode->getExpList() != nullptr) {
        printNode->getExpList()->accept(this);
    }
    return NULL;
}

StmNode *Translator::visit(ReadLnNode *readLnNode) {// TODO ...
    if (readLnNode->getExp() != nullptr) {
        readLnNode->getExp()->accept(this);
    }
    return NULL;
}

StmNode *Translator::visit(ReturnNode *returnNode) {// TODO ...
    if (returnNode->getExp() != nullptr) {
        returnNode->getExp()->accept(this);
    }
    return NULL;
}

void Translator::visit(CaseBlockNode *caseBlockNode) {}

ExprNode *Translator::visit(ExpListNode *expListNode) { return NULL; }// did it in the callNode

//TODO professor disse que o codigo varia conforme a arquitetura e que não era necessario a implementacao
StmNode *Translator::visit(TryNode *tryNode) { return NULL; }

ExprNode *Translator::visit(PrimaryNode *primaryNode) {//TODO checar casos de arranjos?
    //checa se eh ID
    if (primaryNode->getTokenNode() && primaryNode->getTokenNode()->getToken() == ID) {
        VarSymbol *aux;
        if(activeFunction)
        {
            aux = varTable->searchInScope(primaryNode->getLexeme(), activeFunction->getLexeme());
        }
        if (aux)// variavel local
        {
            //TODO verificar corretude
            LocalAccess *tmp = this->currentFrame->addLocal(false, primaryNode->getTokenNode()->getSize());
            return tmp->accessCode();
//            return new BINOP(PLUS, new TEMP(FP), new CONST(aux->getOffset()));
        } else {
            aux = varTable->cSearch(primaryNode->getTokenNode()->getLexeme());
            if (aux) {// var global
                char name[200];
                sprintf(name,"var_%s",primaryNode->getTokenNode()->getLexeme());
                Label *labelAUX=new Label(name);
                return new MEM(new NAME(labelAUX));
            }
        }
    } else // nao eh ID
    {
        //checa se eh expressao
        if(primaryNode->getExp()){
            return primaryNode->getExp()->accept(this);
        }
        //checa se eh um primitivo
        if (primaryNode->getTokenNode()->getToken() == NUMINT) {
            return new CONST(atoi(primaryNode->getLexeme()));
        }

        if (primaryNode->getTokenNode()->getToken() == NUMFLOAT) {
            return new CONSTF(atof(primaryNode->getLexeme()));
        }

        if (primaryNode->getTokenNode()->getToken() == LITERALCHAR ||
            primaryNode->getTokenNode()->getToken() == LITERAL) {
            Literal *literal = new Literal(primaryNode->getLexeme());
            //TODO verificar
            literal->setNext(fragmentList);
            fragmentList = literal;
            return new NAME(new Label(primaryNode->getLexeme()));
        }

        if (primaryNode->getTokenNode()->getToken() == TRUE) {
            return new CONST(1);
        }

        if (primaryNode->getTokenNode()->getToken() == FALSE) {
            return new CONST(0);
        }
    }

    return NULL;
}

ExprNode *Translator::visit(CallNode *callNode) {
    ExpListNode *param = callNode->getParameters(); // pega os parametros da chamada
    ExpList *lcall = NULL;
    while (param) {// se tiver parametros
        ExprNode *exp = param->getExp()->accept(this);// acessa o no
        lcall = new ExpList(exp, lcall);// cria uma lista de exp
        param = param->getNext();// vai para o proximo param
    }
    char callLabel[200];
    sprintf(callLabel, "func_%s_call", callNode->getId()->getLexeme());// cria um label para a chamada
    return new CALL(new NAME(new Label(callLabel)), lcall);// cria o call node com a funcao e seus parametros
}

ExprNode *Translator::visit(ArrayCallNode *arrayCallNode) {
    int tam;
    //verificar se o tipo do array eh uma estrutura
    StructSymbol *structSymbol = structTable->cSearch(
            arrayCallNode->getTypeLexeme());// TODO to verificando certo na tabela?
    if (structSymbol) {
        tam = structSymbol->getSize();
    } else if (arrayCallNode->getExp()->isPointer() || arrayCallNode->getType() == FLOAT) {
        // se nao for uma estrutra e for um ponteiro ou float
        tam = INT32_SIZE;
    } else {
        // se for um tipo primitivo que nao seja ponteiro ou float
        tam = INT8_SIZE;
    }

    return new MEM(new BINOP(PLUS,
                             arrayCallNode->getExp()->accept(this),
                             new BINOP(STAR,
                                       new CONST(tam),
                                       arrayCallNode->getIndex()->accept(this))));
}

ExprNode *Translator::visit(ArrayNode *arrayNode) { return NULL; }

ExprNode *Translator::visit(AssignNode *assignNode) {
    // TODO esse cast eh ok?
    return (ExprNode *) (new MOVE(assignNode->getExp1()->accept(this), assignNode->getExp2()->accept(this)));

}

ExprNode *Translator::visit(AdditionOPNode *additionOPNode) {
    LocalAccess *tmp = this->currentFrame->addLocal(false, INT8_SIZE);
    return new ESEQ(new MOVE(tmp->accessCode(),
                             new BINOP(additionOPNode->getOp()->getToken(),
                                       additionOPNode->getExp1()->accept(this),
                                       additionOPNode->getExp2()->accept(this))),
                    tmp->accessCode());
//    return new BINOP(additionOPNode->getOp()->getToken(), additionOPNode->getExp1()->accept(this),
//                     additionOPNode->getExp2()->accept(this));
}

ExprNode *Translator::visit(MultiplicationOPNode *multiplicationOPNode) {
    LocalAccess *tmp = this->currentFrame->addLocal(false, INT8_SIZE);//TODO eh INT8 mesmo?
    return new ESEQ(new MOVE(tmp->accessCode(),
                             new BINOP(multiplicationOPNode->getOp()->getToken(),
                                       multiplicationOPNode->getExp1()->accept(this),
                                       multiplicationOPNode->getExp2()->accept(this))),
                    tmp->accessCode());
//    return new BINOP(multiplicationOPNode->getOp()->getToken(), multiplicationOPNode->getExp1()->accept(this),
//                     multiplicationOPNode->getExp2()->accept(this));
}

ExprNode *Translator::visit(BooleanOPNode *booleanOPNode) {
    if (booleanOPNode->getExp1() && booleanOPNode->getExp2()) {
        if (booleanOPNode->getOp()->getToken() == AND) {
            Temp *r = new Temp();
            Label *l1 = new Label();
            Label *l2 = new Label();
            Label *l3 = new Label();

            return new ESEQ(new SEQ(new MOVE(new TEMP(r), new CONST(0)),
                                    new SEQ(new CJUMP(NE, booleanOPNode->getExp1()->accept(this), new CONST(0), l1, l2),
                                            new SEQ(new LABEL(l1),
                                                    new SEQ(new CJUMP(NE, booleanOPNode->getExp2()->accept(this),
                                                                      new CONST(0), l3,
                                                                      l2),
                                                            new SEQ(new LABEL(l3),
                                                                    new SEQ(new MOVE(new TEMP(r), new CONST(1)),
                                                                            new LABEL(l2))))))),
                            new TEMP(r));
        } else if (booleanOPNode->getOp()->getToken() == OR) {
            Temp *r = new Temp();
            Label *l1 = new Label();
            Label *l2 = new Label();

            return new ESEQ(new SEQ(new MOVE(new TEMP(r), booleanOPNode->getExp1()->accept(this)),
                                    new SEQ(new CJUMP(NE, booleanOPNode->getExp2()->accept(this), new CONST(0), l1, l2),
                                            new SEQ(new LABEL(l1),
                                                    new SEQ(new MOVE(new TEMP(r), new CONST(1)),
                                                            new LABEL(l2))))),
                            new TEMP(r));
        } else {//TODO if (booleanOPNode->getOp()->getToken() == (LE || GE || GT || LT || EQ || NE)) {
            Temp *r = new Temp();
            Label *t = new Label();
            Label *f = new Label();
            return new ESEQ(new SEQ(new MOVE(new TEMP(r), new CONST(1)),
                                    new SEQ(new CJUMP(booleanOPNode->getOp()->getToken(),
                                                      booleanOPNode->getExp1()->accept(this),
                                                      booleanOPNode->getExp2()->accept(this), t, f),
                                            new SEQ(new LABEL(f),
                                                    new SEQ(new MOVE(new TEMP(r), new CONST(0)), new LABEL(t))))),
                            new TEMP(r));
        }
    }
    return NULL;
}

ExprNode *Translator::visit(NotNode *notNode) {
    LocalAccess *tmp = this->currentFrame->addLocal(false, INT8_SIZE);
    Label *t = new Label();
    Label *f = new Label();
    return new ESEQ(new SEQ(new MOVE(tmp->accessCode(), new CONST(1)),
                            new SEQ(new CJUMP(EQ, notNode->getExp()->accept(this), new CONST(0), t, f),
                                    new SEQ(new LABEL(f),
                                            new SEQ(new MOVE(tmp->accessCode(), new CONST(0)),
                                                    new LABEL(t))))),
                    tmp->accessCode());
}

ExprNode *Translator::visit(SignNode *signNode) {
    LocalAccess *result = this->currentFrame->addLocal(false, INT8_SIZE);
    return new ESEQ(new MOVE(result->accessCode(),
                             new BINOP(MINUS, new CONST(0), signNode->getExp()->accept(this))),
                    result->accessCode());

//    return new BINOP(MINUS, new CONST(0), signNode->getExp()->accept(this));
}

void Translator::visit(FormalListNode *formalListNode) {
    FormalListNode *formalListNodeAUX = formalListNode;
    bool escape;
    while (formalListNodeAUX) {
        escape = false;
        // se for id, ponteiro ou arranjo ele escapa
        if ((formalListNodeAUX->getType()->getType() && formalListNodeAUX->getParameterType() == ID) ||
            (formalListNodeAUX->getPointer()) || (formalListNodeAUX->getArray())) {
            escape = true;
        }
        this->currentFrame->addParam(escape, formalListNodeAUX->getId()->getSize());//TODO get size ou offset?
        formalListNodeAUX = formalListNodeAUX->getNext();
    }
}

void Translator::visit(FunctionNode *functionNode) {
    currentFrame = new FrameMIPS(nullptr, nullptr, nullptr);
    currentFrame->setReturnValue(new Temp());// marcando o temporario pra onde retornar

    if (functionNode->getParameters()) {
        functionNode->getParameters()->accept(this);
    }
    if (functionNode && functionNode->getId()) {
        this->activeFunction = functionTable->cSearch(functionNode->getId()->getLexeme());
    }
    if (functionNode->getLocal()) {
        functionNode->getLocal()->accept(this);
    }

    char endFuncLabel[50];
    sprintf(endFuncLabel, "%s_end", functionNode->getId()->getLexeme());
//    Label *endLab = new Label(endFuncLabel);
    // TODO empilhar

    StmNode *stmNode = NULL;// preciso desse no pra guardar todas as instrucoes dos retornos que estarao no corpo da funcao
    if (functionNode->getBody()) {
        stmNode = this->visit(functionNode->getBody());// faco o no receber o retorno do visit ao corpo da funcao
    }
    // TODO desempilhar

    stmNode = new SEQ(new LABEL(new Label(functionNode->getId()->getLexeme())), stmNode);
    stmNode = new SEQ(stmNode, new LABEL(new Label(endFuncLabel)));

    Procedure *procedure = new Procedure(currentFrame, stmNode);
    procedure->setNext(fragmentList);
    fragmentList = procedure;
}

ExprNode *Translator::visit(PointerExpNode *pointerExpNode) {//TODO verificar corretude
    VarSymbol *pointerVar = varTable->cSearch(pointerExpNode->getId()->getLexeme());
    if (pointerVar) {
        // TODO offset ou size?
        return new MEM(new BINOP(PLUS, pointerExpNode->getExp()->accept(this), new CONST(pointerVar->getOffset())));
    } else {
        return NULL;
    }
}

ExprNode *Translator::visit(NameExpNode *nameExpNode) {
    // TODO aqui eu to pegando a variavel e consequentemente as informacoes de tamanho/offset que ela pertence?
    VarSymbol *var = varTable->cSearch(nameExpNode->getLexeme());
    return new MEM(new BINOP(PLUS, nameExpNode->getExp()->accept(this), new CONST(var->getOffset())));
}

void Translator::visit(VarDeclNode *varDeclNode) {
    VarDeclNode *varDeclNodeAUX = varDeclNode;
    bool escape;
    while (varDeclNodeAUX) {
        escape = false;
        if (varDeclNodeAUX->getType() &&
            varDeclNodeAUX->getType()->getId()->getToken() == ID)// se o tipo da variavel declarada for um ID
        {
            escape = true;
        }
        if (varDeclNodeAUX->getIdList()) {
            IdListNode *idListNodeAUX = varDeclNodeAUX->getIdList();
            while (idListNodeAUX) {
                if (idListNodeAUX->getPointer() || idListNodeAUX->getArray())//se for array ou ponteiro
                {
                    escape = true;
                }
                // buscar na tabela para saber se eh local ou global
                VarSymbol *varSymbol = NULL;
                if (this->activeFunction) {
                    varSymbol = varTable->searchInScope(idListNodeAUX->getId()->getLexeme(),
                                                        activeFunction->getLexeme());
                }

                if (varSymbol)//local
                {
                    //TODO getSize ou getOffset?, verificar se o bool de escapa ta certo
                    this->currentFrame->addLocal(escape, varSymbol->getSize());
                    Variable *variable = new Variable(varSymbol->getType()->getType(), varSymbol->getSize());
                    variable->setNext(fragmentList);
                    fragmentList = variable;
                } else//global
                {
                    varSymbol = varTable->cSearch(idListNodeAUX->getId()->getLexeme());
                    if (varSymbol) {
                        Variable *variable = new Variable(varSymbol->getType()->getType(), varSymbol->getSize());
                        variable->setNext(fragmentList);
                        fragmentList = variable;
                    }
                }
                idListNodeAUX = idListNodeAUX->getNext();
            }
        }
        varDeclNodeAUX = varDeclNodeAUX->getNext();
    }
}

void Translator::visit(TypeDeclNode *typeDeclNode) {}

ExprNode *Translator::visit(AddressValNode *addressValNode) {// TODO acho que so retorna o accept mesmo e ja era?
    if (addressValNode->getExp() != nullptr) {
        return addressValNode->getExp()->accept(this);
    }
    return NULL;
}

ExprNode *Translator::visit(PointerValNode *pointerValNode) {
    return new MEM(pointerValNode->getExp()->accept(this));
}

/*
 * TERMINAL NODES
 */
void Translator::visit(PointerNode *pointerNode) {}

StmNode *Translator::visit(BreakNode *breakNode) { return NULL; }

StmNode *Translator::visit(ThrowNode *throwNode) { return NULL; }

void Translator::visit(TypeNode *typeNode) {}

ExprNode *Translator::visit(TokenNode *tokenNode) { return NULL; }

Translator::Translator(VarTable *varTable, FunctionTable *functionTable, StructTable *structTable) {
    this->varTable = varTable;
    this->functionTable = functionTable;
    this->structTable = structTable;
    fragmentList = nullptr;
    activeFunction = NULL;
}

void Translator::printFragmentList() {
    PrintICT *visitorICT = new PrintICT();
    visitorICT->visit(fragmentList);
}

void Translator::deleteTables() {
    delete this->varTable;
    delete this->structTable;
    delete this->functionTable;
}

