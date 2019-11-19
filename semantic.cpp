#include "semantic.h"

//static int level = 0, breakLevel = 0;
//static SymbolTable *scope[2], *types, *functions;
//FunctionTable *activeFunction = NULL;

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
    //delete semanticVisitor;
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

SymbolTable *getTable(int table)
{
    switch (table)
    {
        case STRUCT:
            return structTable;
        case FUNCTION_TABLE_ID:
            return functionTable;
        case VAR_TABLE_ID:
            return varTable;
        default:
            return NULL;
    }
}

//Semantic::Semantic()
//{
//    //ctor
//}
//Semantic::~Semantic(){
//    //dtor
//}

//**********************************************************************
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
//**********************************************************************
void Semantic::visit(VarDeclNode *varDeclNode)
{
//    if (varDeclNode->getType() && varDeclNode->getIdList())
//    {
//        ///If the var type is a struct, verify if this struct was declared
//        if (varDeclNode->getType()->getId()->getToken()==ID)
//        {
//            if (!structTable->cSearch(varDeclNode->getType()->getId()->getLexeme()))
//            {
//                // TODO imprimeErroSemanticoTipoDesc(varDeclNode->type->id->lexeme, varDeclNode->type->line);
//                ///erro semantico type não definido
//            }
//        }
//        ///Percorre as variaveis de um mesmo type
//        IdListNode *ids = varDeclNode->getIdList();
//        if (ids)
//        {
//            varDeclNode->getIdList()->accept(this);
//            int size = 0;
//            while (ids)
//            {
//                VarSymbol *var;
//                if (activeFunction!=NULL)
//                {
//                    var = varTable->searchInScope(ids->getId()->getLexeme(), activeFunction->getLexeme());
//                }
//                else
//                {
//                    var = varTable->cSearch(ids->getId()->getLexeme());
//                }
//                int sizeAux = BYTE_SIZE;
//                if (ids->getArray()!=NULL)
//                {
//                    sizeAux *= atoi(ids->getArray()->getNumInt()->getLexeme());
//                }
//                if (activeFunction!=NULL)
//                {
//                    ids->getId()->setOffset(activeFunction->getLocalSize() + size + sizeAux);
//                }
//                var->setSize(sizeAux);
//                var->setOffset(ids->getId()->getOffset());
//                size += sizeAux;
//                ids = ids->getNext();
//            }
//            if (activeFunction!=NULL)
//            {
//                activeFunction->incrementLocalSize(size);
//            }
//        }
//    }
//    ///Se existir outras declarações, basta percorrer v->next
//    if (varDeclNode->getNext())
//    {
//        varDeclNode->getNext()->accept(this);
//    }

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
    /**/
    if (varDeclNode->getType()!=NULL && varDeclNode->getIdList()!=NULL)
    {///Verify Struct or Class Declaration
        if (varDeclNode->getType()->getId()->getToken()==ID)
        {///SEMANTIC ERROR - TYPE NOT DEFINED
            if (!structTable->cSearch(varDeclNode->getType()->getId()->getLexeme()))
            {
                // TODO print_semantic_error(ERROR_TYPE_NOT_DEFINED, node->get_line());
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
                var = varTable->searchInScope(list_aux->getId()->getLexeme(), activeFunction->getLexeme());
            }
            else
            {
                var = varTable->cSearch(list_aux->getId()->getLexeme());
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
//**********************************************************************
void Semantic::visit(IdListNode *idListNode)
{
//    if(id->next)
//        id->next->accept(this);

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
    if (!varTable->cInsert(typeNode, idListNode->getId()->getLexeme(), idListNode->getId()->isPointer(),
                           idListNode->getId()->getArraySize(), BOOL_FALSE))
    {
        // TODO print_semantic_error(ERROR_VARIABLE_ALREADY_EXISTS, node->get_line());
    }

    if (idListNode->getNext()!=NULL)
    {
        idListNode->getNext()->accept(this);
    }

    /**/
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
//**********************************************************************
void Semantic::visit(TypeDeclNode *typeDeclNode) // TODO
{
//    ///para percorrer na ordem que esta no codigo
//    if (typeDeclNode->getNext())
//    {
//        typeDeclNode->getNext()->accept(this);
//    }
//
//    if (typeDeclNode->getId() && typeDeclNode->getId()->getLexeme())
//    {
//        if (!types->cSearch(typeDeclNode->getId()->getLexeme()))
//        {
//            Indice_Tipo *type = new Indice_Tipo();
//            VarDeclNode *decl = typeDeclNode->getDecl();
//            int size = 0;
//            while (decl)
//            {
//                IdListNode *ids = decl->getIdList();
//
//                if (decl->getType()->getId()->getToken()==ID && !types->cSearch(decl->getType()->getId()->getLexeme()))
//                {
//                    // TODO imprimeErroSemanticoTipoDesc(decl->type->id->lexeme, decl->type->line);
//                    ///erro semantico type não definido
//                }
//
//                while (ids)
//                {
//                    if (ids->getId() && ids->getId()->getToken()==ID)
//                    {
////                        if (!type->variaveis->cSearch(ids->getId()->getLexeme()))
//                        if (!type->variaveis->cSearch(ids->getId()->getLexeme()))
//                        {
//                            //VarSymbol *campo = new VarSymbol();
//                            VarSymbol *campo = varTable->searchInScope(ids->getId()->getLexeme(), typeDeclNode->getId()->getLexeme());
//                            campo->setType(decl->getType());
//                            int sizeAux = typeSize(campo->getType());
//
//                            if (ids->getPointer())
//                            {
//                                campo->setPointer(true);
//                                ///tamanho de pointer é fixo igual a de um inteiro
//                                sizeAux = POINTER_BYTE_SIZE;
//                            }
//                            else
//                            {
//                                campo->setPointer(false);
//                            }
//
//                            if (ids->getArray() && ids->getArray()->getNumInt() && ids->getArray()->getNumInt()->getToken()==NUMINT)
//                            {
//                                campo->setArraySize(atoi(ids->getArray()->getNumInt()->getLexeme()));
//                                /// tamanho do array é o tamnho vezes o tamanho do type basico
//                                sizeAux = sizeAux*campo->getArraySize();
//                            }
//
//                            size += sizeAux;
//                            type->variaveis->cInsert(campo, ids->getId()->getLexeme());
//                        }
//                        else
//                        {
//                            // TODO imprimeErroSemanticoCampo(ids->id->lexeme, decl->type->line);
//                        }
//                    }
//                    ids = ids->getNext();
//                }
//                decl = decl->getNext();
//            }
//            type->tam = size;
//            types->cInsert(type, typeDeclNode->getId()->getLexeme());
//        }
//        else
//        {
//            // TODO imprimeErroSemanticoDeclara(typeDeclNode->id->lexeme,"Tipo",typeDeclNode->line);
//        }
//    }

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
        // TODO print_semantic_error(ERROR_STRUCT_ALREADY_EXISTS, typeDeclNode->get_line());
    }

    /**/
    if (typeDeclNode->getNext()!=NULL)
    {
        typeDeclNode->getNext()->accept(this);
    }

    //const char *typeLex = typeDeclNode->getId()!=NULL ? typeDeclNode->getId()->getLexeme() : NULL;
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
                VarSymbol *var = varTable->searchInScope(id_list_aux->getId()->getLexeme(),
                                                         typeDeclNode->getId()->getLexeme());
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
//**********************************************************************
void Semantic::visit(TokenNode *tokenNode)
{
    //TODO
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
//**********************************************************************
void Semantic::visit(TypeNode *typeNode)
{
}
//**********************************************************************
void Semantic::visit(FormalListNode *formalListNode)
{
//    if (formalListNode->getType() && formalListNode->getType()->getId() && formalListNode->getId())
//    {
//        ///Declara os parâmetros de uma função no escopo atual
//        if (formalListNode->getType()->getId()->getToken()==ID)
//        {
//            if (!varTable->cSearch(formalListNode->getType()->getId()->getLexeme()))
//            {
//                // TODO imprimeErroSemanticoTipoDesc(formalListNode->type->id->lexeme, formalListNode->type->line);
//                ///erro semantico type não definido
//            }
//            if (!formalListNode->getPointer())
//            {
//                // TODO imprimeErroSemantico("Ponteiro esperado", formalListNode->type->line);
//            }
//        }
////        VarSymbol *varSymbol = new VarSymbol();
//        VarSymbol *varSymbol = varTable->searchInScope(formalListNode->getId()->getLexeme(), activeFunction->getLexeme());
////        varSymbol->setType(formalListNode->getType());
//        int size = BYTE_SIZE;
////        varSymbol->setPointer(formalListNode->getPointer()!=NULL);
//        if (varSymbol->isPointer())
//        {
//            ///pointer tem tamanho fixo de 4 bytes
//            size = POINTER_SIZE;
//        }
//        if (formalListNode->getArray() && formalListNode->getArray()->getNumInt() && formalListNode->getArray()->getNumInt()->getToken()==NUMINT)
//        {
//            varSymbol->setArraySize(atoi(formalListNode->getArray()->getNumInt()->getLexeme()));
//            ///vetor é passado por referencia so um pointer
//            size = POINTER_SIZE;
//        }
//        activeFunction->tamparam += size;
//        Declare(varSymbol, formalListNode->getId()->getLexeme(), formalListNode->getType()->line);
//    }
//    if (formalListNode->getNext())
//    {
//        formalListNode->getNext()->accept(this);
//    }

    //*****************************************************************************************************************
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
        // TODO print_semantic_error(ERROR_VARIABLE_ALREADY_EXISTS, formalListNode->getLine());
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
//**********************************************************************
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
//**********************************************************************
void Semantic::visit(CallNode *callNode)
{
//    if (callNode->getId())
//    {
//        if (callNode->getId()->getToken()==ID)
//        {
//            FunctionSymbol *funcSymbol = functionTable->cSearch(callNode->getId()->getLexeme());
//            if (functionTable)
//            {
//                ///calcular maior chamada de função
//                if (functionTable->tamparam > activeFunction->tammaiorchamada)
//                {
//                    activeFunction->tammaiorchamada = functionTable->tamparam;
//                }
//
//                callNode->pointer = functionTable->pointer;
//                callNode->array = 0;
//                ///Recupera o type de returnAtt da função e associa esse type ao 'call'
//                if (functionTable->returnType->id->TOKEN==ID)
//                {
//                    callNode->typeName = functionTable->returnType->id->lexeme;
//                    callNode->type = typeDef;
//                }
//                else if (functionTable->returnType->id->TOKEN==INT)
//                {
//                    callNode->typeName = NULL;
//                    callNode->type = numInt;
//                }
//                else if (functionTable->returnType->id->TOKEN==FLOAT)
//                {
//                    callNode->typeName = NULL;
//                    callNode->type = numFloat;
//                }
//                else if (functionTable->returnType->id->TOKEN==CHAR)
//                {
//                    callNode->typeName = NULL;
//                    callNode->type = character;
//                }
//                else if (functionTable->returnType->id->TOKEN==BOOL)
//                {
//                    callNode->typeName = NULL;
//                    callNode->type = boolean;
//                }
//                ///Verifica, na sequência, se os parâmetros passados e os esperados são compatíveis
//                if (callNode->getParameters())
//                {
//                    ExpListNode *list = callNode->getParameters();
//                    ExpNode *exp = list->getExp();
//                    FormalListNode *parameters = functionTable->parameters;
//                    int numParam = 1;
//                    do
//                    {
//                        if (exp && !parameters)
//                        {
//                            // TODO imprimeErroSemanticoChamada(callNode->id->lexeme, "muitos", callNode->id->line);
//                        }
//                        else if (!exp && parameters)
//                        {
//                            // TODO imprimeErroSemanticoChamada(callNode->id->lexeme, "poucos", callNode->id->line);
//                        }
//                        else
//                        {
//                            exp->accept(this);
//                            parameterTypeTest(exp, parameters, callNode->getId()->getLexeme(), numParam);
//                        }
//                        list = list->getNext();
//                        if (list)
//                        {
//                            exp = list->getExp();
//                        }
//                        else
//                        {
//                            exp = NULL;
//                        }
//                        parameters = parameters->getNext();
//                        if (exp && !parameters)
//                        {
//                            // TODO imprimeErroSemanticoChamada(callNode->id->lexeme, "muitos", callNode->id->line);
//                        }
//                        else if (!exp && parameters)
//                        {
//                            // TODO imprimeErroSemanticoChamada(callNode->id->lexeme, "poucos", callNode->id->line);
//                        }
//                        numParam++;
//                    }
//                    while (exp && parameters);
//                }
//                else if (functionTable->parameters)
//                {
//                    // TODO imprimeErroSemantico("parameters esperados", callNode->id->line);
//                }
//            }
//            else
//            {
//                // TODO imprimeErroSemanticoNaoDeclara(callNode->id->lexeme, "Funcao", callNode->id->line);
//            }
//        }
//    }
    FunctionSymbol *funcSymbol = functionTable->cSearch(callNode->getId()->getLexeme());
    if (funcSymbol!=NULL)
    {
        if (callNode->getId()!=NULL)
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
                    // TODO print_semantic_error(ERROR_PARAMETER_TYPE_MISMATCH, callNode->getLine());
                    exp = exp->getNext();
                    param = param->getNext();
                    break;
                }
                exp = exp->getNext();
                param = param->getNext();
            }
            if (exp==NULL && param!=NULL)
            {
                // TODO print_semantic_error(ERROR_TOO_FEW_ARGUMENTS, callNode->getLine());
            }
            else if (exp!=NULL && param==NULL)
            {
                // TODO print_semantic_error(ERROR_TOO_MANY_ARGUMENTS, callNode->getLine());
            }
        }
        if (callNode->getParameters()->getNext()!=NULL)
        {
            callNode->getParameters()->getNext()->accept(this);
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
        // TODO print_semantic_error(ERROR_FUNCTION_NOT_DECLARED, callNode->getLine());
    }
}
//**********************************************************************
void Semantic::visit(PrimaryNode *primaryNode)
{
//    if (primaryNode->getExp()==NULL && primaryNode->getToken() && primaryNode->getToken()->getToken()==ID)
//    {
//        ///Se for uma variável, recupera suas informações para serem propagadas na árvore
//        primaryNode->lval = true;
//        VarSymbol *decl = getDecl(primaryNode->getToken()->getLexeme());
//        if (decl)
//        {
//            primaryNode->pointer = decl->pointer;
//            if (decl->array)
//            {
//                primaryNode->array = decl->array;
//            }
//            if (decl->type->id->TOKEN==ID)
//            {
//                primaryNode->typeName = decl->type->id->lexeme;
//                primaryNode->type = typeDef;
//            }
//            else if (decl->type->id->TOKEN==INT)
//            {
//                primaryNode->typeName = NULL;
//                primaryNode->type = numInt;
//            }
//            else if (decl->type->id->TOKEN==FLOAT)
//            {
//                primaryNode->typeName = NULL;
//                primaryNode->type = numFloat;
//            }
//            else if (decl->type->id->TOKEN==CHAR)
//            {
//                primaryNode->typeName = NULL;
//                primaryNode->type = character;
//            }
//            else if (decl->type->id->TOKEN==BOOL)
//            {
//                primaryNode->typeName = NULL;
//                primaryNode->type = boolean;
//            }
//        }
//        else
//        {
//            // TODO imprimeErroSemanticoNaoDeclara(primaryNode->token->lexeme , "Variavel", primaryNode->token->line);
//        }
//    }
//    else
//    {
//        ///Se for uma expressão, calcula-se o type da expressão
//        if (primaryNode->getExp())
//        {
//            primaryNode->lval = primaryNode->getExp()->lval;
//            primaryNode->getExp()->accept(this);
//            primaryNode->type = primaryNode->getExp()->type;
//            primaryNode->typeName = primaryNode->getExp()->typeName;
//        }
//        primaryNode->lval = false;
//        ///Se for um valor direto, obtém-se o seu type
//        if (primaryNode->getToken())
//        {
//            if (primaryNode->getToken()->getToken()==NUMINT)
//            {
//                primaryNode->type = numInt;
//            }
//            if (primaryNode->getToken()->getToken()==NUMFLOAT)
//            {
//                primaryNode->type = numFloat;
//            }
//            if (primaryNode->getToken()->getToken()==LITERAL)
//            {
//                primaryNode->type = literal;
//            }
//            if (primaryNode->getToken()->getToken()==TRUE || primaryNode->getToken()->getToken()==FALSE)
//            {
//                primaryNode->type = boolean;
//            }
//        }
//    }

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

                    if (var_symbol->getType()->getId()->getToken()!=INT
                        && var_symbol->getType()->getId()->getToken()!=FLOAT
                        && var_symbol->getType()->getId()->getToken()!=CHAR
                        && var_symbol->getType()->getId()->getToken()!=LITERAL
                        && var_symbol->getType()->getId()->getToken()!=BOOL)
                    {
                        // TODO print_semantic_error(ERROR_TYPE_NOT_DEFINED, primaryNode->get_line());
                    }
                    else
                    {
                        primaryNode->setType(primaryNode->getExp()->getType());
                        primaryNode->setTypeLexeme(primaryNode->getExp()->getTypeLexeme());
                    }
                }
                else
                {
                    // TODO print_semantic_error(ERROR_VARIABLE_NOT_DECLARED, primaryNode->get_line());
                }

            }
        }
//        else if (typeid(*primaryNode->getExp())==typeid(IntegerNode) || typeid(*primaryNode->getExp())==typeid(RealNode) ||
//            typeid(*primaryNode->getExp())==typeid(CharacterNode) || typeid(*primaryNode->getExp())==typeid(LiteralNode) ||
//            typeid(*primaryNode->getExp())==typeid(TrueNode) || typeid(*primaryNode->getExp())==typeid(FalseNode))
        else if (primaryNode->getToken()->getToken()==NUMINT || primaryNode->getToken()->getToken()==NUMFLOAT ||
            primaryNode->getToken()->getToken()==LITERALCHAR || primaryNode->getToken()->getToken()==LITERAL ||
            primaryNode->getToken()->getToken()==TRUE || primaryNode->getToken()->getToken()==FALSE)
        {
            if (primaryNode->getExp()!=NULL)
            {
                // TODO print_semantic_error(ERROR_UNEXPECTED_ACCESS_IN_NOT_ID_TYPE, primaryNode->get_line());
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
        primaryNode->getToken()->setLexeme(primaryNode->getToken()->getLexeme());
    }
}
//**********************************************************************
void Semantic::visit(FunctionNode *functionNode)// TODO
{
    if (functionNode->getType() && functionNode->getType()->getId() && functionNode->getId())
    {
        ///Se o retorno da função for de um type definido, verifica-se se esse type já foi declarado
        if (functionNode->getType()->getId()->getToken()==ID)
        {
            if (!varTable->cSearch(functionNode->getId()->getLexeme()))// VARTABLE OR FUNCTION TABLE?
            {
                // TODO imprimeErroSemanticoTipoDesc(functionNode->type->id->lexeme, functionNode->type->line);
                ///erro semantico type não definido
            }
        }
        ///Declara a função
        FunctionSymbol *decl = functionTable->cSearch(functionNode->getId()->getLexeme());
        ///Salva a função ativa
        activeFunction = decl;
        // TODO functionDeclaration(decl, functionNode->getId()->getLexeme(), functionNode->getType()->line);
    }
    // TODO BeginScope();?
    if (functionNode->getParameters())
    {
        functionNode->getParameters()->accept(this);
    }
    if (functionNode->getLocal())
    {
        functionNode->getLocal()->accept(this);
    }
    if (functionNode->getBody())
    {
        functionNode->getBody()->accept(this);
        if (functionNode->getType())
        {
            if (!functionNode->getBody()->isReturn())
            {
                // TODO imprimeAvisoSemantico(functions->getTexto( activeFunction->pegaIndiceLexema()));
            }
        }
    }
    else if (functionNode->getType())
    {
        // TODO imprimeAvisoSemantico(functions->getTexto( activeFunction->pegaIndiceLexema()));
    }
    // TODO EndScope();
    //fprintf(stdout,"Funcao %s tem  %d bytes de parameters e %d bytes de var locais %d maior chamada de funcao\n",functions->getTexto( activeFunction->pegaIndiceLexema()), activeFunction->tamparam,activeFunction->tamloc, activeFunction->tammaiorchamada);
    activeFunction = NULL;
}
//**********************************************************************
void Semantic::visit(StmtNode *stmtNode)// TODO
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
//**********************************************************************
void Semantic::visit(StmtListNode *stmtListNode)
{
    if (stmtListNode->getStmt())
    {
        stmtListNode->getStmt()->accept(this);
        //  TODO stmtListNode->returnAtt = stmtListNode->getStmt()->returnAtt;
    }
    if (stmtListNode->getNext())
    {
        stmtListNode->getNext()->accept(this);
        // TODO stmtListNode->returnAtt = stmtListNode->returnAtt || stmtListNode->getNext()->returnAtt;
    }
}
//**********************************************************************
void Semantic::visit(ReadLnNode *read)
{
    if (read->getExp())
    {
        read->getExp()->accept(this);
        if (!read->getExp()->isLValue())
        {
            // TODO imprimeErroSemantico("Lval esperado no Read", read->exp->line);
        }
    }
}
//**********************************************************************
void Semantic::visit(PrintNode *print)
{
    if (print->getExpList())
    {
        print->getExpList()->accept(this);
    }
}
//**********************************************************************
void Semantic::visit(WhileNode *whileNode)
{
    if (whileNode->getHead()!=NULL)
    {
        whileNode->getHead()->accept(this);
        if (whileNode->getHead()->getType()!=BOOL)
        {
            // TODO print_semantic_errVOOor(ERROR_BOOLEAN_EXPRESSION_REQUIRED, node->get_line());
        }
    }
    breakEnabled = BOOL_TRUE;
    if (whileNode->getBody()!=NULL)
    {
        whileNode->getBody()->accept(this);
    }
    breakEnabled = BOOL_FALSE;
}
//**********************************************************************
void Semantic::visit(ReturnNode *returnNode)
{
//    if (returnNode->getExp())
//    {
//        returnNode->getExp()->accept(this);
//        returnNode->pointer = activeFunction->pointer;
//        returnNode->array = 0;
//        ///Recupera o valor de returnAtt esperado da função ativa
//        if (activeFunction->returnType->id->TOKEN==ID)
//        {
//            returnNode->typeName = activeFunction->returnType->id->lexeme;
//            returnNode->type = typeDef;
//        }
//        else if (activeFunction->returnType->id->TOKEN==INT)
//        {
//            returnNode->typeName = NULL;
//            returnNode->type = numInt;
//        }
//        else if (activeFunction->returnType->id->TOKEN==FLOAT)
//        {
//            returnNode->typeName = NULL;
//            returnNode->type = numFloat;
//        }
//        else if (activeFunction->returnType->id->TOKEN==CHAR)
//        {
//            returnNode->typeName = NULL;
//            returnNode->type = character;
//        }
//        else if (activeFunction->returnType->id->TOKEN==BOOL)
//        {
//            returnNode->typeName = NULL;
//            returnNode->type = boolean;
//        }
//        ///Compara o type do valor retornado e o type do returnAtt esperado pela função ativa
//        typeTest(returnNode, returnNode->getExp());
//    }
    if (returnNode->getExp()!=NULL)
    {
        returnNode->getExp()->accept(this);
    }
}
//**********************************************************************
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
//**********************************************************************
void Semantic::visit(ThrowNode *throwNode)
{
}
//**********************************************************************
void Semantic::visit(BreakNode *breakNode)
{
    if (breakEnabled!=BOOL_TRUE)
    {
        // TODO print_semantic_error(ERROR_UNEXPECTED_BREAK, node->get_line());
    }
}
//**********************************************************************
void Semantic::visit(IfNode *ifNode)
{
    if (ifNode->getHead()!=NULL)
    {
        ifNode->getHead()->accept(this);
        if (ifNode->getHead()->getType()!=BOOL)
        {
            // TODO print_semantic_error(ERROR_BOOLEAN_EXPRESSION_REQUIRED, node->get_line());
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
//**********************************************************************
void Semantic::visit(SwitchNode *switchNode)
{
    if (switchNode->getExp()!=NULL)
    {
        switchNode->getExp()->accept(this);
        if (switchNode->getExp()->getType()!=INT)
        {
            // TODO print_semantic_error(ERROR_INT_VALUE_REQUIRED, node->get_line());
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
        // TODO print_semantic_error(ERROR_CASE_STATEMENT_REQUIRED, node->get_line());
    }
}
//**********************************************************************
void Semantic::visit(AdditionOPNode *additionOPNode)
{
//    additionOPNode->lval = false;
//    if (additionOPNode->getExp1())
//    {
//        additionOPNode->getExp1()->accept(this);
//    }
//    if (additionOPNode->getExp2())
//    {
//        additionOPNode->getExp2()->accept(this);
//    }
//    if (additionOPNode->getExp1() && additionOPNode->getExp2())
//    {
//        arrayTest(additionOPNode->getExp1());
//        arrayTest(additionOPNode->getExp2());
//        if (additionOPNode->getExp1()->type==numInt || additionOPNode->getExp1()->type==numFloat)
//        {
//            if (typeTest(additionOPNode->getExp1(), additionOPNode->getExp2()))
//            {
//                additionOPNode->type = MAX(additionOPNode->getExp1()->type, additionOPNode->getExp2()->type);
//                additionOPNode->typeName = additionOPNode->getExp1()->typeName;
//            }
//        }
//        else
//        {
//            // TODO imprimeErroSemantico("Tipo numerico esperado", additionOPNode->exp2->line);
//        }
//    }

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
        // TODO print_semantic_error(ERROR_ARITHMETIC_OP_TYPE, node->get_line());
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
//**********************************************************************
void Semantic::visit(AddressValNode *addressValNode)// TODO
{
//    if (addressValNode->getExp())
//    {
//        addressValNode->getExp()->accept(this);
//        arrayTest(addressValNode->getExp());
//        if (addressValNode->getExp()->pointer)
//        {
//            // TODO imprimeErroSemantico("Ponteiro inesperado", addressValNode->exp->line);
//        }
//        if (!addressValNode->getExp()->lval)
//        {
//            // TODO imprimeErroSemantico("Lval Esperado no adressVal", addressValNode->exp->line);
//        }
//        addressValNode->array = addressValNode->getExp()->array;
//        addressValNode->pointer = true;
//        addressValNode->type = addressValNode->getExp()->type;
//        addressValNode->lval = false;
//        addressValNode->typeName = addressValNode->getExp()->typeName;
//    }
    if (addressValNode->getExp()!=NULL)
    {
        addressValNode->getExp()->accept(this);
    }
}
//**********************************************************************
void Semantic::visit(AssignNode *assignNode)
{
//    if (assignNode->getExp1())
//    {
//        assignNode->getExp1()->accept(this);
//        if (!assignNode->getExp1()->lval)
//        {/// erro lval esperado
//            // TODO imprimeErroSemantico("LVAL esperado", assignNode->exp1->line);
//            assignNode->lval = false;
//        }
//        else
//        {
//            arrayTest(assignNode->getExp2());
//            assignNode->lval = true;
//        }
//    }
//
//    if (assignNode->getExp2())
//    {
//        assignNode->getExp2()->accept(this);
//    }
//    if (assignNode->getExp2() && assignNode->getExp1())
//    {
//        arrayTest(assignNode->getExp1());
//        arrayTest(assignNode->getExp2());
//        if (typeTest(assignNode->getExp2(), assignNode->getExp1()))
//        {
//            assignNode->type = assignNode->getExp1()->type;
//            assignNode->typeName = assignNode->getExp1()->typeName;
//        }
//    }
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
        // TODO print_semantic_error(ERROR_LVALUE_REQUIRED_AS_LEFT_OPERAND_OF_ASSIGNMENT, node->get_line());
    }
    if (assignNode->getExp1()->getType()!=assignNode->getExp2()->getType())
    {
        // TODO print_semantic_error(ERROR_CANNOT_ASSIGN_TO_DIFFERENT_TYPE, node->get_line());
    }
}
//**********************************************************************
void Semantic::visit(BooleanOPNode *booleanOPNode)
{
//    booleanOPNode->lval = false;
//    if (booleanOPNode->getExp1())
//    {
//        booleanOPNode->getExp1()->accept(this);
//        if (booleanOPNode->getOp() && (booleanOPNode->getOp()->getToken()==AND || booleanOPNode->getOp()->getToken()==OR) && booleanOPNode->getExp1()->type!=boolean)
//        {
//            // TODO imprimeErroSemanticoTipo(boolean, NULL, booleanOPNode->exp1->type, NULL, booleanOPNode->exp1->line);
//        }
//        arrayTest(booleanOPNode->getExp1());
//    }
//    if (booleanOPNode->getExp2())
//    {
//        booleanOPNode->getExp2()->accept(this);
//        if (booleanOPNode->getOp() && (booleanOPNode->getOp()->getToken()==AND || booleanOPNode->getOp()->getToken()==OR) && booleanOPNode->getExp2()->type!=boolean)
//        {
//            // TODO imprimeErroSemanticoTipo(boolean, NULL, booleanOPNode->exp2->type, NULL, booleanOPNode->exp2->line);
//        }
//        arrayTest(booleanOPNode->getExp2());
//    }
//    typeTest(booleanOPNode->getExp1(), booleanOPNode->getExp2());
//    booleanOPNode->type = boolean;
//    booleanOPNode->typeName = NULL;

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
        // TODO print_semantic_error(ERROR_LOGICAL_OP_TYPE, node->get_line());
    }
    booleanOPNode->setType(BOOL);
    booleanOPNode->setTypeLexeme(token_id_to_name(BOOL));
}
//**********************************************************************
void Semantic::visit(NameExpNode *nameExpNode)// TODO
{
    nameExpNode->setLValue(true);
    if (nameExpNode->getExp())
    {
        nameExpNode->getExp()->accept(this);
//        arrayTest(nameExpNode->getExp());
        if (nameExpNode->getExp()->getArraySize())
        {
            // TODO imprimeErroSemantico("Array inesperado", exp->line);
        }

        if (nameExpNode->getExp()->isPointer())
            // TODO imprimeErroSemantico("Ponteiro nao esperado", nameExpNode->exp->line);

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
                        // TODO imprimeErroSemantico("Campo Invalido", nameExpNode->exp->line);
                    }
                }
                else
                {
                    // TODO imprimeErroSemantico("Tipo desconhecido", nameExpNode->exp->line);
                }
            }
            else
            {
                // TODO imprimeErroSemantico("Struct esperada", nameExpNode->exp->line);
            }
    }
}
//**********************************************************************
void Semantic::visit(NotNode *notNode)
{
//    notNode->lval = false;
//    if (notNode->getExp())
//    {
//        notNode->getExp()->accept(this);
//        notNode->type = notNode->getExp()->type;
//        notNode->typeName = notNode->getExp()->typeName;
//        notNode->pointer = notNode->getExp()->pointer;
//        arrayTest(notNode->getExp());
//        if (notNode->getExp()->pointer)
//        {
//            // TODO imprimeErroSemantico("Ponteiro nao esperado", notNode->exp->line);
//            if (notNode->type!=boolean)
//            {
//                // TODO imprimeErroSemantico("Tipo booleano esperado", notNode->exp->line);
//            }
//        }
//    }

    if (notNode->getExp()!=NULL)
    {
        notNode->getExp()->accept(this);
    }
    if (notNode->getExp()->getType()!=BOOL)
    {
        // TODO print_semantic_error(ERROR_LOGICAL_OP_TYPE, node->get_line());
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
//**********************************************************************
void Semantic::visit(PointerValNode *pointerValNode)
{
//    pointerValNode->lval = true;
//    if (pointerValNode->getExp())
//    {
//        pointerValNode->getExp()->accept(this);
//        arrayTest(pointerValNode->getExp());
//        pointerValNode->type = pointerValNode->getExp()->type;
//        pointerValNode->typeName = pointerValNode->getExp()->typeName;
//        pointerValNode->pointer = false;
//        if (!pointerValNode->getExp()->pointer)
//        {
//            // TODO imprimeErroSemantico("Ponteiro esperado", pointerValNode->exp->line);
//        }
//    }
    pointerValNode->setLValue(BOOL_TRUE);
    if (pointerValNode->getExp()!=NULL)
    {
        pointerValNode->getExp()->accept(this);
    }
}
//**********************************************************************
void Semantic::visit(MultiplicationOPNode *multiplicationOPNode)
{
//    multiplicationOPNode->lval = false;
//    if (multiplicationOPNode->getExp1())
//    {
//        multiplicationOPNode->getExp1()->accept(this);
//    }
//    if (multiplicationOPNode->getExp2())
//    {
//        multiplicationOPNode->getExp2()->accept(this);
//    }
//    if (multiplicationOPNode->getExp2() && multiplicationOPNode->getExp1())
//    {
//        arrayTest(multiplicationOPNode->getExp1());
//        arrayTest(multiplicationOPNode->getExp2());
//        if (multiplicationOPNode->getExp1()->type==numInt || multiplicationOPNode->getExp1()->type==numFloat)
//        {
//            if (typeTest(multiplicationOPNode->getExp1(), multiplicationOPNode->getExp2()))
//            {
//                //pegar numFloat se algum for numFloat
//                multiplicationOPNode->type = MAX(multiplicationOPNode->getExp1()->type, multiplicationOPNode->getExp2()->type);
//                multiplicationOPNode->typeName = multiplicationOPNode->getExp1()->typeName;
//            }
//        }
//        else
//        {
//            // TODO imprimeErroSemantico("Tipo numerico esperado", multiplicationOPNode->exp1->line);
//        }
//    }

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
        // TODO print_semantic_error(ERROR_ARITHMETIC_OP_TYPE, multiplicationOPNode->get_line());
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
//**********************************************************************
void Semantic::visit(SignNode *signNode)
{
//    signNode->lval = false;
//    if (signNode->getExp())
//    {
//        signNode->getExp()->accept(this);
//        signNode->type = signNode->getExp()->type;
//        signNode->typeName = signNode->getExp()->typeName;
//        arrayTest(signNode->getExp());
//        if (signNode->getExp()->pointer)
//        {
//            // TODO imprimeErroSemantico("Ponteiro nao esperado", signNode->exp->line);
//        }
//        if ((signNode->type!=numInt && signNode->type!=numFloat) || signNode->pointer)
//        {
//            // TODO imprimeErroSemantico("Tipo numerico esperado", signNode->exp->line);
//        }
//    }
    if (signNode->getExp()!=NULL)
    {
        signNode->getExp()->accept(this);
        signNode->setType(signNode->getExp()->getType());
        signNode->setTypeLexeme(signNode->getExp()->getTypeLexeme());
    }
}
//**********************************************************************
void Semantic::visit(ArrayCallNode *arrayCallNode)// TODO
{
//    arrayCallNode->lval = true;
//    if (arrayCallNode->getExp())
//    {
//        arrayCallNode->getExp()->accept(this);
//        if (arrayCallNode->getExp()->array)
//        {
//            arrayCallNode->type = arrayCallNode->getExp()->type;
//            arrayCallNode->typeName = arrayCallNode->getExp()->typeName;
//            arrayCallNode->pointer = arrayCallNode->getExp()->pointer;
//            ///deixou de ser array
//            arrayCallNode->array = 0;
//        }
//        else
//        {
//            // TODO imprimeErroSemantico("Array esperado", arrayCallNode->exp->line);
//        }
//    }
//    if (arrayCallNode->getIndex())
//    {
//        arrayCallNode->getIndex()->accept(this);
//        if (arrayCallNode->getIndex()->type!=numInt)
//        {
//            // TODO imprimeErroSemantico("Numero Inteiro esperado como indice do vetor", arrayCallNode->indice->line);
//        }
//        else
//        {
//            if (typeid(*arrayCallNode->getIndex())==typeid(PrimaryNode))
//            {
//                PrimaryNode *indice = (PrimaryNode *) arrayCallNode->getIndex();
//                if (indice->getToken()->getToken()==NUMINT && arrayCallNode->getExp() && arrayCallNode->getExp()->array)
//                {
//                    if (atoi(indice->getToken()->getLexeme()) >= arrayCallNode->getExp()->array || atoi(indice->getToken()->getLexeme()) < 0)
//                    {
//                        // TODO imprimeErroSemantico("Indice do vetor esta fora do limite permitido", arrayCallNode->indice->line);
//                    }
//                }
//            }
//        }
//    }

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
//**********************************************************************
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
//**********************************************************************
void Semantic::visit(PointerExpNode *pointerExpNode)// TODO
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
                    // TODO imprimeErroSemantico("Campo Invalido", pointerExpNode->exp->line);
                }
            }
            else
            {
                // TODO imprimeErroSemantico("Tipo desconhecido", pointerExpNode->exp->line);
            }
        }
        else
        {
            if (pointerExpNode->getExp()->isPointer())
            {
                // TODO imprimeErroSemantico("Ponteiro esperado", pointerExpNode->exp->line);
            }
            if (pointerExpNode->getExp()->getTypeLexeme())
            {
                // TODO imprimeErroSemantico("Struct esperada", pointerExpNode->exp->line);
            }
        }

    }
}
//**********************************************************************
void Semantic::visit(CaseBlockNode *caseBlockNode)
{
    if (caseBlockNode->getNum() && caseBlockNode->getNum()->getToken()!=NUMINT)
    {
        // TODO imprimeErroSemantico("Case com valor nao inteiro\n", caseBlockNode->num->line);
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



//static bool typeTest(ExpNode *exp1, ExpNode *exp2)
//{
//    if (exp1->pointer!=exp2->pointer)
//    {
//        if (exp1->pointer)
//        {
//            // TODO imprimeErroSemantico("Esperado um Ponteiro", exp1->line);
//        }
//        else
//        {
//            // TODO imprimeErroSemantico("Ponteiro nao esperado", exp1->line);
//        }
//        return false;
//    }
//
//    if (exp1->type==numInt || exp1->type==numFloat)
//    {
//        if (exp2->type!=numInt && exp2->type!=numFloat)
//        {
//            // TODO imprimeErroSemanticoTipo(exp2->type, exp2->typeName, exp1->type, exp1->typeName, exp2->line);
//            return false;
//        }
//    }
//    else
//    {/// caso não seja interio ou real os tipos devem ser iguais
//        if (exp1->type!=exp2->type)
//        {
//            // TODO imprimeErroSemanticoTipo(exp2->type, exp2->typeName, exp1->type, exp1->typeName, exp2->line);
//            return false;
//        }
//        /// se for type definido o texto tem q ser o mesmo
//        if (exp1->type==typeDef && strcmp(exp1->typeName, exp2->typeName)!=0)
//        {
//            // TODO imprimeErroSemanticoTipo(exp2->type, exp2->typeName, exp1->type, exp1->typeName, exp2->line);
//            return false;
//        }
//    }
//
//    return true;
//
//}
//
//static void parameterTypeTest(ExpNode *exp, FormalListNode *parameter, const char *funcName, int numParam)
//{
//    if ((exp->pointer && parameter->getPointer()) || (!exp->pointer && !parameter->getPointer()))
//    {
//        int paramType = 0;
//        switch (parameter->getType()->getId()->getToken())
//        {
//            case INT:
//                paramType = numInt;
//                break;
//            case FLOAT:
//                paramType = numFloat;
//                break;
//            case ID:
//                paramType = typeDef;
//                break;
//            case CHAR:
//                paramType = character;
//                break;
//            case BOOL:
//                paramType = boolean;
//                break;
//        }
//        /// aceitar real quando parametro for inteiro e inteiro quando for real
//        if (paramType==numInt || paramType==numFloat)
//        {
//            if (exp->type!=numInt && exp->type!=numFloat)
//            {
//                // TODO imprimeErroSemanticoParam(funcName, numParam, paramType, parameter->type->id->lexeme, exp->type, exp->typeName, exp->line);
//            }
//        }
//        else if (exp->type!=paramType)
//        {///caso contrario somente se os tipos forem iguais
//            // TODO imprimeErroSemanticoParam(funcName, numParam, paramType, parameter->type->id->lexeme, exp->type, exp->typeName, exp->line);
//        }
//        else if (paramType==typeDef)
//        {/// tem de testar se é o mesmo type
//            if (strcmp(exp->typeName, parameter->getType()->getId()->getLexeme())!=0)
//            {
//                // TODO imprimeErroSemanticoParam(funcName, numParam, paramType, parameter->type->id->lexeme, exp->type, exp->typeName, exp->line);
//            }
//        }
//    }
//    else
//    {
//        char error[255];
//
//        if (exp->pointer)
//        {
//            sprintf(error, "Ponteiro nao esperado como %d parametro da funcao %s", numParam, funcName);
//            // TODO imprimeErroSemantico(error, exp->line);
//        }
//        else
//        {
//            sprintf(error, "Ponteiro esperado como %d parametro da funcao %s", numParam, funcName);
//            // TODO imprimeErroSemantico(error, exp->line);
//        }
//    }
//}
//
//static int typeSize(TypeNode *type)
//{
//    Indice_Tipo *aux;
//    switch (type->getId()->getToken())
//    {
//        case INT:
//            return INT_SIZE;
//            break;
//        case FLOAT:
//            return FLOAT_SIZE;
//            break;
//        case CHAR:
//            return CHAR_SIZE;
//            break;
//        case BOOL:
//            return BOOLEAN_SIZE;
//            break;
//        case ID:
//            aux = (Indice_Tipo *) types->cSearch(type->getId()->getLexeme());
//            if (aux)
//            {
//                return aux->tam;
//            }
//            else
//            {
//                return 0;
//            }
//            break;
//    }
//    return 0;
//}
//static void arrayTest(ExpNode *exp)
//{
//    if (exp->array)
//    {
//        // TODO imprimeErroSemantico("Array inesperado", exp->line);
//    }
//}
//
//static void Declare(VarSymbol *decl, char *lexeme, int line)
//{
//    if (scope[level - 1]->cSearch(lexeme))
//    {
//        // TODO imprimeErroSemanticoDeclara(lexeme, "Variavel", line);
//    }
//    else if (functions->cSearch(lexeme))
//    {
//        // TODO imprimeErroSemanticoExiste(lexeme, "uma funcao", line);
//    }
//    else if (types->cSearch(lexeme))
//    {
//        // TODO imprimeErroSemanticoExiste(lexeme, "um type", line);
//    }
//    else
//    {
//        scope[level - 1]->cInsert(decl, lexeme);
//    }
//}
//
//static void functionDeclaration(FunctionSymbol *decl, char *lexeme, int line)
//{
//    if (functions->cSearch(lexeme))
//    {
//        // TODO imprimeErroSemanticoDeclara(lexeme, "Funcao", line);
//    }
//    else if (scope[level - 1]->cSearch(lexeme))
//    {
//        // TODO imprimeErroSemanticoExiste(lexeme, "uma variavel", line);
//    }
//    else if (types->cSearch(lexeme))
//    {
//        // TODO imprimeErroSemanticoExiste(lexeme, "um type", line);
//    }
//    else
//    {
//        functions->cInsert(decl, lexeme);
//    }
//
//}
//
//static VarSymbol *getDecl(const char *lexeme)
//{
//    int levelAux = level; // levelAux == nivel atual
//    while (levelAux)
//    {
//        VarSymbol *aux = (VarSymbol *) scope[levelAux - 1]->cSearch(lexeme);
//        if (aux)
//        {
//            return aux;
//        }
//        levelAux--;
//    }
//    return NULL;
//}