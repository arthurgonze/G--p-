#include "semantic.h"

#define SCOPE_NON_LOCAL "nonLocal"
#define UNASSIGNED_TOKEN -42

VarTable *varTable;
FunctionTable *functionTable;
StructTable *structTable;
FunctionSymbol *activeFunction = NULL;

int breakDepth = 0;

void startSemantic(ProgramNode *ast)
{
    varTable = new VarTable();
    functionTable = new FunctionTable();
    structTable = new StructTable();

    SemanticTables semanticVisitor;
    SemanticTypes semanticTypes;

    semanticVisitor.visit(ast);
    semanticTypes.visit(ast);
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

bool _match_exp_types(ExpNode *exp1, ExpNode *exp2) {

    bool isSameType = exp1->getType() == exp2->getType();
    bool isSamePointer = exp1->isPointer() == exp2->isPointer();
    bool isArray = (exp1->getArraySize() >= 0) == (exp2->getArraySize() >= 0);
    bool isSameTypeLexeme = true;

    if(exp1->getType() == ID)
        isSameTypeLexeme = !strcmp(exp1->getTypeLexeme(), exp2->getTypeLexeme());

    return (isSameType && isSameTypeLexeme && isSamePointer && isArray);
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
                if (exp->getExp()->getType()!=param->getType()->getId()->getToken() /*
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
    if (primaryNode->getExp()!=NULL && primaryNode->getTokenNode()!=NULL)
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

                if (varSymbol!=nullptr)
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
        BooleanOPNode *booleanOPNode = (BooleanOPNode *) whileNode->getHead();
        if (!booleanOPNode->getOp())
        {
            fprintf(stderr, "[SEMANTIC ERROR - whileNode] BOOLEAN EXPRESSION REQUIRED, line: %d, Conditionlexeme: %s \n", whileNode->getLine(), whileNode->getHead()->getLexeme());
        }
    }
    breakDepth += 1;
    if (whileNode->getBody()!=NULL)
    {
        whileNode->getBody()->accept(this);
    }
    breakDepth -= 1;
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
    if (!breakDepth)
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
        breakDepth += 1;
        switchNode->getBlock()->accept(this);
        breakDepth -= 1;
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

void SemanticTables::visit(ProgramNode *programNode) {
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

void SemanticTables::visit(TypeDeclNode *typeDeclNode) {

    if (typeDeclNode->getNext()!=NULL)
    {
        typeDeclNode->getNext()->accept(this);
    }

    if(!typeDeclNode->getId()->getLexeme())
    {
        fprintf(stderr, "[SEMANTIC ERROR - typeDeclNode] STRUCT WITHOUT A VALID NAME, line: %d\n",
                typeDeclNode->getLine());
        return;
    }
    if (structTable->cSearch(typeDeclNode->getId()->getLexeme()) || !structTable->cInsert(typeDeclNode->getId()->getLexeme(), typeDeclNode->getDecl()))
    {
        fprintf(stderr, "[SEMANTIC ERROR - typeDeclNode] STRUCT ALREADY EXISTS, line: %d, lexeme: %s \n",
                typeDeclNode->getLine(), typeDeclNode->getId()->getLexeme());
        return;
    }

    beginScope(typeDeclNode->getId()->getLexeme());
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

void SemanticTables::visit(TokenNode *tokenNode) {
    if(tokenNode->getToken() != ID)
        tokenNode->setTypeLexeme(token_id_to_name(tokenNode->getToken()));
}

void SemanticTables::visit(IdListNode *idListNode) {
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


/*    {
// TODO       fprintf(stderr, "[SEMANTIC ERROR - idListNode] VARIABLE ALREADY EXISTS, line: %d \n", idListNode->getLine());
    }*/
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

void SemanticTables::visit(VarDeclNode *varDeclNode) {

    if (varDeclNode->getNext()!=NULL)
    {
        varDeclNode->getNext()->accept(this);
    }

    if (varDeclNode->getIdList()!=NULL)
    {
        varDeclNode->getIdList()->accept(this);
    }

    IdListNode *idListAux = varDeclNode->getIdList();
    while (idListAux!=NULL)
    {
        //TODO verificar
        bool isPointer = varDeclNode->getType()->getId()->isPointer() || (varDeclNode->getIdList()->getPointer() != NULL);

        idListAux->getId()->setType(varDeclNode->getType()->getId()->getToken());
        idListAux->getId()->setTypeLexeme(varDeclNode->getType()->getId()->getLexeme());
        idListAux->getId()->setPointer(isPointer);
        idListAux->getId()->setArraySize(varDeclNode->getType()->getId()->getArraySize());
        if(varDeclNode->getType()->getType() == ID)
        {
            if (!structTable->cSearch(idListAux->getId()->getTypeLexeme()))
            {
                fprintf(stderr, "[SEMANTIC ERROR - VarDeclNode] UNKNOWN VARIABLE TYPE line: %d type: %s \n",
                        idListAux->getLine(), idListAux->getId()->getTypeLexeme());

                return;
            }

        }

        int arraySize = -1;

        if(idListAux->getArray())
        {
            arraySize = atoi(idListAux->getArray()->getLexeme());
        }

        if (!varTable->cInsert(varDeclNode->getType(), idListAux->getId()->getLexeme(), isPointer,
                               arraySize, BOOL_FALSE))
        {
            fprintf(stderr, "[SEMANTIC ERROR - VarDeclNode] DUPLICATED VARIABLE NAME line: %d lexeme: %s \n",
                    idListAux->getLine(), idListAux->getId()->getLexeme());

        }
        idListAux = idListAux->getNext();
    }

}

void SemanticTables::visit(FormalListNode *formalListNode) {

    if (formalListNode->getId()!=NULL)
    {
        formalListNode->getId()->accept(this);
    }

    if(!formalListNode->getId() || !formalListNode->getId()->getLexeme() )
    {
        fprintf(stderr, "[SEMANTIC ERROR - formalListNode] FUNCTION PARAMETER WITHOUT A NAME, line: %d\n",
                formalListNode->getLine());
    }

    const char* parameterName = formalListNode->getId()->getLexeme();
    int arraySize = -1;

    if(formalListNode->getArray())
        arraySize = formalListNode->getArray()->getArraySize();

    if (!varTable->cInsert(formalListNode->getType(), parameterName, formalListNode->getPointer()!=NULL, arraySize, BOOL_TRUE))
    {
        fprintf(stderr, "[SEMANTIC ERROR - formalListNode] VARIABLE ALREADY EXISTS, line: %d, lexeme: %s \n",
                formalListNode->getLine(), formalListNode->getId()->getLexeme());
    }


    if (formalListNode->getNext()!=NULL)
    {
        formalListNode->getNext()->accept(this);
    }
}

void SemanticTables::visit(FunctionNode *functionNode) {

    if (functionNode->getId()!=NULL)
    {
        functionNode->getId()->accept(this);
    }


    if(!functionNode->getId() || !functionNode->getId()->getLexeme())
    {
        fprintf(stderr, "[SEMANTIC ERROR - functionNode] FUNCTION WITHOUT A VALID NAME, line: %d\n",
                functionNode->getLine());
        return;
    }
    const char *functionLexeme = functionNode->getId()->getLexeme();

    if(varTable->searchInScope(functionLexeme, SCOPE_NON_LOCAL) || structTable->cSearch(functionLexeme))
    {
        fprintf(stderr, "[SEMANTIC ERROR - functionNode] FUNCTION NAME ALREADY ASSIGNED TO STRUCT OR NON LOCAL VAR, line: %d\n",
                functionNode->getLine());
        return;
    }

    if(functionNode->getReturnType() == ID) {

        if(!structTable->cSearch(functionNode->getReturnTypeLexeme()))
        {
            fprintf(stderr, "[SEMANTIC ERROR - functionNode] UNDEFINED RETURN TYPE, line: %d type: %s\n",
                    functionNode->getLine(), functionNode->getReturnTypeLexeme());

        }
    }

    if(!functionTable->cInsert(functionNode->getType(), functionLexeme,
                               functionNode->getParameters(), functionNode->getPointer())){
        fprintf(stderr, "[SEMANTIC ERROR - functionNode] FUNCTION NAME ALREADY ASSIGNED BEFORE, line: %d\n",
                functionNode->getLine());
        return;
    }

    activeFunction = functionTable->cSearch(functionLexeme);
    beginScope(functionLexeme);

    if (functionNode->getParameters()!=NULL)
    {
        functionNode->getParameters()->accept(this);
    }
    if (functionNode->getLocal()!=NULL)
    {
        functionNode->getLocal()->accept(this);
    }

    endScope();
}

void SemanticTables::visit(FunctionListNode *functionListNode) {
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

void SemanticTypes::visit(ProgramNode *programNode) {

    if (programNode->getFunctions())
    {
        programNode->getFunctions()->accept(this);
    }
}

void SemanticTypes::visit(FunctionNode *functionNode) {

    if (functionNode->getId()!=NULL)
    {
        functionNode->getId()->accept(this);
    }

    if(!functionNode->getId() || !functionNode->getId()->getLexeme() || !functionTable->cSearch(functionNode->getId()->getLexeme()))
        return;

    const char *functionLexeme = functionNode->getId()->getLexeme();

    activeFunction = functionTable->cSearch(functionLexeme);
    beginScope(functionLexeme);

    if (functionNode->getBody()!=NULL)
    {
        functionNode->getBody()->accept(this);
    }

    endScope();
}

void SemanticTypes::visit(FunctionListNode *functionListNode) {

    if (functionListNode->getNext())
    {
        functionListNode->getNext()->accept(this);
    }
    if (functionListNode->getFunction())
    {
        functionListNode->getFunction()->accept(this);
    }
}

void SemanticTypes::visit(BreakNode *breakNode)
{
    if (!breakDepth)
    {
        fprintf(stderr, "[SEMANTIC ERROR - breakNode] UNEXPECTED BREAK, line: %d \n", breakNode->getLine());
    }
}


void SemanticTypes::visit(WhileNode *whileNode)
{
    if(whileNode->getHead())
    {
        whileNode->getHead()->accept(this);
    } else {
        fprintf(stderr,
                "[SEMANTIC ERROR - whileNode] IF WITHOUT A VALID CONDITION, line: %d\n",
                whileNode->getLine());
        return;
    }

    if(whileNode->getBody())
    {
        breakDepth += 1;
        whileNode->getBody()->accept(this);
        breakDepth -= 1;
    }

    if(whileNode->getHead()->getType() != BOOL) {
        fprintf(stderr,
                "[SEMANTIC ERROR - whileNode] CONDITION EXPRESSION MUST BE BOOLEAN, line: %d\n",
                whileNode->getLine());
    }

    whileNode->setReturn(whileNode->getBody() && whileNode->getBody()->isReturn());

}

void SemanticTypes::visit(PrintNode *printNode) {

    if(printNode->getExpList())
        printNode->getExpList()->accept(this);

    printNode->setReturn(false);
}

void SemanticTypes::visit(ThrowNode *throwNode) {

}

void SemanticTypes::visit(SwitchNode *switchNode) {

    if (switchNode->getExp()!=NULL)
    {
        switchNode->getExp()->accept(this);
        if (switchNode->getExp()->getType()!=INT)
        {
            fprintf(stderr, "[SEMANTIC ERROR - switchNode] CONDITION MUST BE INT, line: %d\n", switchNode->getLine());
        }

        switchNode->setLine(switchNode->getExp()->getLine());
    } else {
        fprintf(stderr, "[SEMANTIC ERROR - switchNode] NO VALID CONDITION, line: %d\n", switchNode->getLine());
    }
    if (switchNode->getBlock()!=NULL)
    {
        breakDepth += 1;
        switchNode->getBlock()->accept(this);
        breakDepth -= 1;
    }
    else
    {
        fprintf(stderr, "[SEMANTIC ERROR - switchNode] CASE STATEMENT REQUIRED, line: %d\n", switchNode->getLine());
    }
}

void SemanticTypes::visit(TryNode *tryNode) {

    if(tryNode->getTry())
    {
        tryNode->getTry()->accept(this);
    }

    if(tryNode->getCatch())
    {
        tryNode->getCatch()->accept(this);
    }

}


void SemanticTypes::visit(ReadLnNode *readLnNode) {

    if(readLnNode->getExp())
    {
        readLnNode->getExp()->accept(this);
    }

}
void SemanticTypes::visit(IfNode *ifNode)
{
    if(ifNode->getHead())
    {
        ifNode->getHead()->accept(this);
    } else {
        fprintf(stderr,
                "[SEMANTIC ERROR - ifNode] IF WITHOUT A VALID CONDITION, line: %d\n",
                ifNode->getLine());
        return;
    }

    if(ifNode->getTrueStmt())
    {
        ifNode->getTrueStmt()->accept(this);
    }

    if(ifNode->getFalseStmt())
    {
        ifNode->getFalseStmt()->accept(this);
    }

    if(ifNode->getHead()->getType() != BOOL) {
        fprintf(stderr,
                "[SEMANTIC ERROR - ifNode] CONDITION EXPRESSION MUST BE BOOLEAN, line: %d\n",
                ifNode->getLine());
    }

    ifNode->setReturn(ifNode->getFalseStmt() && ifNode->getTrueStmt() && ifNode->getTrueStmt()->isReturn() && ifNode->getFalseStmt()->isReturn());
}

void SemanticTypes::visit(StmtListNode *stmtListNode)
{

    bool isNextReturn = false;

    //TODO implementar checagem de retorno
    if (stmtListNode->getNext())
    {
        stmtListNode->getNext()->accept(this);
        isNextReturn = stmtListNode->getNext()->isReturn();
    }

    if (stmtListNode->getStmt())
    {
        stmtListNode->getStmt()->accept(this);
        stmtListNode->setReturn(stmtListNode->getStmt()->isReturn());


    }
}

void SemanticTypes::visit(ReturnNode *returnNode)
{
    if(returnNode->getExp()) { //Check the return expression
        returnNode->getExp()->accept(this);
    } else { //No expression means void return

        fprintf(stderr,
                "[SEMANTIC ERROR - returnNode] VOID RETURN STATEMENT NOT ALLOWED, line: %d\n",
                returnNode->getLine());


        return;
    }

    if(!activeFunction)
    {
        fprintf(stderr,
                "[SEMANTIC ERROR - returnNode] RETURN STATEMENT OUTSIDE A FUNCTION, line: %d\n",
                returnNode->getLine());
        return;
    }

    bool isSameType = returnNode->getExp()->getType() == activeFunction->getReturnType()->getType();
    bool isSameArray = returnNode->getExp()->getArraySize() < 0;
    bool isSamePointer = returnNode->getExp()->isPointer() == activeFunction->isPointer();
    bool isSameTypeLexeme = true;
    if(returnNode->getExp()->getType() == ID)
        isSameTypeLexeme = !strcmp(returnNode->getExp()->getTypeLexeme(), activeFunction->getReturnType()->getTypeLexeme());

    if(!(isSameArray && isSameType && isSamePointer && isSameTypeLexeme))
    {
        fprintf(stderr,
                "[SEMANTIC ERROR - returnNode] RETURN TYPE DOES NOT MATCH WITH FUNCTION RETURN TYPE, line: %d\n",
                returnNode->getLine());
        return;
    }

    returnNode->setReturn(true);
}

//TODO Tá MUUUUUUITO ERRADO ISSO, "se tiver que usar typeof no visitor é pq ta errado"
void SemanticTypes::visit(StmtNode *stmtNode)
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

void SemanticTypes::visit(AssignNode *assignNode)
{
    if(assignNode->getExp1())
        assignNode->getExp1()->accept(this);
    else {
        fprintf(stderr,
                "[SEMANTIC ERROR - assignNode] NO EXPRESSION TO BE ASSIGNED, line: %d\n",
                assignNode->getLine());
        return;
    }

    if(assignNode->getExp2())
        assignNode->getExp2()->accept(this);
    else {
        fprintf(stderr,
                "[SEMANTIC ERROR - assignNode] NO EXPRESSION TO ASSIGN, line: %d\n",
                assignNode->getLine());
        return;
    }


    if(!_match_exp_types(assignNode->getExp1(), assignNode->getExp2()))
    {
        fprintf(stderr,
                "[SEMANTIC ERROR - assignNode] CANNOT ASSIGN AN EXPRESSION TO A DIFFERENT TYPE, line: %d\n",
        assignNode->getLine());
        return;
    }

    if(!assignNode->getExp1()->isLValue())
    {
        fprintf(stderr,
                "[SEMANTIC ERROR - assignNode] LVALUE EXPECTED, line: %d expected: %s got: %s\n",
                assignNode->getExp1()->getLine(), assignNode->getExp1()->getTypeLexeme(), assignNode->getExp1()->getTypeLexeme());
        return;
    }



    //TODO verificar se precisa checar rvalue
}

void SemanticTypes::visit(PointerValNode *pointerValNode) {

    if(pointerValNode->getExp())
    {
        pointerValNode->getExp()->accept(this);
    } else {
        fprintf(stderr, "[SEMANTIC ERROR - pointerValNode] INVALID VALUE EXPRESSION, line %d\n",
                pointerValNode->getLine());
        return;
    }

    if(!pointerValNode->getExp()->isPointer()) {
        fprintf(stderr, "[SEMANTIC ERROR - pointerValNode] CANNOT ACCESS VALUE OF A NON POINTER TYPE, line %d\n",
                pointerValNode->getLine());
        return;
    }

    pointerValNode->setType(pointerValNode->getExp()->getType());
    pointerValNode->setTypeLexeme(pointerValNode->getExp()->getTypeLexeme());
    pointerValNode->setLValue(true);//pointerValNode->setLValue(pointerValNode->getExp()->isLValue());
    pointerValNode->setArraySize(pointerValNode->getExp()->getArraySize());
    pointerValNode->setPointer(false);

}

void SemanticTypes::visit(AddressValNode *addressValNode) {

    if(addressValNode->getExp())
    {
        addressValNode->getExp()->accept(this);
    } else {
        fprintf(stderr, "[SEMANTIC ERROR - addressValNode] INVALID VALUE EXPRESSION, line %d\n",
                addressValNode->getLine());
        return;
    }

    if(addressValNode->getExp()->isPointer()) {
        fprintf(stderr, "[SEMANTIC ERROR - addressValNode] CANNOT ACCESS ADDRESS OF A POINTER TYPE, line %d lexeme: %s\n",
                addressValNode->getLine(), addressValNode->getLexeme());
        return;
    }

    addressValNode->setType(addressValNode->getExp()->getType());
    addressValNode->setTypeLexeme(addressValNode->getExp()->getTypeLexeme());
    addressValNode->setLValue(true);//pointerValNode->setLValue(pointerValNode->getExp()->isLValue());
    addressValNode->setArraySize(addressValNode->getExp()->getArraySize());
    addressValNode->setPointer(true);
}

void SemanticTypes::visit(PointerExpNode *pointerExpNode)
{
    if(pointerExpNode->getId()) {
        pointerExpNode->getId()->accept(this);
    }else {
        fprintf(stderr, "[SEMANTIC ERROR - pointerExpNode] NO RECORD ACCESS EXPRESSION, line %d\n",
                pointerExpNode->getExp()->getLine());
        return;
    }

    if(pointerExpNode->getExp()) {
        pointerExpNode->getExp()->accept(this);
    }else {
        fprintf(stderr, "[SEMANTIC ERROR - pointerExpNode] NO STRUCT ACCESS FIELD, line %d\n",
                pointerExpNode->getExp()->getLine());
        return;
    }

    if(!pointerExpNode->getExp()->isPointer() || pointerExpNode->getExp()->getArraySize() > 0)
    {
        fprintf(stderr, "[SEMANTIC ERROR - pointerExpNode] INCORRECT ACCESS TO A STRUCT POINTER, line %d\n",
                pointerExpNode->getExp()->getLine());
        return;
    }
    VarSymbol *recordField = varTable->searchInScope(pointerExpNode->getId()->getLexeme(), pointerExpNode->getExp()->getTypeLexeme());

    if(!recordField)
    {
        fprintf(stderr, "[SEMANTIC ERROR - pointerExpNode] UNKNOWN FIELD IN STRUCT, line: %d field: %s type: %s\n",
                pointerExpNode->getExp()->getLine(), pointerExpNode->getId()->getLexeme(), pointerExpNode->getExp()->getTypeLexeme());
        return;
    }

    pointerExpNode->setType(recordField->getType()->getType());
    pointerExpNode->setTypeLexeme(recordField->getType()->getTypeLexeme());
    pointerExpNode->setPointer(recordField->isPointer());
    pointerExpNode->setArraySize(recordField->getArraySize());
    pointerExpNode->setLValue(true);
}

void SemanticTypes::visit(NameExpNode *nameExpNode)
{
    if(nameExpNode->getId()) {
        nameExpNode->getId()->accept(this);
    }else {
        fprintf(stderr, "[SEMANTIC ERROR - nameExpNode] NO RECORD ACCESS EXPRESSION, line %d\n",
                nameExpNode->getExp()->getLine());
        return;
    }

    if(nameExpNode->getExp()) {
        nameExpNode->getExp()->accept(this);
    }else {
        fprintf(stderr, "[SEMANTIC ERROR - nameExpNode] NO STRUCT ACCESS FIELD, line %d\n",
                nameExpNode->getExp()->getLine());
        return;
    }

    if(nameExpNode->getExp()->isPointer() || nameExpNode->getExp()->getArraySize() > 0)
    {
        fprintf(stderr, "[SEMANTIC ERROR - nameExpNode] INCORRECT ACCESS TO A STRUCT, line %d\n",
                nameExpNode->getExp()->getLine());
        return;
    }
    VarSymbol *recordField = varTable->searchInScope(nameExpNode->getId()->getLexeme(), nameExpNode->getExp()->getTypeLexeme());

    if(!recordField)
    {
        fprintf(stderr, "[SEMANTIC ERROR - nameExpNode] UNKNOWN FIELD IN STRUCT, line: %d field: %s type: %s\n",
                nameExpNode->getExp()->getLine(), nameExpNode->getId()->getLexeme(), nameExpNode->getExp()->getTypeLexeme());
        return;
    }

    nameExpNode->setType(recordField->getType()->getType());
    nameExpNode->setTypeLexeme(recordField->getType()->getTypeLexeme());
    nameExpNode->setPointer(recordField->isPointer());
    nameExpNode->setArraySize(recordField->getArraySize());
    nameExpNode->setLValue(true);
}

void SemanticTypes::visit(ArrayCallNode *arrayCallNode)
{
    if(arrayCallNode->getExp())
    {
        arrayCallNode->getExp()->accept(this);
    } else {
        fprintf(stderr, "[SEMANTIC ERROR - arrayCallNode] INVALID ARRAY ACCESS EXPRESSION, line %d\n",
                arrayCallNode->getLine(), arrayCallNode->getLexeme());
        return;
    }

    if(arrayCallNode->getIndex())
    {
        arrayCallNode->getIndex()->accept(this);
    } else {
        fprintf(stderr, "[SEMANTIC ERROR - arrayCallNode] INVALID ARRAY INDEX EXPRESSION, line %d\n",
                arrayCallNode->getLine(), arrayCallNode->getLexeme());
        return;
    }

    if(arrayCallNode->getExp()->getArraySize() < 0)
    {
        fprintf(stderr, "[SEMANTIC ERROR - arrayCallNode] ARRAY ACCESS TO A NON ARRAY TYPE, line %d\n",
                arrayCallNode->getLine(), arrayCallNode->getTypeLexeme());
        return;
    }


    if(arrayCallNode->getIndex()->getType() != INT)
    {
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

void SemanticTypes::visit(MultiplicationOPNode *multiplicationOpNode) {

    if(multiplicationOpNode->getExp1())
        multiplicationOpNode->getExp1()->accept(this);
    else {
        fprintf(stderr,
                "[SEMANTIC ERROR - multiplicationOpNode] NO EXPRESSION TO BE ASSIGNED, line: %d\n",
                multiplicationOpNode->getLine());
        return;
    }

    if(multiplicationOpNode->getExp2())
        multiplicationOpNode->getExp2()->accept(this);
    else {
        fprintf(stderr,
                "[SEMANTIC ERROR - multiplicationOpNode] NO EXPRESSION TO ASSIGN, line: %d\n",
                multiplicationOpNode->getLine());
        return;
    }


    if(!_match_exp_types(multiplicationOpNode->getExp1(), multiplicationOpNode->getExp2()))
    {
        fprintf(stderr,
                "[SEMANTIC ERROR - multiplicationOpNode] CANNOT ASSIGN AN EXPRESSION TO A DIFFERENT TYPE, line: %d\n",
                multiplicationOpNode->getLine());
        return;
    }

    if(multiplicationOpNode->getExp1()->getType() != INT && multiplicationOpNode->getExp1()->getType() != FLOAT)
    {
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

void SemanticTypes::visit(AdditionOPNode *additionOpNode) {

    if(additionOpNode->getExp1())
        additionOpNode->getExp1()->accept(this);
    else {
        fprintf(stderr,
                "[SEMANTIC ERROR - additionOpNode] NO EXPRESSION TO BE ASSIGNED, line: %d\n",
                additionOpNode->getLine());
        return;
    }

    if(additionOpNode->getExp2())
        additionOpNode->getExp2()->accept(this);
    else {
        fprintf(stderr,
                "[SEMANTIC ERROR - additionOpNode] NO EXPRESSION TO ASSIGN, line: %d\n",
                additionOpNode->getLine());
        return;
    }


    if(!_match_exp_types(additionOpNode->getExp1(), additionOpNode->getExp2()))
    {
        fprintf(stderr,
                "[SEMANTIC ERROR - additionOpNode] CANNOT ASSIGN AN EXPRESSION TO A DIFFERENT TYPE, line: %d\n",
                additionOpNode->getLine());
        return;
    }

    if(additionOpNode->getExp1()->getType() != INT && additionOpNode->getExp1()->getType() != FLOAT)
    {
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

void SemanticTypes::visit(ArrayNode *arrayNode) {

    if(!arrayNode->getNumInt())
    {
        fprintf(stderr, "[SEMANTIC ERROR - arrayNode] INVALID ARRAY SIZE, line: %d\n", arrayNode->getLine());
        return;
    }

}

void SemanticTypes::visit(NotNode *notNode){

    notNode->setLValue(false);
    notNode->setArraySize(-1);
    notNode->setPointer(false);
    notNode->setType(BOOL);

    if(notNode->getExp())
    {
        notNode->getExp()->accept(this);
    } else {
        fprintf(stderr, "[SEMANTIC ERROR - signNode] INVALID EXPRESSION, line %d\n", notNode->getLine());
        return;
    }

    bool isArray = notNode->getExp()->getArraySize() > -1;
    if(notNode->getExp()->getType() != BOOL || isArray || notNode->getExp()->isPointer())
    {
        fprintf(stderr, "[SEMANTIC ERROR - signNode] CANNOT NEGATIVE A NON BOOLEAN VALUE, line %d\n", notNode->getLine());
        return;
    }

}

void SemanticTypes::visit(BooleanOPNode *booleanOpNode)
{

    booleanOpNode->setType(BOOL);
    booleanOpNode->setLValue(false);
    booleanOpNode->setPointer(false);
    booleanOpNode->setArraySize(-1);

    if(booleanOpNode->getExp1())
    {
        booleanOpNode->getExp1()->accept(this);
    } else {
        fprintf(stderr, "[SEMANTIC ERROR - booleanOpNode] INVALID EXPRESSION 1, line %d\n", booleanOpNode->getLine());
        return;
    }

    if(booleanOpNode->getExp2())
    {
        booleanOpNode->getExp2()->accept(this);
    } else {
        fprintf(stderr, "[SEMANTIC ERROR - booleanOpNode] INVALID EXPRESSION 2, line %d\n", booleanOpNode->getLine());
        return;
    }

    if(booleanOpNode->getOp())
    {
        booleanOpNode->getOp()->accept(this);
    } else {
        fprintf(stderr, "[SEMANTIC ERROR - booleanOpNode] INVALID OPERATOR, line %d\n", booleanOpNode->getLine());
        return;
    }

    if(!_match_exp_types(booleanOpNode->getExp1(), booleanOpNode->getExp2()))
    {
        fprintf(stderr, "[SEMANTIC ERROR - booleanOpNode] CANNOT COMPARE DIFFERENT TYPES, line %d\n", booleanOpNode->getLine());
        return;
    }

    bool isArray = booleanOpNode->getExp1()->getArraySize() > -1;
    if(booleanOpNode->getOp()->getToken() != NE && booleanOpNode->getOp()->getToken() != EQ)
        if((booleanOpNode->getExp1()->getType() != INT && booleanOpNode->getExp1()->getType() != FLOAT) || isArray || booleanOpNode->getExp1()->isPointer())
        {
            fprintf(stderr, "[SEMANTIC ERROR - booleanOpNode] CANNOT COMPARE WITH NON NUMERIC TYPES, line %d\n", booleanOpNode->getLine());
            return;
        }

}

void SemanticTypes::visit(SignNode *signNode)
{
    if(signNode->getExp())
    {
        signNode->getExp()->accept(this);
    } else {
        fprintf(stderr, "[SEMANTIC ERROR - signNode] INVALID EXPRESSION, line %d\n", signNode->getLine());
        return;
    }

    bool isArray = signNode->getExp()->getArraySize() > -1;
    if((signNode->getExp()->getType() != INT && signNode->getExp()->getType() != FLOAT) || isArray || signNode->getExp()->isPointer())
    {
        fprintf(stderr, "[SEMANTIC ERROR - signNode] CANNOT NEGATIVE A NON NUMERIC VALUE, line %d\n", signNode->getLine());
        return;
    }

    signNode->setType(signNode->getExp()->getType());
    signNode->setTypeLexeme(signNode->getExp()->getTypeLexeme());
    signNode->setPointer(false);
    signNode->setArraySize(-1);
    signNode->setLValue(false);
}

void SemanticTypes::visit(CallNode *callNode)
{

    if(callNode->getId())
    {
        callNode->getId()->accept(this);
    }
    else
    {
        fprintf(stderr, "[SEMANTIC ERROR - callNode] INVALID FUNCTION CALL, line %d\n", callNode->getLine());
        return;
    }

    if(callNode->getParameters())
    {
        callNode->getParameters()->accept(this);
    }

    FunctionSymbol *func = functionTable->cSearch(callNode->getId()->getLexeme());


    if(!func)
    {
        fprintf(stderr, "[SEMANTIC ERROR - callNode] CALL TO UNDEFINED FUNCTION, line: %d lexeme: %s\n", callNode->getLine(), callNode->getId()->getLexeme());
        return;
    }

    FormalListNode* expectedParameter = func->getVarDecl();
    ExpListNode* parameter = callNode->getParameters();

    while(expectedParameter && parameter) {

        parameter->accept(this);

        bool isSameType = expectedParameter->getParameterType() == parameter->getExp()->getType();
        bool isSameArray = (expectedParameter->getArray()!= NULL) == (parameter->getExp()->getArraySize() > 0);
        bool isSamePointer = (expectedParameter->getPointer() != NULL) == parameter->getExp()->isPointer();
        bool isSameTypeLexeme = true;

        if(expectedParameter->getParameterType() == ID)
            isSameTypeLexeme = !strcmp(expectedParameter->getParameterTypeLexeme(), parameter->getExp()->getTypeLexeme());

        if(!(isSameArray && isSamePointer && isSameType && isSameTypeLexeme))
        {
            fprintf(stderr, "[SEMANTIC ERROR - callNode] PARAMETER TYPE MISMATCH, line: %d parameter: %s\n", parameter->getLine(), expectedParameter->getId()->getLexeme());
            return;
        }

        parameter = parameter->getNext();
        expectedParameter = expectedParameter->getNext();
    }

    if(expectedParameter || parameter)
    {
        fprintf(stderr, "[SEMANTIC ERROR - callNode] PARAMETER COUNT MISMATCH, line: %d lexeme: %s\n", parameter->getLine(), callNode->getId()->getLexeme());
        return;
    }

    callNode->setLexeme(callNode->getId()->getLexeme());
    callNode->setType(func->getReturnType()->getType());
    callNode->setTypeLexeme(func->getReturnType()->getTypeLexeme());
    callNode->setPointer(func->isPointer());
    callNode->setArraySize(-1);
    callNode->setLValue(false);
}

void SemanticTypes::visit(PrimaryNode *primaryNode) {

    //TODO pq eu preciso de um token node e um exp node se eh td exp???
    ExpNode *expNode = primaryNode->getExp();

    if(expNode) {
        expNode->accept(this);

        primaryNode->setType(expNode->getType());
        primaryNode->setTypeLexeme(expNode->getLexeme());
        primaryNode->setPointer(expNode->isPointer());
        primaryNode->setArraySize(expNode->getArraySize());
        primaryNode->setLValue(expNode->isLValue());
    }


    TokenNode *tokenNode = primaryNode->getTokenNode();
    if(tokenNode) {
        tokenNode->accept(this);

        VarSymbol *varSymbol = varTable->cSearch(tokenNode->getLexeme());

        //if token is a variable, check if it exists
        if((tokenNode->getType() == ID || tokenNode->getType() == UNASSIGNED_TOKEN))
        {
            if(varSymbol) {

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

void SemanticTypes::visit(TokenNode *tokenNode)
{
    VarSymbol* var = varTable->cSearch(tokenNode->getLexeme());
    FunctionSymbol *func = functionTable->cSearch(tokenNode->getLexeme());

    if(var)
    {
        tokenNode->setType(var->getType()->getType());
        tokenNode->setTypeLexeme(var->getType()->getTypeLexeme());
        tokenNode->setArraySize(var->getArraySize());
        tokenNode->setPointer(var->isPointer());
        tokenNode->setLValue(true);
    } else if(func) {

        tokenNode->setLValue(false);
        tokenNode->setType(func->getReturnType()->getType());
        tokenNode->setTypeLexeme(func->getReturnType()->getTypeLexeme());
        tokenNode->setPointer(func->isPointer());
        tokenNode->setArraySize(-1);
    }

}

void SemanticTypes::visit(ExpListNode *expListNode)
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