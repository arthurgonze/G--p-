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

void Translator::visit(StmtListNode *node) {
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

void Translator::visit(IfNode *node) {
    if (node->getHead() != nullptr) {
        node->getHead()->accept(this);
    }
    if (node->getTrueStmt() != nullptr) {
        node->getTrueStmt()->accept(this);
    }
    if (node->getFalseStmt() != nullptr) {
        node->getFalseStmt()->accept(this);
    }
}

void Translator::visit(WhileNode *node) {
    if (node->getHead() != nullptr) {
        node->getHead()->accept(this);
    }
    if (node->getBody() != nullptr) {
        node->getBody()->accept(this);
    }
}

void Translator::visit(SwitchNode *node) {
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }
    if (node->getBlock() != nullptr) {
        node->getBlock()->accept(this);
    }
}

void Translator::visit(PrintNode *node) {
    if (node->getExpList() != nullptr) {
        node->getExpList()->accept(this);
    }
}

void Translator::visit(ReadLnNode *node) {
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }
}

void Translator::visit(ReturnNode *node) {
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

void Translator::visit(ExpListNode *node) {
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }
    if (node->getNext() != nullptr) {
        node->getNext()->accept(this);
    }
}

void Translator::visit(TryNode *node) {
    if (node->getTry() != nullptr) {
        node->getTry()->accept(this);
    }
    if (node->getCatch() != nullptr) {
        node->getCatch()->accept(this);
    }
}

void Translator::visit(PrimaryNode *node) {
    if (node->getTokenNode() != nullptr) {
        node->getTokenNode()->accept(this);
    }
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }
}

void Translator::visit(CallNode *node) {
    if (node->getId() != nullptr) {
        node->getId()->accept(this);
    }
    if (node->getParameters() != nullptr) {
        node->getParameters()->accept(this);
    }
}

void Translator::visit(ArrayCallNode *node) {
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }
    if (node->getIndex() != nullptr) {
        node->getIndex()->accept(this);
    }
}

void Translator::visit(ArrayNode *node) {
    if (node->getNumInt() != nullptr) {
        node->getNumInt()->accept(this);
    }
}

void Translator::visit(AssignNode *node) {
    if (node->getExp1() != nullptr) {
        node->getExp1()->accept(this);
    }
    if (node->getExp2() != nullptr) {
        node->getExp2()->accept(this);
    }
}

void Translator::visit(AdditionOPNode *node) {
    if (node->getExp1() != nullptr) {
        node->getExp1()->accept(this);
    }
    if (node->getExp2() != nullptr) {
        node->getExp2()->accept(this);
    }
}

void Translator::visit(MultiplicationOPNode *node) {
    if (node->getExp1() != nullptr) {
        node->getExp1()->accept(this);
    }
    if (node->getExp2() != nullptr) {
        node->getExp2()->accept(this);
    }
}

void Translator::visit(BooleanOPNode *node) {
    if (node->getExp1() != nullptr) {
        node->getExp1()->accept(this);
    }
    if (node->getExp2() != nullptr) {
        node->getExp2()->accept(this);
    }
}

void Translator::visit(NotNode *node) {
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }
}

void Translator::visit(SignNode *node) {
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

void Translator::visit(PointerExpNode *node) {
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }
    if (node->getId() != nullptr) {
        node->getId()->accept(this);
    }
}

void Translator::visit(NameExpNode *node) {
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

void Translator::visit(AddressValNode *node) {
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }
}

void Translator::visit(PointerValNode *node) {
    if (node->getExp() != nullptr) {
        node->getExp()->accept(this);
    }
}

void Translator::visit(StmtNode *node) {
    if (node->getStmt() != nullptr) {
        node->getStmt()->accept(this);
    }
}


/*
 * TERMINAL NODES
 */
void Translator::visit(PointerNode *node) {
}

void Translator::visit(BreakNode *node) {
}

void Translator::visit(ThrowNode *node) {
}

void Translator::visit(TypeNode *node) {
}

void Translator::visit(TokenNode *node) {
}