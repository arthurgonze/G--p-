#include "translator.h"
#include "ast.h"

void startTranslator(ProgramNode *ast) {
    Translator translator;
    translator.visit(ast);
    fprintf(stderr, "\n");
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

StmNode * Translator::visit(StmtListNode *node) {
    if (node->getStmt() != nullptr) {
        node->getStmt()->accept(this);
    }
    if (node->getNext() != nullptr) {
        node->getNext()->accept(this);
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

StmNode * Translator::visit(IfNode *node) {

    Label* thenLabel = new Label();
    Label* endLabel = new Label();

    if (node->getHead() != nullptr) {
        node->getHead()->accept(this);
    }
    if (node->getTrueStmt() != nullptr) {
        node->getTrueStmt()->accept(this);
    }
    if (node->getFalseStmt() != nullptr) {
        node->getFalseStmt()->accept(this);
    }

    if(node->getFalseStmt() == nullptr) {
        return new SEQ(new CJUMP(EQ, node->getHead()->accept(this), new CONST(0), thenLabel , endLabel),
                       new SEQ(new LABEL(thenLabel), new SEQ(node->getTrueStmt()->accept(this), new LABEL(endLabel))));
    }
}

StmNode * Translator::visit(WhileNode *node) {
    if (node->getHead() != nullptr) {
        node->getHead()->accept(this);
    }
    if (node->getBody() != nullptr) {
        node->getBody()->accept(this);
    }
}

StmNode * Translator::visit(SwitchNode *node) {
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }
    if (node->getBlock() != nullptr) {
        node->getBlock()->accept(this);
    }
}

StmNode * Translator::visit(PrintNode *node) {
    if (node->getExpList() != nullptr) {
        node->getExpList()->accept(this);
    }
}

StmNode * Translator::visit(ReadLnNode *node) {
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }
}

StmNode * Translator::visit(ReturnNode *node) {
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

ExprNode * Translator::visit(ExpListNode *node) {
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }
    if (node->getNext() != nullptr) {
        node->getNext()->accept(this);
    }
}

StmNode * Translator::visit(TryNode *node) {
    if (node->getTry() != nullptr) {
        node->getTry()->accept(this);
    }
    if (node->getCatch() != nullptr) {
        node->getCatch()->accept(this);
    }
}

ExprNode * Translator::visit(PrimaryNode *node) {
    if (node->getTokenNode() != nullptr) {
        node->getTokenNode()->accept(this);
    }
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }
}

ExprNode * Translator::visit(CallNode *node) {
    if (node->getId() != nullptr) {
        node->getId()->accept(this);
    }
    if (node->getParameters() != nullptr) {
        node->getParameters()->accept(this);
    }
}

ExprNode * Translator::visit(ArrayCallNode *node) {
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }
    if (node->getIndex() != nullptr) {
        node->getIndex()->accept(this);
    }
}

ExprNode * Translator::visit(ArrayNode *node) {
    if (node->getNumInt() != nullptr) {
        node->getNumInt()->accept(this);
    }
}

ExprNode * Translator::visit(AssignNode *node) {
    if (node->getExp1() != nullptr) {
        node->getExp1()->accept(this);
    }
    if (node->getExp2() != nullptr) {
        node->getExp2()->accept(this);
    }
}

ExprNode * Translator::visit(AdditionOPNode *node) {
    if (node->getExp1() != nullptr) {
        node->getExp1()->accept(this);
    }
    if (node->getExp2() != nullptr) {
        node->getExp2()->accept(this);
    }
}

ExprNode * Translator::visit(MultiplicationOPNode *node) {
    if (node->getExp1() != nullptr) {
        node->getExp1()->accept(this);
    }
    if (node->getExp2() != nullptr) {
        node->getExp2()->accept(this);
    }
}


ExprNode* Translator::visit(BooleanOPNode *node) {
    if (node->getExp1() != nullptr) {
        node->getExp1()->accept(this);
    }
    if (node->getExp2() != nullptr) {
        node->getExp2()->accept(this);
    }

    Temp* r = new Temp();
    Label* t = new Label();
    Label* f = new Label();

    return new ESEQ(new SEQ(new MOVE(new TEMP(r), new CONST(1)),
                            new SEQ(new CJUMP(node->getOp()->getToken(), node->getExp1()->accept(this), node->getExp2()->accept(this), t, f),
                                    new SEQ(new LABEL(f), new SEQ(new MOVE(new TEMP(r), new CONST(0)), new LABEL(t))))), new TEMP(r));
}

ExprNode * Translator::visit(NotNode *node) {
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }
}

ExprNode * Translator::visit(SignNode *node) {
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }
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

ExprNode * Translator::visit(PointerExpNode *node) {
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }
    if (node->getId() != nullptr) {
        node->getId()->accept(this);
    }
}

ExprNode * Translator::visit(NameExpNode *node) {
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

ExprNode * Translator::visit(AddressValNode *node) {
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }
}

ExprNode * Translator::visit(PointerValNode *node) {
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }
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
}

StmNode * Translator::visit(BreakNode *node) {
}

StmNode * Translator::visit(ThrowNode *node) {
}

void Translator::visit(TypeNode *node) {
}

ExprNode * Translator::visit(TokenNode *node) {
}