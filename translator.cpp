#include "translator.h"
#include "ast.h"
#include "symbol_table.h"

void startTranslator(ProgramNode *ast, Translator *translator) {
    ast->accept(translator);
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
    char thenLabelName[200];
    char endlabelName[200];
    sprintf(thenLabelName, "if_then_%p", &ifNode);
    sprintf(endlabelName, "if_end_%p", &ifNode);
    Label *thenLabel = new Label(thenLabelName);
    Label *endLabel = new Label(endlabelName);

    // sem else
    if (ifNode->getFalseStmt() == nullptr && ifNode->getTrueStmt() != nullptr) {
        return new SEQ(new CJUMP(NE, ifNode->getHead()->accept(this), new CONST(0), thenLabel, endLabel),
                       new SEQ(new LABEL(thenLabel),
                               new SEQ(ifNode->getTrueStmt()->accept(this),
                                       new LABEL(endLabel))));
    } else if (ifNode->getFalseStmt() != nullptr && ifNode->getTrueStmt() != nullptr) {// com else
        char elseLabelName[200];
        sprintf(elseLabelName, "if_else_%p", &ifNode);
        Label *elseLabel = new Label(elseLabelName);
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
    char testlabelName[200];
    char startLabelName[200];
    char endlabelName[200];
    sprintf(testlabelName, "while_test_%p", &whileNode);
    sprintf(endlabelName, "while_end_%p", &whileNode);
    sprintf(startLabelName, "while_start_%p", &whileNode);
    Label *testLabel = new Label(testlabelName);
    Label *startLabel = new Label(startLabelName);
    Label *endLabel = new Label(endlabelName);

    return new SEQ(new LABEL(testLabel),
                   new SEQ(new CJUMP(NE, whileNode->getHead()->accept(this), new CONST(0), startLabel, endLabel),
                           new SEQ(new LABEL(startLabel),
                                   new SEQ(whileNode->getBody()->accept(this),
                                           new SEQ(new JUMP(new NAME(testLabel), nullptr),
                                                   new LABEL(endLabel))))));
}

StmNode *Translator::visit(SwitchNode *switchNode) {
    char endlabelName[200];
    char startLabelName[200];

    //criando o label do fim  do switch
    sprintf(endlabelName, "switch_end_%p", &switchNode);
    Label *labelSwitchEnd = new Label(endlabelName);

    sprintf(startLabelName, "switch_start_%p", &switchNode);
    Label *labelSwitchStart = new Label(startLabelName);

    //pego o primeiro bloco do switch
    CaseBlockNode *caseBlock = switchNode->getBlock();

    // variaveis auxiliares
    SEQ *seq2 = NULL, *seq = NULL, *seqAUX = NULL;

    //enquanto houver blocos
    while (caseBlock) {
        //labels auxiliares
        Label *labelEndCase = new Label();
        Label *labelCase = new Label();

        //se tem um proximo bloco no switch
        if (caseBlock->getNext()) {
            // o seq 2 recebe um label de continuacao e um no SEQ nulo
            seq2 = new SEQ(new LABEL(labelEndCase), new LABEL(labelCase));
        } else {
            // caso contrario, o seq2 recebe o label de continuacao e de fim
            seq2 = new SEQ(new LABEL(labelEndCase), new LABEL(labelSwitchEnd));
        }

        // seq recebe o cjump pra expressao do switch, o int pro bloco, o label do case, o label da continuacao e
        // recebe o label do case e recebe as sentencas do bloco e o bloco 3
        seqAUX = new SEQ(new CJUMP(EQ, switchNode->getExp()->accept(this),
                                   new CONST(atoi(caseBlock->getNum()->getLexeme())), labelCase, labelEndCase),
                         new SEQ(new LABEL(labelCase),
                                 new SEQ(caseBlock->getStmt()->accept(this), seq2)));
        seq = new SEQ(seq, seqAUX);
        caseBlock = caseBlock->getNext();
    }
    seq = new SEQ(new LABEL(labelSwitchStart), seq);
    return seq;
}

StmNode *Translator::visit(PrintNode *printNode) {
    // pego a lista de expressoes do print
    ExpListNode *param = printNode->getExpList();
    // crio uma lista de expressoes da ICT
    ExpList *lcall = NULL;
    while (param) {
        //construo esse novo no com as expressoes do no
        lcall = new ExpList(param->getExp()->accept(this), lcall);
        param = param->getNext();
    }
    // retorno  uma expressao call com a lista de expressoes e o name/label criado
    return new EXP(new CALL(new NAME(new Label("printNode")), lcall));
}

StmNode *Translator::visit(ReadLnNode *readLnNode) {
    // TODO dar um append no lexema da expr? readLnNode->getExp()->getLexeme();
    // retorno uma expressao call com o name/label da chamada e o accept para a expressao do read
    return new EXP(new CALL(new NAME(new Label("readLnNode")),
                            new ExpList(readLnNode->getExp()->accept(this), NULL)));
}

StmNode *Translator::visit(ReturnNode *returnNode) {
    Label *funcLabel;
    if (activeFunction) {
        char endFuncLabel[50];
        sprintf(endFuncLabel, "%s_end", activeFunction->getLexeme());
        funcLabel = new Label(endFuncLabel);
    }
    return new SEQ(new MOVE(new TEMP(this->currentFrame->getReturnValue()), returnNode->getExp()->accept(this)),
                   new JUMP(new NAME(funcLabel)));
}

void Translator::visit(CaseBlockNode *caseBlockNode) {}

ExprNode *Translator::visit(ExpListNode *expListNode) { return NULL; }// did it in the callNode

//TODO professor disse que o codigo varia conforme a arquitetura e que não era necessario a implementacao
StmNode *Translator::visit(TryNode *tryNode) { return NULL; }

ExprNode *Translator::visit(PrimaryNode *primaryNode) {//TODO checar casos de arranjos?
    //checa se eh ID
    if (primaryNode->getTokenNode() && primaryNode->getTokenNode()->getToken() == ID) {
        VarSymbol *aux;
        if (activeFunction) {
            aux = varTable->searchInScope(primaryNode->getTokenNode()->getLexeme(), activeFunction->getLexeme());
        }
        if (aux)// variavel local
        {
            //TODO verificar corretude
            LocalAccess *tmp = this->currentFrame->addLocal(false, primaryNode->getTokenNode()->getSize());
            return tmp->accessCode();
//            return new BINOP(PLUS, new TEMP(FP), new CONST(aux->getOffset()));
        } else {
            aux = varTable->cSearch(primaryNode->getTokenNode()->getLexeme());
            if (aux) {// var nonLocal
                char name[200];
                sprintf(name, "nonLocal_var_%s", primaryNode->getTokenNode()->getLexeme());
                Label *labelAUX = new Label(name);
                return new MEM(new NAME(labelAUX));
            }
        }
    } else // nao eh ID
    {
        //checa se eh expressao
        if (primaryNode->getExp()) {
            return primaryNode->getExp()->accept(this);
        }
        //checa se eh um primitivo
        if (primaryNode->getTokenNode()->getToken() == NUMINT) {
            return new CONST(atoi(primaryNode->getTokenNode()->getLexeme()));
        }

        if (primaryNode->getTokenNode()->getToken() == NUMFLOAT) {
            return new CONSTF(atof(primaryNode->getTokenNode()->getLexeme()));
        }

        if (primaryNode->getTokenNode()->getToken() == LITERALCHAR ||
            primaryNode->getTokenNode()->getToken() == LITERAL) {
            Literal *literal = new Literal(primaryNode->getTokenNode()->getLexeme());
            //TODO verificar
            literal->setNext(fragmentList);
            fragmentList = literal;
            return new NAME(new Label(primaryNode->getTokenNode()->getLexeme()));
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
    } else if (arrayCallNode->getExp()->isPointer() || arrayCallNode->getType() == FLOAT|| arrayCallNode->getType() == INT) {
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
//                                       new CONST(atoi(((PrimaryNode*) arrayCallNode->getIndex())->getTokenNode()->getLexeme())))));
                                       arrayCallNode->getIndex()->accept(this))));
}

ExprNode *Translator::visit(ArrayNode *arrayNode) { return NULL; }

ExprNode *Translator::visit(AssignNode *assignNode) {
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
        Temp *r = new Temp();
        Label *l1 = new Label();
        Label *l2 = new Label();
        if (booleanOPNode->getOp()->getToken() == AND) {
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
            return new ESEQ(new SEQ(new MOVE(new TEMP(r), booleanOPNode->getExp1()->accept(this)),
                                    new SEQ(new CJUMP(NE, booleanOPNode->getExp2()->accept(this), new CONST(0), l1, l2),
                                            new SEQ(new LABEL(l1),
                                                    new SEQ(new MOVE(new TEMP(r), new CONST(1)),
                                                            new LABEL(l2))))),
                            new TEMP(r));
        } else {//TODO if (booleanOPNode->getOp()->getToken() == (LE || GE || GT || LT || EQ || NE)) {
            return new ESEQ(new SEQ(new MOVE(new TEMP(r), new CONST(1)),
                                    new SEQ(new CJUMP(booleanOPNode->getOp()->getToken(),
                                                      booleanOPNode->getExp1()->accept(this),
                                                      booleanOPNode->getExp2()->accept(this), l1, l2),
                                            new SEQ(new LABEL(l2),
                                                    new SEQ(new MOVE(new TEMP(r), new CONST(0)), new LABEL(l1))))),
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

    if (functionNode && functionNode->getId()) {
        this->activeFunction = functionTable->cSearch(functionNode->getId()->getLexeme());
    }

    if (functionNode->getParameters()) {
        functionNode->getParameters()->accept(this);
    }
    if (functionNode->getLocal()) {
        functionNode->getLocal()->accept(this);
    }

    char endFuncLabel[50];
    sprintf(endFuncLabel, "%s_end", functionNode->getId()->getLexeme());

    if (functionNode->getBody() && currentFrame) {
        Procedure *procedure = new Procedure(currentFrame,
                                             new SEQ(new SEQ(new LABEL(new Label(functionNode->getId()->getLexeme())),
                                                             functionNode->getBody()->accept(this)),
                                                     new LABEL(new Label(endFuncLabel))));
        procedure->setNext(fragmentList);
        fragmentList = procedure;
    }
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
    VarSymbol *var = varTable->searchInScope(nameExpNode->getId()->getLexeme(), nameExpNode->getExp()->getTypeLexeme());
    if (var) {
        return new MEM(new BINOP(PLUS, nameExpNode->getExp()->accept(this), new CONST(var->getOffset())));
    } else {
        return NULL;
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
    if(fragmentList)
    {
        PrintICT *visitorICT = new PrintICT();
        std::cout << "\n------------------------------" << std::endl;
        std::cout << "---- INTERMEDIATE CODE TREE ----" << std::endl;
        std::cout << "------------------------------\n" << std::endl;
        fragmentList->accept(visitorICT);
        Canonicalizer *canonicalizer = new Canonicalizer();

        fragmentList = canonicalizer->visit(fragmentList);
        while (canonicalizer->isChanged()) {
//        std::cout << "x" << std::endl;
//        fragmentList->accept(visitorICT);
            canonicalizer->change();
            fragmentList = fragmentList->accept(canonicalizer);

        }
        std::cout << "\n------------------------------" << std::endl;
        std::cout << "---- CANONICAL INTERMEDIATE CODE TREE ----" << std::endl;
        std::cout << "------------------------------\n" << std::endl;

        fragmentList->accept(visitorICT);
    }
}

void Translator::deleteTables() {
    delete this->varTable;
    delete this->structTable;
    delete this->functionTable;
}

