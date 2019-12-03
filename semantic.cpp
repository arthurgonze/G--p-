#include "semantic.h"

#define SCOPE_NON_LOCAL "nonLocal"
#define UNASSIGNED_TOKEN -42

VarTable *varTable;
FunctionTable *functionTable;
StructTable *structTable;
FunctionSymbol *activeFunction = NULL;

int breakDepth = 0;

void startSemantic(ProgramNode *ast) {
    varTable = new VarTable();
    functionTable = new FunctionTable();
    structTable = new StructTable();

    SemanticTables semanticVisitor;
    SemanticTypes semanticTypes;

    semanticVisitor.visit(ast);
    semanticTypes.visit(ast);
    fprintf(stderr, "\n");

}

void endSemantic() {
    activeFunction = NULL;
    varTable->print();
    functionTable->print();
    structTable->print();
    delete varTable;
    delete functionTable;
    delete structTable;
}

void beginScope(const char *scopeLex) {
    varTable->beginScope(scopeLex);
    functionTable->beginScope(scopeLex);
    structTable->beginScope(scopeLex);
}

void endScope() {
    varTable->endScope();
    functionTable->endScope();
    structTable->endScope();
}

bool _match_exp_types(ExpNode *exp1, ExpNode *exp2) {

    bool isSameType = exp1->getType() == exp2->getType();
    bool isSamePointer = exp1->isPointer() == exp2->isPointer();
    bool isArray = (exp1->getArraySize() >= 0) == (exp2->getArraySize() >= 0);
    bool isSameTypeLexeme = true;

    if (exp1->getType() == ID)
        isSameTypeLexeme = !strcmp(exp1->getTypeLexeme(), exp2->getTypeLexeme());

    return (isSameType && isSameTypeLexeme && isSamePointer && isArray);
}

void SemanticTables::visit(ProgramNode *programNode) {
    if (programNode->getTypeList()) {
        programNode->getTypeList()->accept(this);
    }
    if (programNode->getVarList()) {
        programNode->getVarList()->accept(this);
    }
    if (programNode->getFunctions()) {
        programNode->getFunctions()->accept(this);
    }
}

void SemanticTables::visit(TypeDeclNode *typeDeclNode) {

    if (typeDeclNode->getNext() != NULL) {
        typeDeclNode->getNext()->accept(this);
    }

    if (!typeDeclNode->getId()->getLexeme()) {
        fprintf(stderr, "[SEMANTIC ERROR - typeDeclNode] STRUCT WITHOUT A VALID NAME, line: %d\n",
                typeDeclNode->getLine());
        return;
    }
    if (structTable->cSearch(typeDeclNode->getId()->getLexeme()) ||
        !structTable->cInsert(typeDeclNode->getId()->getLexeme(), typeDeclNode->getDecl())) {
        fprintf(stderr, "[SEMANTIC ERROR - typeDeclNode] STRUCT ALREADY EXISTS, line: %d, lexeme: %s \n",
                typeDeclNode->getLine(), typeDeclNode->getId()->getLexeme());
        return;
    }

    beginScope(typeDeclNode->getId()->getLexeme());
    if (typeDeclNode->getDecl() != NULL) {
        typeDeclNode->getDecl()->accept(this);
        VarDeclNode *varListAux = typeDeclNode->getDecl();
        int totalSizeAux = 0;
        while (varListAux != NULL) {
            IdListNode *idListAux = varListAux->getIdList();
            while (idListAux != NULL) {
                VarSymbol *var = varTable->searchInScope(idListAux->getId()->getLexeme(),
                                                         typeDeclNode->getId()->getLexeme());
                int sizeAux = BYTE_SIZE;
                if (idListAux->getArray() != NULL) {
                    sizeAux *= atoi(idListAux->getArray()->getNumInt()->getLexeme());
                }
                var->setSize(sizeAux);
                var->setOffset(totalSizeAux);
                totalSizeAux += sizeAux;
                idListAux = idListAux->getNext();
            }
            if (varListAux->getNext()) {
                varListAux = varListAux->getNext()->getNext();
            } else {
                varListAux = NULL;
            }
        }
        StructSymbol *structSymbol = structTable->cSearch(typeDeclNode->getId()->getLexeme());
        structSymbol->setSize(totalSizeAux);
    }
    endScope();
}

void SemanticTables::visit(TokenNode *tokenNode) {
    if (tokenNode->getToken() != ID)
        tokenNode->setTypeLexeme(token_id_to_name(tokenNode->getToken()));
}

void SemanticTables::visit(IdListNode *idListNode) {
    if (idListNode->getId() != NULL) {
        idListNode->getId()->accept(this);
        idListNode->getId()->setPointer(idListNode->getPointer() != NULL);
        if (idListNode->getArray() != NULL) {
            idListNode->getArray()->accept(this);
            idListNode->getId()->setArraySize(idListNode->getArray()->getArraySize());
        }
    }
    if (idListNode->getNext() != NULL) {
        idListNode->getNext()->accept(this);
    }
    if (idListNode->getArray() != NULL) {
        idListNode->getArray()->accept(this);
    }
    TypeNode *typeNode = new TypeNode(idListNode->getId(), idListNode->getId()->getTypeLexeme());
    typeNode->setLine(idListNode->getLine());


}


void SemanticTables::visit(VarDeclNode *varDeclNode) {

    if (varDeclNode->getNext() != NULL) {
        varDeclNode->getNext()->accept(this);
    }

    if (varDeclNode->getIdList() != NULL) {
        varDeclNode->getIdList()->accept(this);
    }

    IdListNode *idListAux = varDeclNode->getIdList();
    while (idListAux != NULL) {

        bool isPointer = idListAux->getId()->isPointer() || (idListAux->getPointer() != NULL);

        idListAux->getId()->setType(varDeclNode->getType()->getId()->getToken());
        idListAux->getId()->setTypeLexeme(varDeclNode->getType()->getId()->getLexeme());
        idListAux->getId()->setPointer(isPointer);
        idListAux->getId()->setArraySize(varDeclNode->getType()->getId()->getArraySize());
        if (varDeclNode->getType()->getType() == ID) {
            if (!structTable->cSearch(idListAux->getId()->getTypeLexeme())) {
                fprintf(stderr, "[SEMANTIC ERROR - VarDeclNode] UNKNOWN VARIABLE TYPE line: %d type: %s \n",
                        idListAux->getLine(), idListAux->getId()->getTypeLexeme());

                return;
            }

        }

        int arraySize = -1;

        if (idListAux->getArray()) {
            arraySize = atoi(idListAux->getArray()->getLexeme());
        }

        if (!varTable->cInsert(varDeclNode->getType(), idListAux->getId()->getLexeme(), isPointer,
                               arraySize, BOOL_FALSE)) {
            fprintf(stderr, "[SEMANTIC ERROR - VarDeclNode] DUPLICATED VARIABLE NAME line: %d lexeme: %s \n",
                    idListAux->getLine(), idListAux->getId()->getLexeme());

        }
        idListAux = idListAux->getNext();
    }

}

void SemanticTables::visit(FormalListNode *formalListNode) {

    if (formalListNode->getId() != NULL) {
        formalListNode->getId()->accept(this);
    }

    if (!formalListNode->getId() || !formalListNode->getId()->getLexeme()) {
        fprintf(stderr, "[SEMANTIC ERROR - formalListNode] FUNCTION PARAMETER WITHOUT A NAME, line: %d\n",
                formalListNode->getLine());
    }

    const char *parameterName = formalListNode->getId()->getLexeme();
    int arraySize = -1;

    if (formalListNode->getArray())
        arraySize = formalListNode->getArray()->getArraySize();

    if (!varTable->cInsert(formalListNode->getType(), parameterName, formalListNode->getPointer() != NULL, arraySize,
                           BOOL_TRUE)) {
        fprintf(stderr, "[SEMANTIC ERROR - formalListNode] VARIABLE ALREADY EXISTS, line: %d, lexeme: %s \n",
                formalListNode->getLine(), formalListNode->getId()->getLexeme());
    }


    if (formalListNode->getNext() != NULL) {
        formalListNode->getNext()->accept(this);
    }
}

void SemanticTables::visit(FunctionNode *functionNode) {

    if (functionNode->getId() != NULL) {
        functionNode->getId()->accept(this);
    }


    if (!functionNode->getId() || !functionNode->getId()->getLexeme()) {
        fprintf(stderr, "[SEMANTIC ERROR - functionNode] FUNCTION WITHOUT A VALID NAME, line: %d\n",
                functionNode->getLine());
        return;
    }
    const char *functionLexeme = functionNode->getId()->getLexeme();

    if (varTable->searchInScope(functionLexeme, SCOPE_NON_LOCAL) || structTable->cSearch(functionLexeme)) {
        fprintf(stderr,
                "[SEMANTIC ERROR - functionNode] FUNCTION NAME ALREADY ASSIGNED TO STRUCT OR NON LOCAL VAR, line: %d\n",
                functionNode->getLine());
        return;
    }

    if (functionNode->getReturnType() == ID) {

        if (!structTable->cSearch(functionNode->getReturnTypeLexeme())) {
            fprintf(stderr, "[SEMANTIC ERROR - functionNode] UNDEFINED RETURN TYPE, line: %d type: %s\n",
                    functionNode->getLine(), functionNode->getReturnTypeLexeme());

        }
    }

    if (!functionTable->cInsert(functionNode->getType(), functionLexeme,
                                functionNode->getParameters(), functionNode->getPointer())) {
        fprintf(stderr, "[SEMANTIC ERROR - functionNode] FUNCTION NAME ALREADY ASSIGNED BEFORE, line: %d\n",
                functionNode->getLine());
        return;
    }

    activeFunction = functionTable->cSearch(functionLexeme);
    beginScope(functionLexeme);

    if (functionNode->getParameters() != NULL) {
        functionNode->getParameters()->accept(this);
    }
    if (functionNode->getLocal() != NULL) {
        functionNode->getLocal()->accept(this);
    }

    endScope();
}

void SemanticTables::visit(FunctionListNode *functionListNode) {
    if (functionListNode->getNext()) {
        functionListNode->getNext()->accept(this);
    }
    if (functionListNode->getFunction()) {
        functionListNode->getFunction()->accept(this);
    }
}

void SemanticTypes::visit(ProgramNode *programNode) {

    if (programNode->getFunctions()) {
        programNode->getFunctions()->accept(this);
    }
}

void SemanticTypes::visit(FunctionNode *functionNode) {

    if (functionNode->getId() != NULL) {
        functionNode->getId()->accept(this);
    }

    if (!functionNode->getId() || !functionNode->getId()->getLexeme() ||
        !functionTable->cSearch(functionNode->getId()->getLexeme()))
        return;

    const char *functionLexeme = functionNode->getId()->getLexeme();

    activeFunction = functionTable->cSearch(functionLexeme);
    beginScope(functionLexeme);

    if (functionNode->getBody() != NULL) {
        functionNode->getBody()->accept(this);
    }

    if(functionNode->getBody() == NULL || !functionNode->getBody()->isReturn())
    {
        fprintf(stderr,
                "[SEMANTIC ERROR - FunctionNode] RETURN EXPECTED, line: %d\n",
                functionNode->getLine());
    }
    endScope();
}

void SemanticTypes::visit(FunctionListNode *functionListNode) {

    if (functionListNode->getNext()) {
        functionListNode->getNext()->accept(this);
    }
    if (functionListNode->getFunction()) {
        functionListNode->getFunction()->accept(this);
    }
}

void SemanticTypes::visit(BreakNode *breakNode) {
    if (!breakDepth) {
        fprintf(stderr, "[SEMANTIC ERROR - breakNode] UNEXPECTED BREAK, line: %d \n", breakNode->getLine());
    }
}


void SemanticTypes::visit(WhileNode *whileNode) {
    if (whileNode->getHead()) {
        whileNode->getHead()->accept(this);
    } else {
        fprintf(stderr,
                "[SEMANTIC ERROR - whileNode] IF WITHOUT A VALID CONDITION, line: %d\n",
                whileNode->getLine());
        return;
    }

    if (whileNode->getBody()) {
        breakDepth += 1;
        whileNode->getBody()->accept(this);
        breakDepth -= 1;
    }

    if (whileNode->getHead()->getType() != BOOL) {
        fprintf(stderr,
                "[SEMANTIC ERROR - whileNode] CONDITION EXPRESSION MUST BE BOOLEAN, line: %d\n",
                whileNode->getLine());
    }

    whileNode->setReturn(whileNode->getBody() && whileNode->getBody()->isReturn());

}

void SemanticTypes::visit(PrintNode *printNode) {

    if (printNode->getExpList())
        printNode->getExpList()->accept(this);

    printNode->setReturn(false);
}

void SemanticTypes::visit(ThrowNode *throwNode) {

}

void SemanticTypes::visit(SwitchNode *switchNode) {

    if (switchNode->getExp() != NULL) {
        switchNode->getExp()->accept(this);
        if (switchNode->getExp()->getType() != INT) {
            fprintf(stderr, "[SEMANTIC ERROR - switchNode] CONDITION MUST BE INT, line: %d\n", switchNode->getLine());
        }

        switchNode->setLine(switchNode->getExp()->getLine());
    } else {
        fprintf(stderr, "[SEMANTIC ERROR - switchNode] NO VALID CONDITION, line: %d\n", switchNode->getLine());
    }
    if (switchNode->getBlock() != NULL) {
        breakDepth += 1;
        switchNode->getBlock()->accept(this);
        breakDepth -= 1;
    } else {
        fprintf(stderr, "[SEMANTIC ERROR - switchNode] CASE STATEMENT REQUIRED, line: %d\n", switchNode->getLine());
    }
}

void SemanticTypes::visit(TryNode *tryNode) {

    if (tryNode->getTry()) {
        tryNode->getTry()->accept(this);
    }

    if (tryNode->getCatch()) {
        tryNode->getCatch()->accept(this);
    }

}


void SemanticTypes::visit(ReadLnNode *readLnNode) {

    if (readLnNode->getExp()) {
        readLnNode->getExp()->accept(this);
    }

}

void SemanticTypes::visit(IfNode *ifNode) {
    if (ifNode->getHead()) {
        ifNode->getHead()->accept(this);
    } else {
        fprintf(stderr,
                "[SEMANTIC ERROR - ifNode] IF WITHOUT A VALID CONDITION, line: %d\n",
                ifNode->getLine());
        return;
    }

    if (ifNode->getTrueStmt()) {
        ifNode->getTrueStmt()->accept(this);
    }

    if (ifNode->getFalseStmt()) {
        ifNode->getFalseStmt()->accept(this);
    }

    if (ifNode->getHead()->getType() != BOOL) {
        fprintf(stderr,
                "[SEMANTIC ERROR - ifNode] CONDITION EXPRESSION MUST BE BOOLEAN, line: %d\n",
                ifNode->getLine());
    }

    ifNode->setReturn(ifNode->getFalseStmt() && ifNode->getTrueStmt() && ifNode->getTrueStmt()->isReturn() &&
                      ifNode->getFalseStmt()->isReturn());
}

void SemanticTypes::visit(StmtListNode *stmtListNode) {

    bool aux_return = false;
    if (stmtListNode->getNext()) {
        stmtListNode->getNext()->accept(this);
        aux_return = stmtListNode->getNext()->isReturn();
    }

    if (stmtListNode->getStmt()) {
        stmtListNode->getStmt()->accept(this);
        stmtListNode->setReturn(stmtListNode->getStmt()->isReturn());
        aux_return = aux_return || stmtListNode->isReturn();
    }

    stmtListNode->setReturn(aux_return);

}

void SemanticTypes::visit(ReturnNode *returnNode) {
    if (returnNode->getExp()) { //Check the return expression
        returnNode->getExp()->accept(this);
    } else { //No expression means void return

        fprintf(stderr,
                "[SEMANTIC ERROR - returnNode] VOID RETURN STATEMENT NOT ALLOWED, line: %d\n",
                returnNode->getLine());


        return;
    }

    if (!activeFunction) {
        fprintf(stderr,
                "[SEMANTIC ERROR - returnNode] RETURN STATEMENT OUTSIDE A FUNCTION, line: %d\n",
                returnNode->getLine());
        return;
    }


    bool isSameType = returnNode->getExp()->getType() ==
                      activeFunction->getReturnType()->getType();

    bool isSameArray = returnNode->getExp()->getArraySize() < 0;
    bool isSamePointer = returnNode->getExp()->isPointer() == activeFunction->isPointer();
    bool isSameTypeLexeme = true;
    if (returnNode->getExp()->getType() == ID)
        isSameTypeLexeme = !strcmp(returnNode->getExp()->getTypeLexeme(),
                                   activeFunction->getReturnType()->getTypeLexeme());

    if (!(isSameArray && isSameType && isSamePointer && isSameTypeLexeme)) {
        fprintf(stderr,
                "[SEMANTIC ERROR - returnNode] RETURN TYPE DOES NOT MATCH WITH FUNCTION RETURN TYPE, line: %d\n",
                returnNode->getLine());
        return;
    }

    returnNode->setReturn(true);
}

void SemanticTypes::visit(StmtNode *stmtNode) {
    if (stmtNode->getStmt()) {
        stmtNode->getStmt()->accept(this);
    }
}

void SemanticTypes::visit(AssignNode *assignNode) {
    if (assignNode->getExp1())
        assignNode->getExp1()->accept(this);
    else {
        fprintf(stderr,
                "[SEMANTIC ERROR - assignNode] NO EXPRESSION TO BE ASSIGNED, line: %d\n",
                assignNode->getLine());
        return;
    }

    if (assignNode->getExp2())
        assignNode->getExp2()->accept(this);
    else {
        fprintf(stderr,
                "[SEMANTIC ERROR - assignNode] NO EXPRESSION TO ASSIGN, line: %d\n",
                assignNode->getLine());
        return;
    }


    if (!_match_exp_types(assignNode->getExp1(), assignNode->getExp2())) {
        fprintf(stderr,
                "[SEMANTIC ERROR - assignNode] CANNOT ASSIGN AN EXPRESSION TO A DIFFERENT TYPE, line: %d\n",
                assignNode->getLine());
        return;
    }

    if (!assignNode->getExp1()->isLValue()) {
        fprintf(stderr,
                "[SEMANTIC ERROR - assignNode] LVALUE EXPECTED, line: %d expected: %s got: %s\n",
                assignNode->getExp1()->getLine(), assignNode->getExp1()->getTypeLexeme(),
                assignNode->getExp1()->getTypeLexeme());
        return;
    }

}

void SemanticTypes::visit(PointerValNode *pointerValNode) {

    if (pointerValNode->getExp()) {
        pointerValNode->getExp()->accept(this);
    } else {
        fprintf(stderr, "[SEMANTIC ERROR - pointerValNode] INVALID VALUE EXPRESSION, line %d\n",
                pointerValNode->getLine());
        return;
    }

    if (!pointerValNode->getExp()->isPointer()) {
        fprintf(stderr, "[SEMANTIC ERROR - pointerValNode] CANNOT ACCESS VALUE OF A NON POINTER TYPE, line %d\n",
                pointerValNode->getLine());
        return;
    }

    pointerValNode->setType(pointerValNode->getExp()->getType());
    pointerValNode->setTypeLexeme(pointerValNode->getExp()->getTypeLexeme());
    pointerValNode->setLValue(true);
    pointerValNode->setArraySize(pointerValNode->getExp()->getArraySize());
    pointerValNode->setPointer(false);

}

/*
 * Address verification
 */

void SemanticTypes::visit(AddressValNode *addressValNode) {

    if (addressValNode->getExp()) {
        addressValNode->getExp()->accept(this);
    } else {
        fprintf(stderr, "[SEMANTIC ERROR - addressValNode] INVALID VALUE EXPRESSION, line %d\n",
                addressValNode->getLine());
        return;
    }

    if (addressValNode->getExp()->isPointer()) {
        fprintf(stderr,
                "[SEMANTIC ERROR - addressValNode] CANNOT ACCESS ADDRESS OF A POINTER TYPE, line %d lexeme: %s\n",
                addressValNode->getLine(), addressValNode->getLexeme());
        return;
    }

    addressValNode->setType(addressValNode->getExp()->getType());
    addressValNode->setTypeLexeme(addressValNode->getExp()->getTypeLexeme());
    addressValNode->setLValue(true);
    addressValNode->setArraySize(addressValNode->getExp()->getArraySize());
    addressValNode->setPointer(true);
}

/*
 * Pointer access verification
 */

void SemanticTypes::visit(PointerExpNode *pointerExpNode) {
    if (pointerExpNode->getId()) {
        pointerExpNode->getId()->accept(this);
    } else {
        fprintf(stderr, "[SEMANTIC ERROR - pointerExpNode] NO RECORD ACCESS EXPRESSION, line %d\n",
                pointerExpNode->getExp()->getLine());
        return;
    }

    if (pointerExpNode->getExp()) {
        pointerExpNode->getExp()->accept(this);
    } else {
        fprintf(stderr, "[SEMANTIC ERROR - pointerExpNode] NO STRUCT ACCESS FIELD, line %d\n",
                pointerExpNode->getExp()->getLine());
        return;
    }

    if (!pointerExpNode->getExp()->isPointer() || pointerExpNode->getExp()->getArraySize() > 0) {
        fprintf(stderr, "[SEMANTIC ERROR - pointerExpNode] INCORRECT ACCESS TO A STRUCT POINTER, line %d\n",
                pointerExpNode->getExp()->getLine());
        return;
    }
    VarSymbol *recordField = varTable->searchInScope(pointerExpNode->getId()->getLexeme(),
                                                     pointerExpNode->getExp()->getTypeLexeme());

    if (!recordField) {
        fprintf(stderr, "[SEMANTIC ERROR - pointerExpNode] UNKNOWN FIELD IN STRUCT, line: %d field: %s type: %s\n",
                pointerExpNode->getExp()->getLine(), pointerExpNode->getId()->getLexeme(),
                pointerExpNode->getExp()->getTypeLexeme());
        return;
    }

    pointerExpNode->setType(recordField->getType()->getType());
    pointerExpNode->setTypeLexeme(recordField->getType()->getTypeLexeme());
    pointerExpNode->setPointer(recordField->isPointer());
    pointerExpNode->setArraySize(recordField->getArraySize());
    pointerExpNode->setLValue(true);
}

/*
 * Name access verification
 */
void SemanticTypes::visit(NameExpNode *nameExpNode) {
    if (nameExpNode->getId()) {
        nameExpNode->getId()->accept(this);
    } else {
        fprintf(stderr, "[SEMANTIC ERROR - nameExpNode] NO RECORD ACCESS EXPRESSION, line %d\n",
                nameExpNode->getExp()->getLine());
        return;
    }

    if (nameExpNode->getExp()) {
        nameExpNode->getExp()->accept(this);
    } else {
        fprintf(stderr, "[SEMANTIC ERROR - nameExpNode] NO STRUCT ACCESS FIELD, line %d\n",
                nameExpNode->getExp()->getLine());
        return;
    }

    if (nameExpNode->getExp()->isPointer() || nameExpNode->getExp()->getArraySize() > 0) {
        fprintf(stderr, "[SEMANTIC ERROR - nameExpNode] INCORRECT ACCESS TO A STRUCT, line %d\n",
                nameExpNode->getExp()->getLine());
        return;
    }
    VarSymbol *recordField = varTable->searchInScope(nameExpNode->getId()->getLexeme(),
                                                     nameExpNode->getExp()->getTypeLexeme());

    if (!recordField) {
        fprintf(stderr, "[SEMANTIC ERROR - nameExpNode] UNKNOWN FIELD IN STRUCT, line: %d field: %s type: %s\n",
                nameExpNode->getExp()->getLine(), nameExpNode->getId()->getLexeme(),
                nameExpNode->getExp()->getTypeLexeme());
        return;
    }

    nameExpNode->setType(recordField->getType()->getType());
    nameExpNode->setTypeLexeme(recordField->getType()->getTypeLexeme());
    nameExpNode->setPointer(recordField->isPointer());
    nameExpNode->setArraySize(recordField->getArraySize());
    nameExpNode->setLValue(true);
}

/*
 * Check if the array call is okay or if have error on array access
 */

void SemanticTypes::visit(ArrayCallNode *arrayCallNode) {
    if (arrayCallNode->getExp()) {
        arrayCallNode->getExp()->accept(this);
    } else {
        fprintf(stderr, "[SEMANTIC ERROR - arrayCallNode] INVALID ARRAY ACCESS EXPRESSION, line %d\n",
                arrayCallNode->getLine());
        return;
    }

    if (arrayCallNode->getIndex()) {
        arrayCallNode->getIndex()->accept(this);
    } else {
        fprintf(stderr, "[SEMANTIC ERROR - arrayCallNode] INVALID ARRAY INDEX EXPRESSION, line %d\n",
                arrayCallNode->getLine());
        return;
    }

    if (arrayCallNode->getExp()->getArraySize() < 0) {
        fprintf(stderr, "[SEMANTIC ERROR - arrayCallNode] ARRAY ACCESS TO A NON ARRAY TYPE, line %d\n",
                arrayCallNode->getLine());
        return;
    }


    if (arrayCallNode->getIndex()->getType() != INT) {
        fprintf(stderr, "[SEMANTIC ERROR - arrayCallNode] ARRAY ACCESS WITH NON INT EXPRESSION, line %d type: %s\n",
                arrayCallNode->getLine(), arrayCallNode->getTypeLexeme());
        return;
    }

    //Set node type to the single element type
    arrayCallNode->setType(arrayCallNode->getExp()->getType());
    arrayCallNode->setTypeLexeme(arrayCallNode->getExp()->getTypeLexeme());
    arrayCallNode->setPointer(arrayCallNode->getExp()->isPointer());
    arrayCallNode->setArraySize(-1);
}

/*
 * Assing Multiplication Expression verification
 */

void SemanticTypes::visit(MultiplicationOPNode *multiplicationOpNode) {

    if (multiplicationOpNode->getExp1())
        multiplicationOpNode->getExp1()->accept(this);
    else {
        fprintf(stderr,
                "[SEMANTIC ERROR - multiplicationOpNode] NO EXPRESSION TO BE ASSIGNED, line: %d\n",
                multiplicationOpNode->getLine());
        return;
    }

    if (multiplicationOpNode->getExp2())
        multiplicationOpNode->getExp2()->accept(this);
    else {
        fprintf(stderr,
                "[SEMANTIC ERROR - multiplicationOpNode] NO EXPRESSION TO ASSIGN, line: %d\n",
                multiplicationOpNode->getLine());
        return;
    }


    if (!_match_exp_types(multiplicationOpNode->getExp1(), multiplicationOpNode->getExp2())) {
        fprintf(stderr,
                "[SEMANTIC ERROR - multiplicationOpNode] CANNOT ASSIGN AN EXPRESSION TO A DIFFERENT TYPE, line: %d\n",
                multiplicationOpNode->getLine());
        return;
    }

    if (multiplicationOpNode->getExp1()->getType() != INT && multiplicationOpNode->getExp1()->getType() != FLOAT) {
        fprintf(stderr,
                "[SEMANTIC ERROR - multiplicationOpNode] CANNOT OPERATE WITH NON NUMERIC VALUES, line: %d\n",
                multiplicationOpNode->getLine());
        return;
    }

    multiplicationOpNode->setType(multiplicationOpNode->getExp1()->getType());
    multiplicationOpNode->setTypeLexeme(multiplicationOpNode->getExp1()->getTypeLexeme());
    multiplicationOpNode->setPointer(multiplicationOpNode->getExp1()->isPointer());
    multiplicationOpNode->setArraySize(multiplicationOpNode->getExp1()->getArraySize());
    multiplicationOpNode->setLValue(false);
}

/*
 * Assing Addition Expression verification
 */

void SemanticTypes::visit(AdditionOPNode *additionOpNode) {

    if (additionOpNode->getExp1())
        additionOpNode->getExp1()->accept(this);
    else {
        fprintf(stderr,
                "[SEMANTIC ERROR - additionOpNode] NO EXPRESSION TO BE ASSIGNED, line: %d\n",
                additionOpNode->getLine());
        return;
    }

    if (additionOpNode->getExp2())
        additionOpNode->getExp2()->accept(this);
    else {
        fprintf(stderr,
                "[SEMANTIC ERROR - additionOpNode] NO EXPRESSION TO ASSIGN, line: %d\n",
                additionOpNode->getLine());
        return;
    }


    if (!_match_exp_types(additionOpNode->getExp1(), additionOpNode->getExp2())) {
        fprintf(stderr,
                "[SEMANTIC ERROR - additionOpNode] CANNOT ASSIGN AN EXPRESSION TO A DIFFERENT TYPE, line: %d\n",
                additionOpNode->getLine());
        return;
    }

    if (additionOpNode->getExp1()->getType() != INT && additionOpNode->getExp1()->getType() != FLOAT) {
        fprintf(stderr,
                "[SEMANTIC ERROR - additionOpNode] CANNOT OPERATE WITH NON NUMERIC VALUES, line: %d\n",
                additionOpNode->getLine());
        return;
    }

    additionOpNode->setType(additionOpNode->getExp1()->getType());
    additionOpNode->setTypeLexeme(additionOpNode->getExp1()->getTypeLexeme());
    additionOpNode->setPointer(additionOpNode->getExp1()->isPointer());
    additionOpNode->setArraySize(additionOpNode->getExp1()->getArraySize());
    additionOpNode->setLValue(false);
}

/*
 * Array Node Size verification
 */


void SemanticTypes::visit(ArrayNode *arrayNode) {

    if (!arrayNode->getNumInt()) {
        fprintf(stderr, "[SEMANTIC ERROR - arrayNode] INVALID ARRAY SIZE, line: %d\n", arrayNode->getLine());
        return;
    }

}

/*
 * Not Expression verification
 */

void SemanticTypes::visit(NotNode *notNode) {

    notNode->setLValue(false);
    notNode->setArraySize(-1);
    notNode->setPointer(false);
    notNode->setType(BOOL);

    if (notNode->getExp()) {
        notNode->getExp()->accept(this);
    } else {
        fprintf(stderr, "[SEMANTIC ERROR - signNode] INVALID EXPRESSION, line %d\n", notNode->getLine());
        return;
    }

    bool isArray = notNode->getExp()->getArraySize() > -1;
    if (notNode->getExp()->getType() != BOOL || isArray || notNode->getExp()->isPointer()) {
        fprintf(stderr, "[SEMANTIC ERROR - signNode] CANNOT NEGATIVE A NON BOOLEAN VALUE, line %d\n",
                notNode->getLine());
        return;
    }

}

/*
 * Boolean verification
 */

void SemanticTypes::visit(BooleanOPNode *booleanOpNode) {

    booleanOpNode->setType(BOOL);
    booleanOpNode->setLValue(false);
    booleanOpNode->setPointer(false);
    booleanOpNode->setArraySize(-1);

    if (booleanOpNode->getExp1()) {
        booleanOpNode->getExp1()->accept(this);
    } else {
        fprintf(stderr, "[SEMANTIC ERROR - booleanOpNode] INVALID EXPRESSION 1, line %d\n", booleanOpNode->getLine());
        return;
    }

    if (booleanOpNode->getExp2()) {
        booleanOpNode->getExp2()->accept(this);
    } else {
        fprintf(stderr, "[SEMANTIC ERROR - booleanOpNode] INVALID EXPRESSION 2, line %d\n", booleanOpNode->getLine());
        return;
    }

    if (booleanOpNode->getOp()) {
        booleanOpNode->getOp()->accept(this);
    } else {
        fprintf(stderr, "[SEMANTIC ERROR - booleanOpNode] INVALID OPERATOR, line %d\n", booleanOpNode->getLine());
        return;
    }

    if (!_match_exp_types(booleanOpNode->getExp1(), booleanOpNode->getExp2())) {
        fprintf(stderr, "[SEMANTIC ERROR - booleanOpNode] CANNOT COMPARE DIFFERENT TYPES, line %d\n",
                booleanOpNode->getLine());
        return;
    }

    bool isArray = booleanOpNode->getExp1()->getArraySize() > -1;
    if (booleanOpNode->getOp()->getToken() != NE && booleanOpNode->getOp()->getToken() != EQ)
        if ((booleanOpNode->getExp1()->getType() != INT && booleanOpNode->getExp1()->getType() != FLOAT) || isArray ||
            booleanOpNode->getExp1()->isPointer()) {
            fprintf(stderr, "[SEMANTIC ERROR - booleanOpNode] CANNOT COMPARE WITH NON NUMERIC TYPES, line %d\n",
                    booleanOpNode->getLine());
            return;
        }

}

/*
 * Sign type verification
 */

void SemanticTypes::visit(SignNode *signNode) {
    if (signNode->getExp()) {
        signNode->getExp()->accept(this);
    } else {
        fprintf(stderr, "[SEMANTIC ERROR - signNode] INVALID EXPRESSION, line %d\n", signNode->getLine());
        return;
    }

    bool isArray = signNode->getExp()->getArraySize() > -1;
    if ((signNode->getExp()->getType() != INT && signNode->getExp()->getType() != FLOAT) || isArray ||
        signNode->getExp()->isPointer()) {
        fprintf(stderr, "[SEMANTIC ERROR - signNode] CANNOT NEGATIVE A NON NUMERIC VALUE, line %d\n",
                signNode->getLine());
        return;
    }

    signNode->setType(signNode->getExp()->getType());
    signNode->setTypeLexeme(signNode->getExp()->getTypeLexeme());
    signNode->setPointer(false);
    signNode->setArraySize(-1);
    signNode->setLValue(false);
}

/*
 * Funtion Call Type verification
 */

void SemanticTypes::visit(CallNode *callNode) {

    if (callNode->getId()) {
        callNode->getId()->accept(this);
    } else {
        fprintf(stderr, "[SEMANTIC ERROR - callNode] INVALID FUNCTION CALL, line %d\n", callNode->getLine());
        return;
    }

    if (callNode->getParameters()) {
        callNode->getParameters()->accept(this);
    }

    FunctionSymbol *func = functionTable->cSearch(callNode->getId()->getLexeme());


    if (!func) {
        fprintf(stderr, "[SEMANTIC ERROR - callNode] CALL TO UNDEFINED FUNCTION, line: %d lexeme: %s\n",
                callNode->getLine(), callNode->getId()->getLexeme());
        return;
    }

    FormalListNode *expectedParameter = func->getVarDecl();
    ExpListNode *parameter = callNode->getParameters();

    while (expectedParameter && parameter) {

        parameter->accept(this);

        bool isSameType = expectedParameter->getParameterType() == parameter->getExp()->getType();
        bool isSameArray = (expectedParameter->getArray() != NULL) == (parameter->getExp()->getArraySize() > 0);
        bool isSamePointer = (expectedParameter->getPointer() != NULL) == parameter->getExp()->isPointer();
        bool isSameTypeLexeme = true;

        if (expectedParameter->getParameterType() == ID)
            isSameTypeLexeme = !strcmp(expectedParameter->getParameterTypeLexeme(),
                                       parameter->getExp()->getTypeLexeme());

        if (!(isSameArray && isSamePointer && isSameType && isSameTypeLexeme)) {
            fprintf(stderr, "[SEMANTIC ERROR - callNode] PARAMETER TYPE MISMATCH, line: %d parameter: %s\n",
                    parameter->getLine(), expectedParameter->getId()->getLexeme());
            return;
        }

        parameter = parameter->getNext();
        expectedParameter = expectedParameter->getNext();
    }

    if (expectedParameter || parameter) {
        fprintf(stderr, "[SEMANTIC ERROR - callNode] PARAMETER COUNT MISMATCH, line: %d lexeme: %s\n",
                parameter->getLine(), callNode->getId()->getLexeme());
        return;
    }

    callNode->setLexeme(callNode->getId()->getLexeme());
    callNode->setType(func->getReturnType()->getType());
    callNode->setTypeLexeme(func->getReturnType()->getTypeLexeme());
    callNode->setPointer(func->isPointer());
    callNode->setArraySize(-1);
    callNode->setLValue(false);
}

/*
 * Primary type attribution
 */

void SemanticTypes::visit(PrimaryNode *primaryNode) {

    ExpNode *expNode = primaryNode->getExp();

    if (expNode) {
        expNode->accept(this);

        primaryNode->setType(expNode->getType());
        primaryNode->setTypeLexeme(expNode->getLexeme());
        primaryNode->setPointer(expNode->isPointer());
        primaryNode->setArraySize(expNode->getArraySize());
        primaryNode->setLValue(expNode->isLValue());
    }


    TokenNode *tokenNode = primaryNode->getTokenNode();
    if (tokenNode) {
        tokenNode->accept(this);

        VarSymbol *varSymbol = varTable->cSearch(tokenNode->getLexeme());

        //if token is a variable, check if it exists
        if ((tokenNode->getType() == ID || tokenNode->getType() == UNASSIGNED_TOKEN)) {
            if (varSymbol) {

                primaryNode->setType(varSymbol->getType()->getType());
                primaryNode->setTypeLexeme(varSymbol->getType()->getTypeLexeme());
                primaryNode->setPointer(varSymbol->isPointer());
                primaryNode->setArraySize(varSymbol->getArraySize());
                primaryNode->setLValue(true);
            } else {
                fprintf(stderr, "[SEMANTIC ERROR - primaryNode] UNDEFINED SYMBOL, line %d lexeme: %s\n",
                        primaryNode->getLine(), tokenNode->getLexeme());
            }

            return;
        }

        primaryNode->setType(tokenNode->getType());
        primaryNode->setTypeLexeme(tokenNode->getTypeLexeme());
        primaryNode->setPointer(tokenNode->isPointer());
        primaryNode->setArraySize(tokenNode->getArraySize());
        primaryNode->setLValue(tokenNode->isLValue());
    }


}

/*
 * Token type attribution
 */

void SemanticTypes::visit(TokenNode *tokenNode) {
    VarSymbol *var = varTable->cSearch(tokenNode->getLexeme());
    FunctionSymbol *func = functionTable->cSearch(tokenNode->getLexeme());

    if (var) {
        tokenNode->setType(var->getType()->getType());
        tokenNode->setTypeLexeme(var->getType()->getTypeLexeme());
        tokenNode->setArraySize(var->getArraySize());
        tokenNode->setPointer(var->isPointer());
        tokenNode->setLValue(true);
    } else if (func) {

        tokenNode->setLValue(false);
        tokenNode->setType(func->getReturnType()->getType());
        tokenNode->setTypeLexeme(func->getReturnType()->getTypeLexeme());
        tokenNode->setPointer(func->isPointer());
        tokenNode->setArraySize(-1);
    }

}

void SemanticTypes::visit(ExpListNode *expListNode) {
    if (expListNode->getExp()) {
        expListNode->getExp()->accept(this);
    }
    if (expListNode->getNext()) {
        expListNode->getNext()->accept(this);
    }
}