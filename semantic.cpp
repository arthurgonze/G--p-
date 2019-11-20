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
}
void endSemantic()
{
    varTable->print();
    functionTable->print();
    structTable->print();

    activeFunction = NULL;
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
    IdListNode *id_list_aux = varDeclNode->getIdList();
    while (id_list_aux!=NULL)
    {
        id_list_aux->getId()->setType(varDeclNode->getType()->getId()->getToken());
        id_list_aux->getId()->setTypeLexeme(varDeclNode->getType()->getId()->getLexeme());
        id_list_aux = id_list_aux->getNext();
    }
    if (varDeclNode->getIdList()!=NULL)
    {
        varDeclNode->getIdList()->accept(this);
    }
    if (varDeclNode->getNext()!=NULL)
    {
        varDeclNode->getNext()->accept(this);
    }

    if (varDeclNode->getType()!=NULL && varDeclNode->getIdList()!=NULL)
    {
        if (varDeclNode->getType()->getId()->getToken()==ID)
        {
            if (!structTable->cSearch(varDeclNode->getType()->getId()->getLexeme()))
            {
                fprintf(stderr, "[SEMANTIC ERROR - VarDeclNode] TYPE NOT DEFINED, line: %d \n", varDeclNode->getLine());
            }
        }
    }
    if (varDeclNode->getIdList()!=NULL)
    {
        varDeclNode->getIdList()->accept(this);
        IdListNode *list_aux = varDeclNode->getIdList();
        int total_size_aux = 0;
        while (list_aux!=NULL)
        {
            VarSymbol *var;
            if (activeFunction!=NULL)
            {
                const char *aux1 = list_aux->getId()->getLexeme();
                const char *aux2 = activeFunction->getLexeme();
                var = varTable->searchInScope(aux1, aux2);
            }
            else
            {
                const char *aux = list_aux->getId()->getLexeme();
                var = varTable->cSearch(aux);
            }
            int size_aux = BYTE_SIZE;
            if (list_aux->getArray()!=NULL)
            {
                size_aux *= atoi(list_aux->getArray()->getNumInt()->getLexeme());
            }
            if (activeFunction!=NULL)
            {
                list_aux->getId()->setOffset(activeFunction->getLocalSize() + total_size_aux + size_aux);
            }
            var->setSize(size_aux);
            var->setOffset(list_aux->getId()->getOffset());
            total_size_aux += size_aux;
            list_aux = list_aux->getNext();
        }
        if (activeFunction!=NULL)
        {
            activeFunction->incrementLocalSize(total_size_aux);
        }
    }

    if (varDeclNode->getNext()!=NULL)
    {
        varDeclNode->getNext()->accept(this);
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
    TypeNode *typeNode = new TypeNode(idListNode->getId(), idListNode->getId()->getTypeLexeme());
    typeNode->setLine(idListNode->getLine());
    if (!varTable->cInsert(typeNode, idListNode->getId()->getLexeme(), idListNode->getId()->isPointer(), idListNode->getId()->getArraySize(), BOOL_FALSE))
    {
        // TODO fprintf(stderr, "[SEMANTIC ERROR - idListNode] VARIABLE ALREADY EXISTS, line: %d \n", idListNode->getLine());
    }

    if (idListNode->getNext()!=NULL)
    {
        idListNode->getNext()->accept(this);
    }

    if (idListNode->getId()!=NULL)
    {
        idListNode->getId()->accept(this);
    }
    if (idListNode->getArray()!=NULL)
    {
        idListNode->getArray()->accept(this);
    }
    if (idListNode->getNext()!=NULL)
    {
        idListNode->getNext()->accept(this);
    }
}

void Semantic::visit(TypeDeclNode *typeDeclNode)
{
    const char *typeLex = typeDeclNode->getId()!=NULL ? typeDeclNode->getId()->getLexeme() : NULL;

    if (typeDeclNode->getNext()!=NULL)
    {
        typeDeclNode->getNext()->accept(this);
    }

    beginScope(typeLex);
    if (typeDeclNode->getDecl()!=NULL)
    {
        typeDeclNode->getDecl()->accept(this);
    }
    endScope();

    if (!structTable->cInsert(typeLex, typeDeclNode->getDecl()))
    {
        fprintf(stderr, "[SEMANTIC ERROR - typeDeclNode] STRUCT ALREADY EXISTS, line: %d \n", typeDeclNode->getLine());
    }
    if (typeDeclNode->getNext()!=NULL)
    {
        typeDeclNode->getNext()->accept(this);
    }

    beginScope(typeLex);
    if (typeDeclNode->getDecl()!=NULL)
    {
        typeDeclNode->getDecl()->accept(this);
        VarDeclNode *var_list_aux = typeDeclNode->getDecl();
        int total_size_aux = 0;
        while (var_list_aux!=NULL)
        {
            IdListNode *id_list_aux = var_list_aux->getIdList();
            while (id_list_aux!=NULL)
            {
                VarSymbol *var = varTable->searchInScope(id_list_aux->getId()->getLexeme(), typeDeclNode->getId()->getLexeme());
                int size_aux = BYTE_SIZE;
                if (id_list_aux->getArray()!=NULL)
                {
                    size_aux *= atoi(id_list_aux->getArray()->getNumInt()->getLexeme());
                }
                var->setSize(size_aux);
                var->setOffset(total_size_aux);
                total_size_aux += size_aux;
                id_list_aux = id_list_aux->getNext();
            }
            var_list_aux = var_list_aux->getNext()->getNext();
        }
        StructSymbol *structSymbol = structTable->cSearch(typeDeclNode->getId()->getLexeme());
        structSymbol->setSize(total_size_aux);
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
    const char *idLexAux = formalListNode->getId()!=NULL ? formalListNode->getId()->getLexeme() : NULL;
    if (formalListNode->getId()!=NULL)
    {
        formalListNode->getId()->accept(this);
    }
    if (formalListNode->getArray()!=NULL)
    {
        formalListNode->getArray()->accept(this);
    }
    int arraySize = formalListNode->getArray()!=NULL ? atoi(formalListNode->getArray()->getNumInt()->getLexeme()) : -1;

    if (!varTable->cInsert(formalListNode->getType(), idLexAux, formalListNode->getPointer()!=NULL, arraySize, BOOL_TRUE))
    {
        fprintf(stderr, "[SEMANTIC ERROR - formalListNode] VARIABLE ALREADY EXISTS, line: %d \n", formalListNode->getLine());
    }

    if (formalListNode->getNext()!=NULL)
    {
        formalListNode->getNext()->accept(this);
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
    if (formalListNode->getNext()!=NULL)
    {
        formalListNode->getNext()->accept(this);
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
                if (exp->getExp()->getType()!=param->getType()->getId()->getToken() ||
                    exp->getExp()->getTypeLexeme()!=param->getType()->getId()->getLexeme())
                {
                    fprintf(stderr, "[SEMANTIC ERROR - callNode] PARAMETER TYPE MISMATCH, line: %d \n", callNode->getLine());

                    exp = exp->getNext();
                    param = param->getNext();
                    break;
                }
                exp = exp->getNext();
                param = param->getNext();
            }
            if (exp==NULL && param!=NULL)
            {
                fprintf(stderr, "[SEMANTIC ERROR - callNode] TOO FEW ARGUMENTS, line: %d \n", callNode->getLine());
            }
            else if (exp!=NULL && param==NULL)
            {
                fprintf(stderr, "[SEMANTIC ERROR - callNode] TOO MANY ARGUMENTS, line: %d \n", callNode->getLine());
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
        fprintf(stderr, "[SEMANTIC ERROR - callNode] FUNCTION NOT DECLARED, line: %d \n", callNode->getLine());
    }
}

void Semantic::visit(PrimaryNode *primaryNode)
{
    if (primaryNode->getExp()!=NULL)
    {
        primaryNode->getExp()->accept(this);
        if (TokenNode *id = dynamic_cast<TokenNode *>(primaryNode->getExp()))
        {
            primaryNode->setLValue(BOOL_TRUE);
            if (primaryNode->getExp()!=NULL && typeid(*primaryNode->getExp())==typeid(CallNode))
            {
                CallNode *function_call = dynamic_cast<CallNode *>(primaryNode->getExp());
                primaryNode->getExp()->accept(this);
                primaryNode->setType(primaryNode->getExp()->getType());
                primaryNode->setTypeLexeme(primaryNode->getExp()->getTypeLexeme());
            }
            else
            {
                VarSymbol *var_symbol = varTable->cSearch(id->getLexeme());

                if (var_symbol!=NULL)
                {

                    id->setType(var_symbol->getType()->getId()->getToken());
                    id->setTypeLexeme(var_symbol->getType()->getId()->getLexeme());

                    if (var_symbol->getType()->getId()->getToken()!=INT && var_symbol->getType()->getId()->getToken()!=FLOAT
                        && var_symbol->getType()->getId()->getToken()!=CHAR && var_symbol->getType()->getId()->getToken()!=LITERAL
                        && var_symbol->getType()->getId()->getToken()!=BOOL)
                    {
                        fprintf(stderr, "[SEMANTIC ERROR - primaryNode] TYPE NOT DEFINED, line: %d \n", primaryNode->getLine());
                    }
                    else
                    {
                        primaryNode->setType(primaryNode->getExp()->getType());
                        primaryNode->setTypeLexeme(primaryNode->getExp()->getTypeLexeme());
                    }
                }
                else
                {
                    fprintf(stderr, "[SEMANTIC ERROR - primaryNode] VARIABLE NOT DECLARED, line: %d \n", primaryNode->getLine());
                }

            }
        }
        else if ((dynamic_cast<CallNode *>(primaryNode->getExp())->getId()->getToken()==NUMINT || dynamic_cast<CallNode *>(primaryNode->getExp())->getId()->getToken()==NUMFLOAT ||
            dynamic_cast<CallNode *>(primaryNode->getExp())->getId()->getToken()==LITERALCHAR || dynamic_cast<CallNode *>(primaryNode->getExp())->getId()->getToken()==LITERAL ||
            dynamic_cast<CallNode *>(primaryNode->getExp())->getId()->getToken()==TRUE || dynamic_cast<CallNode *>(primaryNode->getExp())->getId()->getToken()==FALSE))
        {
            if (primaryNode->getExp()!=NULL)
            {
                fprintf(stderr, "[SEMANTIC ERROR - primaryNode] UNEXPECTED ACCESS IN NOT ID TYPE, line: %d \n", primaryNode->getLine());
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
        dynamic_cast<CallNode *>(primaryNode->getExp())->setLexeme(dynamic_cast<CallNode *>(primaryNode->getExp())->getLexeme());
    }
}

void Semantic::visit(FunctionNode *functionNode)
{
    const char *function_lex = functionNode->getId()!=NULL ? functionNode->getId()->getLexeme() : NULL;

    if (!functionTable->cInsert(functionNode->getType(), function_lex, functionNode->getParameters(), functionNode->getPointer()!=NULL))
    {
        fprintf(stderr, "[SEMANTIC ERROR - functionNode] FUNCTION ALREADY EXISTS, line: %d \n", functionNode->getLine());
    }

    if (functionNode->getBody()!=NULL)
    {
        functionNode->getBody()->accept(this);
    }

    ///*** Check if type was declared
    if (functionNode->getType()!=NULL && functionNode->getId()!=NULL)
    {
        if (functionNode->getType()->getId()->getToken()==ID)
        {
            if (!structTable->cSearch(functionNode->getType()->getLexeme()))
            {
                fprintf(stderr, "[SEMANTIC ERROR - functionNode] RETURN TYPE NOT DEFINED, line: %d \n", functionNode->getLine());
            }
        }
    }

    activeFunction = functionTable->cSearch(function_lex);
    beginScope(function_lex);
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
    VarDeclNode *var_stmt_aux = functionNode->getLocal();
    while (var_stmt_aux!=NULL && var_stmt_aux->getNext()!=NULL)
    {
        var_stmt_aux = var_stmt_aux->getNext();
    }
    if (var_stmt_aux!=NULL && var_stmt_aux->getIdList()!=NULL)
    {
        StmtListNode *stmt_list_aux = functionNode->getBody();
        while (stmt_list_aux!=NULL && stmt_list_aux->getStmt()!=NULL)
        {
            if (typeid(*stmt_list_aux->getStmt())==typeid(ReturnNode))
            {
                stmt_list_aux->getStmt()->accept(this);
                ReturnNode *return_stmt = dynamic_cast<ReturnNode *>(stmt_list_aux->getStmt());
                if (functionNode->getType()->getId()->getToken()!=return_stmt->getExp()->getType())
                {
                    fprintf(stderr, "[SEMANTIC ERROR - functionNode] INCOMPATIBLE RETURN TYPE, line: %d \n", functionNode->getLine());
                }
                stmt_list_aux = stmt_list_aux->getNext();
            }
            else
            {
                if (stmt_list_aux->getNext()==NULL)
                {
                    fprintf(stderr, "[SEMANTIC ERROR - functionNode] MISSING RETURN STATEMENT, line: %d \n", functionNode->getLine());
                    break;
                }
                else
                {
                    stmt_list_aux = stmt_list_aux->getNext();
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
            fprintf(stderr, "[SEMANTIC ERROR - read] LVALUE EXPECTED, line: %d \n", read->getExp()->getLine()); // TODO read->exp->line?
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
        if (whileNode->getHead()->getType()!=BOOL)
        {
            fprintf(stderr, "[SEMANTIC ERROR - whileNode] BOOLEAN EXPRESSION REQUIRED, line: %d \n", whileNode->getLine());
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
            fprintf(stderr, "[SEMANTIC ERROR - ifNode] BOOLEAN EXPRESSION REQUIRED, line: %d \n", ifNode->getLine());
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
            fprintf(stderr, "[SEMANTIC ERROR - switchNode] INT VALUE REQUIRED, line: %d \n", switchNode->getLine());
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
        fprintf(stderr, "[SEMANTIC ERROR - switchNode] CASE STATEMENT REQUIRED, line: %d \n", switchNode->getLine());
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
        fprintf(stderr, "[SEMANTIC ERROR - additionOPNode] ARITHMETIC OP TYPE, line: %d \n", additionOPNode->getLine());
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

    if (!assignNode->getExp1()->isLValue())
    {
        fprintf(stderr, "[SEMANTIC ERROR - assignNode] LVALUE EXPECTED: %d \n", assignNode->getLine());
    }
    if (assignNode->getExp1()->getType()!=assignNode->getExp2()->getType())
    {
        fprintf(stderr, "[SEMANTIC ERROR - assignNode] CANNOT ASSIGN TO DIFFERENT TYPE, line: %d \n", assignNode->getLine());
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
        fprintf(stderr, "[SEMANTIC ERROR - booleanOPNode] LOGICAL OP TYPE, line: %d \n", booleanOPNode->getLine());
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
            fprintf(stderr, "[SEMANTIC ERROR - nameExpNode] UNEXPECTED ARRAY, line: %d \n", nameExpNode->getLine());
        }

        if (nameExpNode->getExp()->isPointer())
        {
            fprintf(stderr, "[SEMANTIC ERROR - nameExpNode] UNEXPECTED POINTER, line: %d \n", nameExpNode->getExp()->getLine());// nameExpNode->exp->line
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
                    fprintf(stderr, "[SEMANTIC ERROR - nameExpNode] INVALID FIELD, line: %d \n", nameExpNode->getExp()->getLine());//nameExpNode->exp->line
                }
            }
            else
            {
                fprintf(stderr, "[SEMANTIC ERROR - nameExpNode] UNKOWN TYPE, line: %d \n", nameExpNode->getExp()->getLine());//nameExpNode->exp->line
            }
        }
        else
        {
            fprintf(stderr, "[SEMANTIC ERROR - nameExpNode] STRUCT EXPECTED, line: %d \n", nameExpNode->getExp()->getLine());//nameExpNode->exp->line
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
        fprintf(stderr, "[SEMANTIC ERROR - notNode] LOGICAL OP TYPE, line: %d \n", notNode->getLine());
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
        fprintf(stderr, "[SEMANTIC ERROR - multiplicationOPNode] ARITHMETIC OP TYPE, line: %d \n", multiplicationOPNode->getLine());
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
                    fprintf(stderr, "[SEMANTIC ERROR - pointerExpNode] INVALID FIELD, line: %d \n", pointerExpNode->getExp()->getLine());//pointerExpNode->exp->line
                }
            }
            else
            {
                fprintf(stderr, "[SEMANTIC ERROR - pointerExpNode] UNKOWN TYPE, line: %d \n", pointerExpNode->getExp()->getLine());//pointerExpNode->exp->line
            }
        }
        else
        {
            if (pointerExpNode->getExp()->isPointer())
            {
                fprintf(stderr, "[SEMANTIC ERROR - pointerExpNode] POINTER EXPECTED, line: %d \n", pointerExpNode->getExp()->getLine());//pointerExpNode->exp->line
            }
            if (pointerExpNode->getExp()->getTypeLexeme())
            {
                fprintf(stderr, "[SEMANTIC ERROR - pointerExpNode] STRUCT EXPECTED, line: %d \n", pointerExpNode->getExp()->getLine());//pointerExpNode->exp->line
            }
        }

    }
}

void Semantic::visit(CaseBlockNode *caseBlockNode)
{
    if (caseBlockNode->getNum() && caseBlockNode->getNum()->getToken()!=NUMINT)
    {
        fprintf(stderr, "[SEMANTIC ERROR - caseBlockNode] CASE WITH NON-INTEGER VALUE, line: %d \n", caseBlockNode->getNum()->getLine());//caseBlockNode->num->line
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