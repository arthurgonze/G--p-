#include "semantic.h"

VarTable *varTable;
FunctionTable *functionTable;
StructTable *structTable;
FunctionSymbol *activeFunction = NULL;

int breakEnabled = BOOL_FALSE;

void startSemantic(ProgramNode *ast)
{
    varTable = new VarTable();
    functionTable = new FunctionTable();
    structTable = new StructTable();
    Semantic semanticVisitor;
    semanticVisitor.visit(ast);
    fprintf(stderr, "\n");

}
void endSemantic()
{
    activeFunction = NULL;
    varTable->print();
    functionTable->print();
    structTable->print();
    delete varTable;
    delete functionTable;
    delete structTable;
}

void beginScope(const char *scopeLex)
{
    varTable->beginScope(scopeLex);
    functionTable->beginScope(scopeLex);
    structTable->beginScope(scopeLex);
}
void endScope()
{
    varTable->endScope();
    functionTable->endScope();
    structTable->endScope();
}

void Semantic::visit(ProgramNode *programNode)
{
    if (programNode->getTypeList())
    {
        programNode->getTypeList()->accept(this);
    }
    if (programNode->getVarList())
    {
        programNode->getVarList()->accept(this);
    }
    if (programNode->getFunctions())
    {
        programNode->getFunctions()->accept(this);
    }
}

void Semantic::visit(VarDeclNode *varDeclNode)
{
    if (varDeclNode->getIdList()!=NULL)
    {
        varDeclNode->getIdList()->accept(this);
    }
    if (varDeclNode->getNext()!=NULL)
    {
        varDeclNode->getNext()->accept(this);
    }

    IdListNode *idListAux = varDeclNode->getIdList();
    while (idListAux!=NULL)
    {
        idListAux->getId()->setType(varDeclNode->getType()->getId()->getToken());
        idListAux->getId()->setTypeLexeme(varDeclNode->getType()->getId()->getLexeme());
        idListAux = idListAux->getNext();
    }

    if (varDeclNode->getType()!=NULL && varDeclNode->getIdList()!=NULL)
    {
        if (varDeclNode->getType()->getId()->getToken()==ID)
        {
            if (!structTable->cSearch(varDeclNode->getType()->getId()->getLexeme()))
            {
                fprintf(stderr, "[SEMANTIC ERROR - VarDeclNode] TYPE NOT DEFINED, line: %d, typeLexeme: %s \n",
                        varDeclNode->getLine(), varDeclNode->getType()->getLexeme());
            }
        }
    }
    if (varDeclNode->getIdList()!=NULL)
    {
        IdListNode *listAux = varDeclNode->getIdList();
        int totalSizeAux = 0;
        while (listAux!=NULL)
        {
            VarSymbol *var;
            if (activeFunction!=NULL)
            {
                const char *aux1 = listAux->getId()->getLexeme();
                const char *aux2 = activeFunction->getLexeme();
                var = varTable->searchInScope(aux1, aux2);
            }
            else
            {
                const char *aux = listAux->getId()->getLexeme();
                var = varTable->cSearch(aux);
            }
            int sizeAux = BYTE_SIZE;
            if (listAux->getArray()!=NULL)
            {
                sizeAux *= atoi(listAux->getArray()->getNumInt()->getLexeme());

            }
            if (activeFunction!=NULL)
            {
                listAux->getId()->setOffset(activeFunction->getLocalSize() + totalSizeAux + sizeAux);
            }
            var->setSize(sizeAux);
            var->setOffset(listAux->getId()->getOffset());
            totalSizeAux += sizeAux;
            listAux = listAux->getNext();
        }
        if (activeFunction!=NULL)
        {
            activeFunction->incrementLocalSize(totalSizeAux);
        }
    }
}

void Semantic::visit(IdListNode *idListNode)
{
    if (idListNode->getId()!=NULL)
    {
        idListNode->getId()->accept(this);
        idListNode->getId()->setPointer(idListNode->getPointer()!=NULL);
        if (idListNode->getArray()!=NULL)
        {
            idListNode->getArray()->accept(this);
            idListNode->getId()->setArraySize(idListNode->getArray()->getArraySize());
        }
    }
    if (idListNode->getNext()!=NULL)
    {
        idListNode->getNext()->accept(this);
    }
    if (idListNode->getArray()!=NULL)
    {
        idListNode->getArray()->accept(this);
    }
    TypeNode *typeNode = new TypeNode(idListNode->getId(), idListNode->getId()->getTypeLexeme());
    typeNode->setLine(idListNode->getLine());
    if (!varTable->cInsert(typeNode, idListNode->getId()->getLexeme(), idListNode->getId()->isPointer(),
                           idListNode->getId()->getArraySize(), BOOL_FALSE))
    {
// TODO       fprintf(stderr, "[SEMANTIC ERROR - idListNode] VARIABLE ALREADY EXISTS, line: %d \n", idListNode->getLine());
    }
}

void Semantic::visit(TypeDeclNode *typeDeclNode)
{
    const char *typeLex;
    if (typeDeclNode->getId()!=NULL)
    {
        typeLex = typeDeclNode->getId()->getLexeme();
    }
    else
    {
        typeLex = NULL;
    }

    if (typeDeclNode->getNext()!=NULL)
    {
        typeDeclNode->getNext()->accept(this);
    }

    if (!structTable->cInsert(typeLex, typeDeclNode->getDecl()))
    {
        fprintf(stderr, "[SEMANTIC ERROR - typeDeclNode] STRUCT ALREADY EXISTS, line: %d, lexeme: %s \n",
                typeDeclNode->getLine(), typeDeclNode->getId()->getLexeme());
    }

    beginScope(typeLex);
    if (typeDeclNode->getDecl()!=NULL)
    {
        typeDeclNode->getDecl()->accept(this);
        VarDeclNode *varListAux = typeDeclNode->getDecl();
        int totalSizeAux = 0;
        while (varListAux!=NULL)
        {
            IdListNode *idListAux = varListAux->getIdList();
            while (idListAux!=NULL)
            {
                VarSymbol *var = varTable->searchInScope(idListAux->getId()->getLexeme(), typeDeclNode->getId()->getLexeme());
                int sizeAux = BYTE_SIZE;
                if (idListAux->getArray()!=NULL)
                {
                    sizeAux *= atoi(idListAux->getArray()->getNumInt()->getLexeme());
                }
                var->setSize(sizeAux);
                var->setOffset(totalSizeAux);
                totalSizeAux += sizeAux;
                idListAux = idListAux->getNext();
            }
            if (varListAux->getNext())
            {
                varListAux = varListAux->getNext()->getNext();
            }
            else
            {
                varListAux = NULL;
            }
        }
        StructSymbol *structSymbol = structTable->cSearch(typeDeclNode->getId()->getLexeme());
        structSymbol->setSize(totalSizeAux);
    }
    endScope();
}

void Semantic::visit(TokenNode *tokenNode)
{
    if (tokenNode->getToken()==ID)
    {
        tokenNode->setLexeme(tokenNode->getLexeme());
    }
    if (tokenNode->getToken()==INT)
    {
        tokenNode->setTypeLexeme(token_id_to_name(INT));
        tokenNode->setLexeme(tokenNode->getLexeme());
    }
    if (tokenNode->getToken()==FLOAT)
    {
        tokenNode->setTypeLexeme(token_id_to_name(FLOAT));
        tokenNode->setLexeme(tokenNode->getLexeme());
    }
    if (tokenNode->getToken()==CHAR)
    {
        tokenNode->setTypeLexeme(token_id_to_name(CHAR));
        tokenNode->setLexeme(tokenNode->getLexeme());
    }
    if (tokenNode->getToken()==LITERAL)
    {
        tokenNode->setTypeLexeme(token_id_to_name(LITERAL));
        tokenNode->setLexeme(tokenNode->getLexeme());
    }
}

void Semantic::visit(FormalListNode *formalListNode)
{
    if (formalListNode->getId()!=NULL)
    {
        formalListNode->getId()->accept(this);
    }
    if (formalListNode->getArray()!=NULL)
    {
        formalListNode->getArray()->accept(this);
    }
    if (formalListNode->getNext()!=NULL)
    {
        formalListNode->getNext()->accept(this);
    }

    const char *idLexAux;
    if (formalListNode->getId()!=NULL)
    {
        idLexAux = formalListNode->getId()->getLexeme();
    }
    else
    {
        idLexAux = NULL;
    }

    int arraySize;
    if (formalListNode->getArray()!=NULL)
    {
        arraySize = atoi(formalListNode->getArray()->getNumInt()->getLexeme());
    }
    else
    {
        arraySize = -1;
    }

    if (!varTable->cInsert(formalListNode->getType(), idLexAux, formalListNode->getPointer()!=NULL, arraySize, BOOL_TRUE))
    {
        fprintf(stderr, "[SEMANTIC ERROR - formalListNode] VARIABLE ALREADY EXISTS, line: %d, lexeme: %s \n",
                formalListNode->getLine(), formalListNode->getId()->getLexeme());
    }

    if (formalListNode->getId()!=NULL && activeFunction!=NULL)
    {
        VarSymbol *var = varTable->searchInScope(formalListNode->getId()->getLexeme(), activeFunction->getLexeme());
        int sizeAux = BYTE_SIZE;
        if (formalListNode->getArray()!=NULL)
        {
            sizeAux *= atoi(formalListNode->getArray()->getNumInt()->getLexeme());
        }
        var->setSize(sizeAux);
        var->setOffset(activeFunction->getParamSize());
        formalListNode->getId()->setOffset(var->getOffset());
        activeFunction->incrementParamSize(sizeAux);
    }
}

void Semantic::visit(ExpListNode *expListNode)
{
    if (expListNode->getExp())
    {
        expListNode->getExp()->accept(this);
    }
    if (expListNode->getNext())
    {
        expListNode->getNext()->accept(this);
    }
}

void Semantic::visit(CallNode *callNode)
{
    FunctionSymbol *funcSymbol = functionTable->cSearch(callNode->getId()->getLexeme());
    if (funcSymbol!=NULL)
    {
        if (callNode->getId()!=NULL && activeFunction)
        {
            callNode->getId()->accept(this);
            FunctionSymbol *func = functionTable->cSearch(callNode->getId()->getLexeme());
            /// calcular maior chamada de função
            if (func->getParamSize() > activeFunction->getCallSize())
            {
                activeFunction->setCallSize(func->getParamSize());
            }
        }
        //Verifica, na sequência, se os parâmetros passados e os esperados são compatíveis
        if (callNode->getParameters()!=NULL)
        {
            callNode->getParameters()->accept(this);
            ExpListNode *exp = callNode->getParameters();
            FormalListNode *param = funcSymbol->getVarDecl();

            while (exp!=NULL && param!=NULL)
            {
                if (exp->getExp()->getType()!=param->getType()->getId()->getToken() /*TODO||
                    exp->getExp()->getTypeLexeme()!=param->getType()->getId()->getLexeme()*/)
                {
                    PrimaryNode *node = (PrimaryNode *) exp->getExp();
                    if (node->getTokenNode()->getToken()==ID)
                    {
                        VarSymbol *symbol = varTable->cSearch(node->getTokenNode()->getLexeme());

                       if(symbol->getType()->getId()->getType() != param->getType()->getId()->getToken())
                       {
                            fprintf(stderr, "[SEMANTIC ERROR - callNode] ID %s DOES NOT MATCH WITH PARAMETER TYPE, GOT %s EXPECTED %s line: %d, lexeme: %s \n",
                                    node->getTokenNode()->getLexeme(),
                                    token_id_to_name(symbol->getType()->getId()->getType()), token_id_to_name(param->getType()->getId()->getToken()),
                                    callNode->getLine(), callNode->getId()->getLexeme());
                        }
                    }
                    else
                    {
                        fprintf(stderr, "[SEMANTIC ERROR - callNode] PARAMETER TYPE MISMATCH, line: %d, lexeme: %s \n",
                                callNode->getLine(), callNode->getId()->getLexeme());
                    }

                    exp = exp->getNext();
                    param = param->getNext();
                    break;
                }
                exp = exp->getNext();
                param = param->getNext();
            }
            if (exp==NULL && param!=NULL)
            {
                fprintf(stderr, "[SEMANTIC ERROR - callNode] TOO FEW ARGUMENTS, line: %d, lexeme: %s \n", callNode->getLine(), callNode->getId()->getLexeme());
            }
            else if (exp!=NULL && param==NULL)
            {
                fprintf(stderr, "[SEMANTIC ERROR - callNode] TOO MANY ARGUMENTS, line: %d, lexeme: %s \n", callNode->getLine(), callNode->getId()->getLexeme());
            }
        }
        if (callNode->getParameters()!=NULL && callNode->getParameters()->getNext()!=NULL && callNode->getParameters()->getNext()->getExp()!=NULL)
        {
            callNode->getParameters()->getNext()->getExp()->accept(this);
            callNode->setType(callNode->getParameters()->getNext()->getExp()->getType());
            callNode->setTypeLexeme(callNode->getParameters()->getNext()->getExp()->getTypeLexeme());
        }
        else
        {
            callNode->setType(funcSymbol->getReturnType()->getId()->getToken());
            callNode->setTypeLexeme(funcSymbol->getReturnType()->getId()->getLexeme());
        }
    }
    else
    {
        fprintf(stderr, "[SEMANTIC ERROR - callNode] FUNCTION NOT DECLARED, line: %d, lexeme: %s \n", callNode->getLine(), callNode->getId()->getLexeme());
    }
}

void Semantic::visit(PrimaryNode *primaryNode)
{
    if (primaryNode->getExp()!=NULL)
    {
        primaryNode->getExp()->accept(this);
        if (TokenNode *id = (TokenNode *) (primaryNode->getExp()))
        {
            primaryNode->setLValue(BOOL_TRUE);
            if (primaryNode->getExp()!=NULL && typeid(*primaryNode->getExp())==typeid(CallNode))
            {
                primaryNode->getExp()->accept(this);
                primaryNode->setType(primaryNode->getExp()->getType());
                primaryNode->setTypeLexeme(primaryNode->getExp()->getTypeLexeme());
            }
            else
            {
                VarSymbol *varSymbol = varTable->cSearch(id->getLexeme());

                if (varSymbol!=NULL)
                {
                    id->setType(varSymbol->getType()->getId()->getToken());
                    id->setTypeLexeme(varSymbol->getType()->getId()->getLexeme());

                    if (varSymbol->getType()->getId()->getToken()!=INT && varSymbol->getType()->getId()->getToken()!=FLOAT
                        && varSymbol->getType()->getId()->getToken()!=CHAR && varSymbol->getType()->getId()->getToken()!=LITERAL
                        && varSymbol->getType()->getId()->getToken()!=BOOL)
                    {
                        fprintf(stderr, "[SEMANTIC ERROR - primaryNode] TYPE NOT DEFINED, line: %d, lexeme: %s \n", primaryNode->getLine(), primaryNode->getLexeme());
                    }
                    else
                    {
                        primaryNode->setType(primaryNode->getExp()->getType());
                        primaryNode->setTypeLexeme(primaryNode->getExp()->getTypeLexeme());
                    }
                }
                else
                {
                    fprintf(stderr, "[SEMANTIC ERROR - primaryNode] VARIABLE NOT DECLARED, line: %d, lexeme: %s \n", primaryNode->getLine(), primaryNode->getLexeme());
                }

            }
        }
        else if ((typeid(primaryNode)==typeid(CallNode)) && (((CallNode *) primaryNode)->getId()->getToken()==NUMINT || ((CallNode *) primaryNode)->getId()->getToken()==NUMFLOAT ||
            ((CallNode *) primaryNode)->getId()->getToken()==LITERALCHAR || ((CallNode *) primaryNode)->getId()->getToken()==LITERAL ||
            ((CallNode *) primaryNode)->getId()->getToken()==TRUE || ((CallNode *) primaryNode)->getId()->getToken()==FALSE))
        {
            if (primaryNode->getExp()!=NULL)
            {
                fprintf(stderr, "[SEMANTIC ERROR - primaryNode] UNEXPECTED ACCESS IN NOT ID TYPE, line: %d, lexeme: %s \n", primaryNode->getLine(), primaryNode->getLexeme());
            }
            primaryNode->setType(primaryNode->getExp()->getType());
            primaryNode->setTypeLexeme(primaryNode->getExp()->getTypeLexeme());
        }
        else
        {
            if (primaryNode->getExp()!=NULL)
            {
                primaryNode->getExp()->accept(this);
                primaryNode->setType(primaryNode->getExp()->getType());
                primaryNode->setTypeLexeme(primaryNode->getExp()->getTypeLexeme());
            }
            else
            {
                primaryNode->setType(primaryNode->getExp()->getType());
                primaryNode->setTypeLexeme(primaryNode->getExp()->getTypeLexeme());
            }
        }

        primaryNode->setPointer(primaryNode->getExp()->isPointer());
        primaryNode->setArraySize(primaryNode->getExp()->getArraySize());
        ((CallNode *) primaryNode)->setLexeme(((CallNode *) primaryNode)->getLexeme());
    }
}

void Semantic::visit(FunctionNode *functionNode)
{
    if (functionNode->getId()!=NULL)
    {
        functionNode->getId()->accept(this);
    }
    if (functionNode->getParameters()!=NULL)
    {
        functionNode->getParameters()->accept(this);
    }
    if (functionNode->getLocal()!=NULL)
    {
        functionNode->getLocal()->accept(this);
    }
    if (functionNode->getBody()!=NULL)
    {
        functionNode->getBody()->accept(this);
    }

    const char *functionLexeme;
    if (functionNode->getId()!=NULL)
    {
        functionLexeme = functionNode->getId()->getLexeme();
    }
    else
    {
        functionLexeme = NULL;
    }

    if (!functionTable->cInsert(functionNode->getType(), functionLexeme, functionNode->getParameters(), functionNode->getPointer()!=NULL))
    {
        fprintf(stderr, "[SEMANTIC ERROR - functionNode] FUNCTION ALREADY EXISTS, line: %d, lexeme: %s \n", functionNode->getLine(), functionNode->getId()->getLexeme());
    }

    ///*** Check if type was declared
    if (functionNode->getType()!=NULL && functionNode->getId()!=NULL)
    {
        if (functionNode->getType()->getId()->getToken()==ID)
        {
            const char *aux = functionNode->getType()->getId()->getLexeme();
            if (strcmp(aux, "void")!=0)
            {
                if (!structTable->cSearch(functionNode->getType()->getId()->getLexeme()))
                {
                    fprintf(stderr, "[SEMANTIC ERROR - functionNode] RETURN TYPE NOT DEFINED, line: %d, lexeme: %s \n", functionNode->getLine(), functionNode->getId()->getLexeme());
                }
            }
        }
    }

    activeFunction = functionTable->cSearch(functionLexeme);

    beginScope(functionLexeme);

    VarDeclNode *varStmtAux = functionNode->getLocal();
    while (varStmtAux!=NULL && varStmtAux->getNext()!=NULL)
    {
        varStmtAux = varStmtAux->getNext();
    }
    if (varStmtAux!=NULL && varStmtAux->getIdList()!=NULL)
    {
        StmtListNode *stmtListAux = functionNode->getBody();
        while (stmtListAux!=NULL && stmtListAux->getStmt()!=NULL)
        {
            if (typeid(*stmtListAux->getStmt())==typeid(ReturnNode))
            {
                stmtListAux->getStmt()->accept(this);
                ReturnNode *returnStmt = (ReturnNode *) (stmtListAux->getStmt());
                if (functionNode->getType()->getId()->getToken()!=returnStmt->getExp()->getType())
                {
                    fprintf(stderr, "[SEMANTIC ERROR - functionNode] INCOMPATIBLE RETURN TYPE, line: %d, lexeme: %s \n", functionNode->getLine(), functionNode->getId()->getLexeme());
                }
                stmtListAux = stmtListAux->getNext();
            }
            else
            {
                if (stmtListAux->getStmt()==NULL)
                {
                    fprintf(stderr, "[SEMANTIC ERROR - functionNode] MISSING RETURN STATEMENT, line: %d, lexeme: %s \n", functionNode->getLine(), functionNode->getId()->getLexeme());
                    break;
                }
                else
                {
                    stmtListAux = stmtListAux->getNext();
                }
            }
        }
    }

    endScope();
    activeFunction = NULL;
}

void Semantic::visit(StmtNode *stmtNode)
{
    if (stmtNode->getStmt())
    {
        stmtNode->getStmt()->accept(this);
        if (typeid(*stmtNode->getStmt())==typeid(ReturnNode))
        {
            stmtNode->setReturn(true);
        }
        if (typeid(*stmtNode->getStmt())==typeid(IfNode))
        {
            IfNode *aux = (IfNode *) stmtNode->getStmt();
            if (aux->getTrueStmt() && aux->getFalseStmt())
            {
                stmtNode->setReturn(aux->getTrueStmt()->isReturn() && aux->getFalseStmt()->isReturn());
            }
        }
        if (typeid(*stmtNode->getStmt())==typeid(StmtListNode))
        {
            StmtListNode *aux = (StmtListNode *) stmtNode->getStmt();
            stmtNode->setReturn(aux->isReturn());
        }
    }
}

void Semantic::visit(StmtListNode *stmtListNode)
{
    if (stmtListNode->getStmt())
    {
        stmtListNode->getStmt()->accept(this);
    }
    if (stmtListNode->getNext())
    {
        stmtListNode->getNext()->accept(this);
    }
}

void Semantic::visit(ReadLnNode *read)
{
    if (read->getExp())
    {
        read->getExp()->accept(this);
        if (!read->getExp()->isLValue())
        {
            fprintf(stderr, "[SEMANTIC ERROR - read] LVALUE EXPECTED, line: %d, lexeme: %s \n", read->getExp()->getLine(), read->getExp()->getLexeme());
        }
    }
}

void Semantic::visit(PrintNode *print)
{
    if (print->getExpList())
    {
        print->getExpList()->accept(this);
    }
}

void Semantic::visit(WhileNode *whileNode)
{
    if (whileNode->getHead()!=NULL)
    {
        whileNode->getHead()->accept(this);
        BooleanOPNode *booleanOPNode = (BooleanOPNode*)whileNode->getHead();
        if (!booleanOPNode->getOp())
        {
            fprintf(stderr, "[SEMANTIC ERROR - whileNode] BOOLEAN EXPRESSION REQUIRED, line: %d, Conditionlexeme: %s \n", whileNode->getLine(), whileNode->getHead()->getLexeme());
        }
    }
    breakEnabled = BOOL_TRUE;
    if (whileNode->getBody()!=NULL)
    {
        whileNode->getBody()->accept(this);
    }
    breakEnabled = BOOL_FALSE;
}

void Semantic::visit(ReturnNode *returnNode)
{
    if (returnNode->getExp()!=NULL)
    {
        returnNode->getExp()->accept(this);
    }
}

void Semantic::visit(TryNode *tryNode)
{
    if (tryNode->getTry())
    {
        tryNode->getTry()->accept(this);
    }
    if (tryNode->getCatch())
    {
        tryNode->getCatch()->accept(this);
    }
}

void Semantic::visit(BreakNode *breakNode)
{
    if (breakEnabled!=BOOL_TRUE)
    {
        fprintf(stderr, "[SEMANTIC ERROR - breakNode] UNEXPECTED BREAK, line: %d \n", breakNode->getLine());
    }
}

void Semantic::visit(IfNode *ifNode)
{
    if (ifNode->getHead()!=NULL)
    {
        ifNode->getHead()->accept(this);
        if (ifNode->getHead()->getType()!=BOOL)
        {
            fprintf(stderr, "[SEMANTIC ERROR - ifNode] BOOLEAN EXPRESSION REQUIRED, line: %d, conditionLexeme: %s \n", ifNode->getLine(), ifNode->getHead()->getLexeme());
        }
    }
    if (ifNode->getTrueStmt()!=NULL)
    {
        ifNode->getTrueStmt()->accept(this);
    }
    if (ifNode->getFalseStmt()!=NULL)
    {
        ifNode->getFalseStmt()->accept(this);
    }
}

void Semantic::visit(SwitchNode *switchNode)
{
    if (switchNode->getExp()!=NULL)
    {
        switchNode->getExp()->accept(this);
        if (switchNode->getExp()->getType()!=INT)
        {
            fprintf(stderr, "[SEMANTIC ERROR - switchNode] INT VALUE REQUIRED, line: %d, lexeme: %s \n", switchNode->getLine(), switchNode->getExp()->getLexeme());
        }
    }
    if (switchNode->getBlock()!=NULL)
    {
        breakEnabled = BOOL_TRUE;
        switchNode->getBlock()->accept(this);
        breakEnabled = BOOL_FALSE;
    }
    else
    {
        fprintf(stderr, "[SEMANTIC ERROR - switchNode] CASE STATEMENT REQUIRED, line: %d, lexeme: %s \n", switchNode->getLine(), switchNode->getExp()->getLexeme());
    }
}

void Semantic::visit(AdditionOPNode *additionOPNode)
{
    if (additionOPNode->getExp1()!=NULL)
    {
        additionOPNode->getExp1()->accept(this);
    }
    if (additionOPNode->getExp2()!=NULL)
    {
        additionOPNode->getExp2()->accept(this);
    }

    if ((additionOPNode->getExp1()->getType()!=INT && additionOPNode->getExp1()->getType()!=FLOAT) &&
        (additionOPNode->getExp2()->getType()==INT || additionOPNode->getExp2()->getType()==FLOAT))
    {
        fprintf(stderr, "[SEMANTIC ERROR - additionOPNode] ARITHMETIC OP TYPE, line: %d, lexeme: %s \n", additionOPNode->getLine(), additionOPNode->getLexeme());
    }
    else
    {
        if (additionOPNode->getExp1()->getType()==additionOPNode->getExp2()->getType())
        {
            additionOPNode->setType(additionOPNode->getExp1()->getType());
        }
        else
        {
            additionOPNode->setType(FLOAT);
        }
        additionOPNode->setTypeLexeme(token_id_to_name(additionOPNode->getType()));
    }

}

void Semantic::visit(AddressValNode *addressValNode)
{
    if (addressValNode->getExp()!=NULL)
    {
        addressValNode->getExp()->accept(this);
    }
}

void Semantic::visit(AssignNode *assignNode)
{
    if (assignNode->getExp1()!=NULL)
    {
        assignNode->getExp1()->accept(this);
    }
    if (assignNode->getExp2()!=NULL)
    {
        assignNode->getExp2()->accept(this);
    }

    if (assignNode->getExp1()!=NULL && !assignNode->getExp1()->isLValue())
    {
        fprintf(stderr, "[SEMANTIC ERROR - assignNode] LVALUE EXPECTED, line: %d, lexeme Exp1: %s\n", assignNode->getLine(), assignNode->getExp1()->getLexeme());
    }

    if (assignNode->getExp1()!=NULL && assignNode->getExp2()!=NULL && assignNode->getExp1()->getType()!=assignNode->getExp2()->getType())
    {
        fprintf(stderr, "[SEMANTIC ERROR - assignNode] CANNOT ASSIGN TO DIFFERENT TYPE, line: %d, lexeme Exp1: %s, lexeme Exp2: %s \n", assignNode->getLine(), assignNode->getExp1()->getLexeme(), assignNode->getExp2()->getLexeme());
    }
}

void Semantic::visit(BooleanOPNode *booleanOPNode)
{
    if (booleanOPNode->getExp1()!=NULL)
    {
        booleanOPNode->getExp1()->accept(this);
    }
    if (booleanOPNode->getExp2()!=NULL)
    {
        booleanOPNode->getExp2()->accept(this);
    }
    if (booleanOPNode->getExp1()->getType()!=BOOL || booleanOPNode->getExp2()->getType()!=BOOL)
    {
        // TODO fprintf(stderr, "[SEMANTIC ERROR - booleanOPNode] LOGICAL OP TYPE, line: %d, OPLexeme: %s \n", booleanOPNode->getLine(), token_id_to_name(booleanOPNode->getOp()->getToken()));
    }
    booleanOPNode->setType(BOOL);
    booleanOPNode->setTypeLexeme(token_id_to_name(BOOL));
}

void Semantic::visit(NameExpNode *nameExpNode)
{
    nameExpNode->setLValue(true);
    if (nameExpNode->getExp())
    {
        nameExpNode->getExp()->accept(this);
        if (nameExpNode->getExp()->getArraySize())
        {
            fprintf(stderr, "[SEMANTIC ERROR - nameExpNode] UNEXPECTED ARRAY, line: %d, lexeme: %s \n", nameExpNode->getLine(), nameExpNode->getId()->getLexeme());
        }

        if (nameExpNode->getExp()->isPointer())
        {
            fprintf(stderr, "[SEMANTIC ERROR - nameExpNode] UNEXPECTED POINTER, line: %d, lexeme: %s \n", nameExpNode->getExp()->getLine(), nameExpNode->getExp()->getLexeme());// nameExpNode->exp->line
        }

        if (nameExpNode->getExp()->getTypeLexeme())
        {
            ///Verifica acesso a campos de uma estrutura
            StructSymbol *decl = structTable->cSearch(nameExpNode->getExp()->getTypeLexeme());
            if (decl)
            {
                VarSymbol *campo = varTable->cSearch(nameExpNode->getId()->getLexeme());
                if (campo)
                {
                    nameExpNode->setPointer(campo->isPointer());
                    nameExpNode->setArraySize(campo->getArraySize());
                    if (campo->getType()->getId()->getToken()==ID)
                    {
                        nameExpNode->setTypeLexeme(campo->getType()->getId()->getLexeme());
                        nameExpNode->setType(TYPEDEF);
                    }
                    else if (campo->getType()->getId()->getToken()==INT)
                    {
                        nameExpNode->setTypeLexeme(NULL);
                        nameExpNode->setType(NUMINT);
                    }
                    else if (campo->getType()->getId()->getToken()==FLOAT)
                    {
                        nameExpNode->setTypeLexeme(NULL);
                        nameExpNode->setType(NUMFLOAT);
                    }
                    else if (campo->getType()->getId()->getToken()==CHAR)
                    {
                        nameExpNode->setTypeLexeme(NULL);
                        nameExpNode->setType(LITERALCHAR);
                    }
                    else if (campo->getType()->getId()->getToken()==BOOL)
                    {
                        nameExpNode->setTypeLexeme(NULL);
                        nameExpNode->setType(BOOL);
                    }
                }
                else
                {
                    fprintf(stderr, "[SEMANTIC ERROR - nameExpNode] INVALID FIELD, line: %d, TypeLexeme: %s \n", nameExpNode->getExp()->getLine(), token_id_to_name(nameExpNode->getExp()->getType()));//nameExpNode->exp->line
                }
            }
            else
            {
                fprintf(stderr, "[SEMANTIC ERROR - nameExpNode] UNKOWN TYPE, line: %d, TypeLexeme: %s \n", nameExpNode->getExp()->getLine(), token_id_to_name(nameExpNode->getExp()->getType()));//nameExpNode->exp->line
            }
        }
        else
        {
            //fprintf(stderr, "[SEMANTIC ERROR - nameExpNode] STRUCT EXPECTED, line: %d, TypeLexeme: %s \n", nameExpNode->getExp()->getLine(), token_id_to_name(nameExpNode->getExp()->getType()));//nameExpNode->exp->line
        }
    }
}

void Semantic::visit(NotNode *notNode)
{
    if (notNode->getExp()!=NULL)
    {
        notNode->getExp()->accept(this);
    }
    if (notNode->getExp()->getType()!=BOOL)
    {
        fprintf(stderr, "[SEMANTIC ERROR - notNode] LOGICAL OP TYPE, line: %d, lexeme: %s \n", notNode->getLine(), notNode->getLexeme());
    }
    notNode->setType(BOOL);
    notNode->setTypeLexeme(token_id_to_name(BOOL));
    if (token_id_to_name(TRUE)==notNode->getLexeme())
    {
        notNode->setLexeme(token_id_to_name(FALSE));
    }
    else
    {
        notNode->setLexeme(token_id_to_name(TRUE));
    }
}

void Semantic::visit(PointerValNode *pointerValNode)
{
    pointerValNode->setLValue(BOOL_TRUE);
    if (pointerValNode->getExp()!=NULL)
    {
        pointerValNode->getExp()->accept(this);
    }
}

void Semantic::visit(MultiplicationOPNode *multiplicationOPNode)
{
    if (multiplicationOPNode->getExp1()!=NULL)
    {
        multiplicationOPNode->getExp1()->accept(this);
    }
    if (multiplicationOPNode->getExp2()!=NULL)
    {
        multiplicationOPNode->getExp2()->accept(this);
    }

    if ((multiplicationOPNode->getExp1()->getType()!=INT && multiplicationOPNode->getExp1()->getType()!=FLOAT) &&
        (multiplicationOPNode->getExp2()->getType()==INT || multiplicationOPNode->getExp2()->getType()==FLOAT))
    {
        fprintf(stderr, "[SEMANTIC ERROR - multiplicationOPNode] ARITHMETIC OP TYPE, line: %d, Exp1Lexeme: %s \n", multiplicationOPNode->getLine(), multiplicationOPNode->getExp1()->getLexeme());
    }
    else
    {
        if (multiplicationOPNode->getExp1()->getType()==multiplicationOPNode->getExp2()->getType())
        {
            multiplicationOPNode->setType(multiplicationOPNode->getExp1()->getType());
        }
        else
        {
            multiplicationOPNode->setType(FLOAT);
        }
        multiplicationOPNode->setTypeLexeme(token_id_to_name(multiplicationOPNode->getType()));
    }
}

void Semantic::visit(SignNode *signNode)
{
    if (signNode->getExp()!=NULL)
    {
        signNode->getExp()->accept(this);
        signNode->setType(signNode->getExp()->getType());
        signNode->setTypeLexeme(signNode->getExp()->getTypeLexeme());
    }
}

void Semantic::visit(ArrayCallNode *arrayCallNode)
{
    //TODO: RECEBER LEXEMA PARA PODER verificar se valor do index está fora do limite
    arrayCallNode->setLValue(BOOL_TRUE);
    if (arrayCallNode->getIndex()!=NULL)
    {
        arrayCallNode->getIndex()->accept(this);
    }
    if (arrayCallNode->getExp()!=NULL)
    {
        arrayCallNode->getExp()->accept(this);
    }

    PrimaryNode *aux = (PrimaryNode *) arrayCallNode->getIndex();

    if (aux->getTokenNode()->getToken()==ID)
    {
        VarSymbol *varSymbol = varTable->cSearch(aux->getTokenNode()->getLexeme());
        if (varSymbol->getType()->getId()->getType()!=INT)
        {
            fprintf(stderr, "[SEMANTIC ERROR - ArrayCallNode] ARRAY INDEX TYPE MISMATCH, INT EXPECTED, line: %d, Index Type: %s \n", arrayCallNode->getLine(), token_id_to_name(varSymbol->getType()->getId()->getType()));
        }
    }
    else if (aux->getTokenNode()->getToken()!=NUMINT)
    {
        fprintf(stderr, "[SEMANTIC ERROR - ArrayCallNode] ARRAY INDEX TYPE MISMATCH, INT EXPECTED, line: %d, Index Type: %s \n", arrayCallNode->getLine(), token_id_to_name(aux->getTokenNode()->getToken()));
    }
}

void Semantic::visit(FunctionListNode *functionListNode)
{
    /// deixar na mesma ordem do codigo
    if (functionListNode->getNext())
    {
        functionListNode->getNext()->accept(this);
    }
    if (functionListNode->getFunction())
    {
        functionListNode->getFunction()->accept(this);
    }
}

void Semantic::visit(PointerExpNode *pointerExpNode)
{
    pointerExpNode->setLValue(true);
    if (pointerExpNode->getExp())
    {
        pointerExpNode->getExp()->accept(this);
        // TODO arrayTest(pointerExpNode->getExp());
        if (pointerExpNode->getExp()->isPointer() && pointerExpNode->getExp()->getTypeLexeme())
        {
            StructSymbol *dec = structTable->cSearch(pointerExpNode->getExp()->getTypeLexeme());
            if (dec)
            {
                VarSymbol *campo = varTable->cSearch(pointerExpNode->getId()->getLexeme());
                if (campo)
                {
                    pointerExpNode->setPointer(campo->isPointer());
                    pointerExpNode->setArraySize(campo->getArraySize());
                    if (campo->getType()->getId()->getToken()==ID)
                    {
                        pointerExpNode->setTypeLexeme(campo->getType()->getId()->getLexeme());
                        pointerExpNode->setType(TYPEDEF);
                    }
                    else if (campo->getType()->getId()->getToken()==INT)
                    {
                        pointerExpNode->setTypeLexeme(NULL);
                        pointerExpNode->setType(NUMINT);
                    }
                    else if (campo->getType()->getId()->getToken()==FLOAT)
                    {
                        pointerExpNode->setTypeLexeme(NULL);
                        pointerExpNode->setType(NUMFLOAT);
                    }
                    else if (campo->getType()->getId()->getToken()==CHAR)
                    {
                        pointerExpNode->setTypeLexeme(NULL);
                        pointerExpNode->setType(LITERALCHAR);
                    }
                    else if (campo->getType()->getId()->getToken()==BOOL)
                    {
                        pointerExpNode->setTypeLexeme(NULL);
                        pointerExpNode->setType(BOOL);
                    }
                }
                else
                {
                    fprintf(stderr, "[SEMANTIC ERROR - pointerExpNode] INVALID FIELD, line: %d, lexeme: %s \n", pointerExpNode->getExp()->getLine(), pointerExpNode->getExp()->getLexeme());//pointerExpNode->exp->line
                }
            }
            else
            {
                fprintf(stderr, "[SEMANTIC ERROR - pointerExpNode] UNKOWN TYPE, line: %d, lexeme: %s \n", pointerExpNode->getExp()->getLine(), pointerExpNode->getExp()->getLexeme());//pointerExpNode->exp->line
            }
        }
        else
        {
            if (pointerExpNode->getExp()->isPointer())
            {
                fprintf(stderr, "[SEMANTIC ERROR - pointerExpNode] POINTER EXPECTED, line: %d, lexeme: %s \n", pointerExpNode->getExp()->getLine(), pointerExpNode->getExp()->getLexeme());//pointerExpNode->exp->line
            }
            if (pointerExpNode->getExp()->getTypeLexeme())
            {
                fprintf(stderr, "[SEMANTIC ERROR - pointerExpNode] STRUCT EXPECTED, line: %d, lexeme: %s \n", pointerExpNode->getExp()->getLine(), pointerExpNode->getExp()->getLexeme());//pointerExpNode->exp->line
            }
        }

    }
}

void Semantic::visit(CaseBlockNode *caseBlockNode)
{
    if (caseBlockNode->getNum() && caseBlockNode->getNum()->getToken()!=NUMINT)
    {
        fprintf(stderr, "[SEMANTIC ERROR - caseBlockNode] CASE WITH NON-INTEGER VALUE, line: %d , lexeme: %s \n", caseBlockNode->getNum()->getLine(), caseBlockNode->getNum()->getLexeme());//caseBlockNode->num->line
    }
    if (caseBlockNode->getStmt())
    {
        caseBlockNode->getStmt()->accept(this);
    }
    if (caseBlockNode->getNext())
    {
        caseBlockNode->getNext()->accept(this);
    }
}

Semantic::Semantic() {}
void Semantic::visit(TypeNode *typeNode) {}
void Semantic::visit(ThrowNode *throwNode) {}