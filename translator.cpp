#include "translator.h"
#include "ast.h"
#include "symbol_table.h"

//FunctionSymbol *activeFunction = NULL;

void startTranslator(ProgramNode *ast) {
//    Translator translator;
//    translator.visit(ast);
//    fprintf(stderr, "\n");
}

void endTranslator() {
    //TODO ...
}

/********** TRANSLATOR **********/
void Translator::visit(ProgramNode *node) {
    if (node->getVarList() != nullptr) {
        node->getVarList()->accept(this);
    }
    if (node->getTypeList() != nullptr) {
        node->getTypeList()->accept(this);
    }
    if (node->getFunctions() != nullptr) {
        node->getFunctions()->accept(this);
    }
}

void Translator::visit(FunctionListNode *node) {
    if (node->getNext() != nullptr) {
        node->getNext()->accept(this);
    }
    if (node->getFunction() != nullptr) {
        node->getFunction()->accept(this);
    }
}

void Translator::visit(IdListNode *node) {
    if (node->getPointer() != nullptr) {
        node->getPointer()->accept(this);
    }
    if (node->getId() != nullptr) {
        node->getId()->accept(this);
    }
    if (node->getArray() != nullptr) {
        node->getArray()->accept(this);
    }
    if (node->getNext() != nullptr) {
        node->getNext()->accept(this);
    }
}

StmNode *Translator::visit(StmtListNode *node) {
    if (node->getStmt() != nullptr) {
        node->getStmt()->accept(this);
    }

    if (node->getNext() != nullptr) {
        return new SEQ(node->getStmt()->accept(this), node->getNext()->accept(this));
    }
    else {
        return node->getStmt()->accept(this);
    }

}

void Translator::visit(VarStmtNode *node) {
    if (node->getDecl() != nullptr) {
        node->getDecl()->accept(this);
    }
    if (node->getBody() != nullptr) {
        node->getBody()->accept(this);
    }
}

StmNode *Translator::visit(IfNode *node) {

    Label *thenLabel = new Label();
    Label *elseLabel = new Label();
    Label *endLabel = new Label();

    if (node->getHead() != nullptr) {
        node->getHead()->accept(this);
    }
    if (node->getTrueStmt() != nullptr) {
        node->getTrueStmt()->accept(this);
    }
    if (node->getFalseStmt() != nullptr) {
        node->getFalseStmt()->accept(this);
    }

    if (node->getFalseStmt() == nullptr) {
        return new SEQ(new CJUMP(EQ, node->getHead()->accept(this), new CONST(0), thenLabel, endLabel),
                       new SEQ(new LABEL(thenLabel),
                               new SEQ(node->getTrueStmt()->accept(this),
                                       new LABEL(endLabel))));
    } else {
        return new SEQ(new CJUMP(EQ, node->getHead()->accept(this), new CONST(0), thenLabel, elseLabel),
                       new SEQ(new LABEL(thenLabel),
                               new SEQ(new SEQ(node->getTrueStmt()->accept(this),
                                               new JUMP(new NAME(endLabel), nullptr)),
                                       new SEQ(new LABEL(elseLabel),
                                               new SEQ(node->getFalseStmt()->accept(this),
                                                       new LABEL(endLabel))))));
    }
}

StmNode *Translator::visit(WhileNode *node) {
    if (node->getHead() != nullptr) {
        node->getHead()->accept(this);
    }
    if (node->getBody() != nullptr) {
        node->getBody()->accept(this);
    }

    Label *testLabel = new Label();
    Label *startLabel = new Label();
    Label *endLabel = new Label();

    return new SEQ(new LABEL(testLabel),
                   new SEQ(new CJUMP(NE, node->getHead()->accept(this), new CONST(0), startLabel, endLabel),
                           new SEQ(new LABEL(startLabel),
                                   new SEQ(node->getBody()->accept(this),
                                           new SEQ(new JUMP(new NAME(testLabel), nullptr),
                                                   new LABEL(endLabel))))
                   ));

}

StmNode *Translator::visit(SwitchNode *node) {
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }
    if (node->getBlock() != nullptr) {
        node->getBlock()->accept(this);
    }
}

StmNode *Translator::visit(PrintNode *node) {
    if (node->getExpList() != nullptr) {
        node->getExpList()->accept(this);
    }
}

StmNode *Translator::visit(ReadLnNode *node) {
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }
}

StmNode *Translator::visit(ReturnNode *node) {
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }

}

void Translator::visit(CaseBlockNode *node) {
    if (node->getNum() != nullptr) {
        node->getNum()->accept(this);
    }
    if (node->getStmt() != nullptr) {
        node->getStmt()->accept(this);
    }
    if (node->getNext() != nullptr) {
        node->getNext()->accept(this);
    }
}

ExprNode *Translator::visit(ExpListNode *node) {
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }
    if (node->getNext() != nullptr) {
        node->getNext()->accept(this);
    }
}

StmNode *Translator::visit(TryNode *node) {
    if (node->getTry() != nullptr) {
        node->getTry()->accept(this);
    }
    if (node->getCatch() != nullptr) {
        node->getCatch()->accept(this);
    }
}

ExprNode *Translator::visit(PrimaryNode *node) {
    if (node->getTokenNode() != nullptr) {
        node->getTokenNode()->accept(this);
    }
    if (node->getExp() != nullptr) {
        return node->getExp()->accept(this);
    }
}

ExprNode *Translator::visit(CallNode *node) {
    if (node->getId() != nullptr) {
        node->getId()->accept(this);
    }
    if (node->getParameters() != nullptr) {
        node->getParameters()->accept(this);
    }
}

ExprNode *Translator::visit(ArrayCallNode *node) {
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }
    if (node->getIndex() != nullptr) {
        node->getIndex()->accept(this);
    }

//    return new MEM
}

ExprNode *Translator::visit(ArrayNode *node) {
    if (node->getNumInt() != nullptr) {
        node->getNumInt()->accept(this);
    }
}

ExprNode *Translator::visit(AssignNode *node) {

    if (node->getExp1() != nullptr) {
        node->getExp1()->accept(this);
    }
    if (node->getExp2() != nullptr) {
        node->getExp2()->accept(this);
    }
}

ExprNode *Translator::visit(AdditionOPNode *node) {
    if (node->getExp1() != nullptr) {
        node->getExp1()->accept(this);
    }
    if (node->getExp2() != nullptr) {
        node->getExp2()->accept(this);
    }

    return new BINOP(node->getOp()->getToken(), node->getExp1()->accept(this), node->getExp2()->accept(this));
}

ExprNode *Translator::visit(MultiplicationOPNode *node) {
    if (node->getExp1() != nullptr) {
        node->getExp1()->accept(this);
    }
    if (node->getExp2() != nullptr) {
        node->getExp2()->accept(this);
    }

    return new BINOP(node->getOp()->getToken(), node->getExp1()->accept(this), node->getExp2()->accept(this));
}


ExprNode *Translator::visit(BooleanOPNode *node) {
    if (node->getExp1() != nullptr) {
        node->getExp1()->accept(this);
    }
    if (node->getExp2() != nullptr) {
        node->getExp2()->accept(this);
    }


    if (node->getOp()->getToken() == AND) {
        Temp *r = new Temp();
        Label *l1 = new Label();
        Label *l2 = new Label();
        Label *l3 = new Label();

        return new ESEQ(new SEQ(new MOVE(new TEMP(r), new CONST(0)),
                                new SEQ(new CJUMP(NE, node->getExp1()->accept(this), new CONST(0), l1, l2),
                                        new SEQ(new LABEL(l1),
                                                new SEQ(new CJUMP(NE, node->getExp2()->accept(this), new CONST(0), l3,
                                                                  l2),
                                                        new SEQ(new LABEL(l3),
                                                                new SEQ(new MOVE(new TEMP(r), new CONST(1)),
                                                                        new LABEL(l2))))))),
                        new TEMP(r));
    } else if (node->getOp()->getToken() == OR) {
        Temp *r = new Temp();
        Label *l1 = new Label();
        Label *l2 = new Label();

        return new ESEQ(new SEQ(new MOVE(new TEMP(r), node->getExp1()->accept(this)),
                                new SEQ(new CJUMP(NE, node->getExp2()->accept(this), new CONST(0), l1, l2),
                                        new SEQ(new LABEL(l1),
                                                new SEQ(new MOVE(new TEMP(r), new CONST(1)),
                                                        new LABEL(l2))))),
                        new TEMP(r));
    } //TODO Não sei se precisa de verificar pq acho q n pode ser null, ja que veio do semantico
    else if (node->getOp()->getToken() == (LE || GE || GT || LT || EQ || NE)) {
        Temp *r = new Temp();
        Label *t = new Label();
        Label *f = new Label();
        //TODO SAO SO ESSES OPERADORES RELACIONAIS OU TEM MAIS?(na condicao do if)
        return new ESEQ(new SEQ(new MOVE(new TEMP(r), new CONST(1)),
                                new SEQ(new CJUMP(node->getOp()->getToken(), node->getExp1()->accept(this),
                                                  node->getExp2()->accept(this), t, f),
                                        new SEQ(new LABEL(f),
                                                new SEQ(new MOVE(new TEMP(r), new CONST(0)), new LABEL(t))))),
                        new TEMP(r));
    }
    return NULL;
}

ExprNode *Translator::visit(NotNode *node) {
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }

}

ExprNode *Translator::visit(SignNode *node) {
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }

    return new BINOP(MINUS, new CONST(0), node->getExp()->accept(this));

}

void Translator::visit(FormalListNode *node) {
    if (node->getType() != nullptr) {
        node->getType()->accept(this);
    }
    if (node->getPointer() != nullptr) {
        node->getPointer()->accept(this);
    }
    if (node->getId() != nullptr) {
        node->getId()->accept(this);
    }
    if (node->getArray() != nullptr) {
        node->getArray()->accept(this);
    }
    if (node->getNext() != nullptr) {
        node->getNext()->accept(this);
    }
}

void Translator::visit(FunctionNode *node) {
    if (node->getType() != nullptr) {
        node->getType()->accept(this);
    }
    if (node->getPointer() != nullptr) {
        node->getPointer()->accept(this);
    }
    if (node->getId() != nullptr) {
        node->getId()->accept(this);
    }
    //activeFunction = functionTable->cSearch(node->getId()->getLexeme());
    if (node->getParameters() != nullptr) {
        node->getParameters()->accept(this);
    }
    if (node->getLocal() != nullptr) {
        node->getLocal()->accept(this);
    }
    if (node->getBody() != nullptr) {
        node->getBody()->accept(this);
    }

}

ExprNode *Translator::visit(PointerExpNode *node) {
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }
    if (node->getId() != nullptr) {
        node->getId()->accept(this);
    }
}

ExprNode *Translator::visit(NameExpNode *node) {
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }
    if (node->getId() != nullptr) {
        node->getId()->accept(this);
    }
}

void Translator::visit(VarDeclNode *node) {
    if (node->getType() != nullptr) {
        node->getType()->accept(this);
    }
    if (node->getIdList() != nullptr) {
        node->getIdList()->accept(this);
    }
    if (node->getNext() != nullptr) {
        node->getNext()->accept(this);
    }

}

void Translator::visit(TypeDeclNode *node) {
    if (node->getDecl() != nullptr) {
        node->getDecl()->accept(this);
    }
    if (node->getId() != nullptr) {
        node->getId()->accept(this);
    }
    if (node->getNext() != nullptr) {
        node->getNext()->accept(this);
    }
}

ExprNode *Translator::visit(AddressValNode *node) {
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }
}

ExprNode *Translator::visit(PointerValNode *node) {
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }

    return new MEM(node->getExp()->accept(this));
}

//StmNode * Translator::visit(StmtNode *node) {
//    if (node->getStmt() != nullptr) {
//        node->getStmt()->accept(this);
//    }
//}


/*
 * TERMINAL NODES
 */
void Translator::visit(PointerNode *node) {
//    TODO return NULL;?
}

StmNode *Translator::visit(BreakNode *node) {
}

StmNode *Translator::visit(ThrowNode *node) {
    return NULL;
}

void Translator::visit(TypeNode *node) {
    // TODO return NULL?
}

ExprNode *Translator::visit(TokenNode *node) {
    if (node->getToken() == NUMINT) {
        return new CONST(atoi(node->getLexeme()));
    }

    if (node->getToken() == NUMFLOAT) {
        return new CONSTF(atof(node->getLexeme()));
    }

    if (node->getToken() == LITERALCHAR || node->getToken() == LITERAL) {
        Literal *literal = new Literal(node->getLexeme());
        //TODO verificar
        literal->setNext(fragmentList);
        fragmentList = literal;
        return new NAME(new Label(node->getLexeme()));
    }

    if (node->getToken() == TRUE) {
        return new CONST(1);
    }

    if (node->getToken() == FALSE) {
        return new CONST(0);
    }

    if (node->getToken() == ID) {
        //TODO verificar se eh variavel ou funcao?... na real acho que a verificação do ID não deve ser feita aqui por nao ter peso nenhum
        // verificar se eh variavel simples
        VarSymbol *aux = varTable->searchInScope(node->getLexeme(), "lexemaDaFuncaoAtual"/*TODO activeFunction->getLexeme()*/);
        if(aux)//se for diferente de nulo o id eh variavel
        {
            return new BINOP(PLUS, new TEMP(FP), new CONST(aux->getOffset()));
        }else
        {
            aux =varTable->cSearch(node->getLexeme());
            if(aux)
            {
                return new MEM(new BINOP(PLUS, new TEMP(FP), new CONST(aux->getOffset())));//TODO VERIFICAR FRAME POINTER E OFFSET e olhar no slide o codigo
            }
        }
        return NULL;
    }
}

Translator::Translator(VarTable *varTable, FunctionTable *functionTable, StructTable *structTable) {
this->varTable = varTable;
this->functionTable = functionTable;
this->structTable = structTable;
}
