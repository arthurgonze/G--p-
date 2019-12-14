#include "translator.h"
#include "ast.h"
#include "symbol_table.h"

void startTranslator(ProgramNode *ast, Translator *translator) {
    translator->visit(ast);
    fprintf(stderr, "\n");
}

void endTranslator(Translator *translator) {
    //TODO ...
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
    if (stmtListNode->getNext() != nullptr) {
        return new SEQ(stmtListNode->getStmt()->accept(this), stmtListNode->getNext()->accept(this));
    } else {
        return stmtListNode->getStmt()->accept(this);
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

StmNode *Translator::visit(SwitchNode *switchNode) {
    if (switchNode->getExp() != nullptr) {
        switchNode->getExp()->accept(this);
    }
    if (switchNode->getBlock() != nullptr) {
        switchNode->getBlock()->accept(this);
    }
}

StmNode *Translator::visit(PrintNode *printNode) {
    if (printNode->getExpList() != nullptr) {
        printNode->getExpList()->accept(this);
    }
}

StmNode *Translator::visit(ReadLnNode *readLnNode) {
    if (readLnNode->getExp() != nullptr) {
        readLnNode->getExp()->accept(this);
    }
}

StmNode *Translator::visit(ReturnNode *returnNode) {
    if (returnNode->getExp() != nullptr) {
        returnNode->getExp()->accept(this);
    }

}

void Translator::visit(CaseBlockNode *caseBlockNode) {}

ExprNode *Translator::visit(ExpListNode *expListNode) { return NULL; }// did it in the callNode

StmNode *Translator::visit(TryNode *tryNode) {
    if (tryNode->getTry() != nullptr) {
        tryNode->getTry()->accept(this);
    }
    if (tryNode->getCatch() != nullptr) {
        tryNode->getCatch()->accept(this);
    }
}

ExprNode *Translator::visit(PrimaryNode *primaryNode) {
    if (primaryNode->getTokenNode() != nullptr) {
        primaryNode->getTokenNode()->accept(this);
    }
    if (primaryNode->getExp() != nullptr) {
        return primaryNode->getExp()->accept(this);
    }
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
    StructSymbol *structSymbol = structTable->cSearch(arrayCallNode->getTypeLexeme());// TODO to verificando certo na tabela?
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
            //TODO SAO SO ESSES OPERADORES RELACIONAIS OU TEM MAIS?(na condicao do if)
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
        // TODO fazer um if pra dizer que tudo escapa e setar as parada pra true?

//        VarSymbol *varSymbol= varTable->searchInScope(formalListNodeAUX->getId()->getLexeme(), activeFunction->getLexeme());
        //(Indice_campos*)funcaoatual->escopo->verificaExiste(formalListNodeAUX->id->lex);
        this->currentFrame->addParam(escape, formalListNodeAUX->getId()->getSize());//TODO get size ou offset?
        formalListNodeAUX = formalListNodeAUX->getNext();
    }
}

void Translator::visit(FunctionNode *functionNode) {
    currentFrame = new FrameMIPS(nullptr, nullptr, nullptr);
    currentFrame->setReturnValue(new Temp());// marcando o temporario pra onde retornar
    // TODO acho que a active function ta dando ruim e por isso as variavei locais não sao adicionada la no VarDeclNode
    activeFunction = functionTable->cSearch(functionNode->getId()->getLexeme());
    if (functionNode->getParameters()) {
        functionNode->getParameters()->accept(this);
    }
    if (functionNode->getLocal()) {
        functionNode->getLocal()->accept(this);
    }

    char endFuncLabel[50];
    sprintf(endFuncLabel, "%s_end", functionNode->getId()->getLexeme());
    Label *endLab = new Label(endFuncLabel);
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

ExprNode *Translator::visit(PointerExpNode *pointerExpNode) {
    if (pointerExpNode->getExp() != nullptr) {
        pointerExpNode->getExp()->accept(this);
    }
    if (pointerExpNode->getId() != nullptr) {
        pointerExpNode->getId()->accept(this);
    }
}

ExprNode *Translator::visit(NameExpNode *nameExpNode) {
    if (nameExpNode->getExp() != nullptr) {
        nameExpNode->getExp()->accept(this);
    }
    if (nameExpNode->getId() != nullptr) {
        nameExpNode->getId()->accept(this);
    }
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
                // TODO fazer um if pra dizer que tudo escapa e setar as parada pra true?
                // buscar na tabela para saber se eh local ou global
                if (activeFunction) {
                    VarSymbol *varSymbol = varTable->searchInScope(idListNodeAUX->getId()->getLexeme(),
                                                                   activeFunction->getLexeme());
                    if (varSymbol)//local
                    {
                        //TODO getSize ou getOffset?, verificar se o bool de escapa ta certo
                        this->currentFrame->addLocal(escape, varSymbol->getSize());
                    }
                    //TODO decidir aonde colocar o else para definir se a variavel eh global
//                    else//global
//                    {
//                        varSymbol = varTable->cSearch(idListNodeAUX->getId()->getLexeme());
//                        if (varSymbol) {
//                            //TODO eh assim que pega o int do tipo da variavel mesmo ne?
//                            Variable *variable = new Variable(varSymbol->getType()->getType(), varSymbol->getSize());
//                            variable->setNext(fragmentList);
//                            fragmentList = variable;
//                        }
//                    }
                } else //global
                {
                    VarSymbol *varSymbol = varTable->cSearch(idListNodeAUX->getId()->getLexeme());
                    if (varSymbol) {
                        //TODO eh assim que pega o int do tipo da variavel mesmo ne?
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

ExprNode *Translator::visit(AddressValNode *addressValNode) {
    if (addressValNode->getExp() != nullptr) {
        addressValNode->getExp()->accept(this);
    }
}

ExprNode *Translator::visit(PointerValNode *pointerValNode) {
    return new MEM(pointerValNode->getExp()->accept(this));
}

/*
 * TERMINAL NODES
 */
void Translator::visit(PointerNode *pointerNode) {}

StmNode *Translator::visit(BreakNode *breakNode) {}

StmNode *Translator::visit(ThrowNode *throwNode) { return NULL; }

void Translator::visit(TypeNode *typeNode) {}

ExprNode *Translator::visit(TokenNode *tokenNode) {
    if (tokenNode->getToken() == NUMINT) {
        return new CONST(atoi(tokenNode->getLexeme()));
    }

    if (tokenNode->getToken() == NUMFLOAT) {
        return new CONSTF(atof(tokenNode->getLexeme()));
    }

    if (tokenNode->getToken() == LITERALCHAR || tokenNode->getToken() == LITERAL) {
        Literal *literal = new Literal(tokenNode->getLexeme());
        //TODO verificar
        literal->setNext(fragmentList);
        fragmentList = literal;
        return new NAME(new Label(tokenNode->getLexeme()));
    }

    if (tokenNode->getToken() == TRUE) {
        return new CONST(1);
    }

    if (tokenNode->getToken() == FALSE) {
        return new CONST(0);
    }

    if (tokenNode->getToken() == ID) {
        //TODO verificar se eh variavel ou funcao?... na real acho que a verificação do ID não deve ser feita aqui por nao ter peso nenhum
        // verificar se eh variavel simples
        VarSymbol *aux = varTable->searchInScope(tokenNode->getLexeme(), "FuncaoAtiva"/*activeFunction->getLexeme()*/);
        if (aux)//se for diferente de nulo o id eh variavel
        {
            return new BINOP(PLUS, new TEMP(FP), new CONST(aux->getOffset()));
        } else {
            aux = varTable->cSearch(tokenNode->getLexeme());
            if (aux) {
                return new MEM(new BINOP(PLUS, new TEMP(FP), new CONST(
                        aux->getOffset())));//TODO VERIFICAR FRAME POINTER E OFFSET e olhar no slide o codigo
            }
        }
        return NULL;
    }
}

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

