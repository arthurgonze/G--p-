#include "parser.h"
#include "visitor.h"

PrintAST *visitor;
ProgramNode *program;

ProgramNode *Parser::StartParser()
{
    Advance();
    // TODO fprintf(stdout, "PROGRAM\n");
    program = Program(nullptr, nullptr, nullptr);
    visitor = new PrintAST();
    program->accept(visitor);
    delete (visitor);
    return program;
}

/**
* If the current token is the expected eats the token and return the lexeme if the current token is LITERAL, INTEGER, REAL, ID or CHARACTER.
* Else prints an error message and skips all tokens until it finds one that is in the sync set.
*/
const char *Parser::EatOrSkip(int expectedToken, const int *syncSet)
{
    if (tok==expectedToken)
    {
        const char *aux = (tok==LITERAL || tok==NUMINT || tok==NUMFLOAT || tok==ID || tok==LITERALCHAR) ? lexical_analyzer_last_lexeme() : nullptr;
        Eat(tok);
        return aux;
    }
    else
    {
        fprintf(stderr, "[SYNTAX ERROR] Error(EatOrSkip), Token error: Esperado: %s, Processado: %s \n", token_id_to_name(expectedToken), token_id_to_name(tok));
        int i = 0;
        while (tok!=ENDOFFILE)
        {
            while (syncSet[i]!='\0')
            {
                if (tok!=syncSet[i])
                {
                    i++;
                }
                else
                {
                    return nullptr;
                }
            }
            i = 0;
            Eat(tok);
        }
        return nullptr;
    }
}

/**
* Skips all tokens until it finds one that is in the sync set.
*/
void Parser::Sync(int token, const int *syncSet)
{
    fprintf(stderr, "[SYNTAX ERROR] Error(Sync), Token error: %s \n", token_id_to_name(token));
    int i = 0;
    while (tok!=ENDOFFILE)
    {
        while (syncSet[i]!='\0')
        {
            if (tok!=syncSet[i])
            {
                i++;
            }
            else
            {
                return;
            }
        }
        i = 0;
        Eat(tok);
    }
}

void Parser::Eat(int t)
{
    if (tok==t)
    {
        if (t==ID || t==NUMINT || t==NUMFLOAT|| t == LITERALCHAR || t == LITERAL)
        {
            // TODO fprintf(stdout, "MATCH - %s.%s\n", token_id_to_name(t), lexical_analyzer_last_lexeme());
        }
        else
        {
            // TODO fprintf(stdout, "MATCH - %s\n", token_id_to_name(t));
        }
        Advance();
    }
    else
    {
        fprintf(stderr, "[SYNTAX ERROR] Error(Eat), Token error: Esperado: %s, Processado: %s \n", token_id_to_name(t), token_id_to_name(tok));
    }

}

int Parser::programFollowSet[] = {TYPEDEF, INT, FLOAT, BOOL, ID, CHAR, ENDOFFILE, '\0'};
ProgramNode *Parser::Program(FunctionListNode *functionList, TypeDeclNode *typeList, VarDeclNode *varList) // OK
{
    switch (tok)
    {
        case TYPEDEF:
        {
            Eat(TYPEDEF);

            EatOrSkip(STRUCT, programFollowSet);
            EatOrSkip(LBRACE, programFollowSet);

            TypeNode *type = Type();
            IdListNode *idList = IdList();

            EatOrSkip(SEMICOLON, programFollowSet);

            VarDeclNode *varListNode = new VarDeclNode(type, idList, VarDecl());
            varListNode->setLine(lexical_analyzer_getLine());

            EatOrSkip(RBRACE, programFollowSet);

            TokenNode *id = new TokenNode(ID, EatOrSkip(ID, programFollowSet));
            id->setLine(lexical_analyzer_getLine());

            EatOrSkip(SEMICOLON, programFollowSet);

            typeList = new TypeDeclNode(varListNode, id, typeList);
            typeList->setLine(lexical_analyzer_getLine());

            typeList = TypeDecl(typeList);
            return Program(functionList, typeList, varList);
        }
        case INT:
        case FLOAT:
        case BOOL:
        case CHAR:
        case ID:
        {
            TypeNode *type = Type();
            PointerNode *pointer = Pointer();

            TokenNode *id = new TokenNode(ID, EatOrSkip(ID, programFollowSet));
            id->setLine(lexical_analyzer_getLine());

            ASTNode *ast = ProgramAUX(type, pointer, id, varList);
            if (ast != NULL && typeid(*ast) == typeid(FunctionNode))
            {
                FunctionNode *f = (FunctionNode *) ast;

                functionList = new FunctionListNode(f, functionList);
                functionList->setLine(lexical_analyzer_getLine());

                return ProgramList(functionList, typeList, varList);
            }
            varList = (VarDeclNode *) ast;
            return Program(functionList, typeList, varList);
        }
        case ENDOFFILE:
        {
            Eat(ENDOFFILE);

            ProgramNode *programNode = new ProgramNode(functionList, typeList, varList);
            programNode->setLine(lexical_analyzer_getLine());

            return programNode;
        }
        default:
        {
            Sync(tok, programFollowSet);
            return Program(functionList, typeList, varList);
        }
    }
    ProgramNode *programNode = new ProgramNode(functionList, typeList, varList);
    programNode->setLine(lexical_analyzer_getLine());

    return programNode;
}

int Parser::programAUXFollowSet[] = {ENDOFFILE, '\0'};
ASTNode *Parser::ProgramAUX(TypeNode *type, PointerNode *pointer, TokenNode *id, VarDeclNode *varFunc)
{
    // TODO fprintf(stdout,"PROGRAM_AUX\n");
    switch (tok)
    {
        case LPARENT:
        {
            Eat(LPARENT);
            FormalListNode *parameters = FormalList();

            EatOrSkip(RPARENT, programAUXFollowSet);
            EatOrSkip(LBRACE, programAUXFollowSet);

            VarStmtNode *varStmtNode = VarStmt(nullptr);

            FunctionNode *func = new FunctionNode(type, pointer, id, parameters, varStmtNode->getDecl(), varStmtNode->getBody());
            func->setLine(lexical_analyzer_getLine());

            EatOrSkip(RBRACE, programAUXFollowSet);
            return func;
        }
        case COMMA:
        case LBRACE:
        case SEMICOLON: // PARA O CASO int a; no main
        {
            ArrayNode *array = Array();

            IdListNode *idList = new IdListNode(pointer, id, array, IdListAUX());
            idList->setLine(lexical_analyzer_getLine());

            EatOrSkip(SEMICOLON, programAUXFollowSet);

            VarDeclNode *list = new VarDeclNode(type, idList, varFunc);
            list->setLine(lexical_analyzer_getLine());

            return list;
        }
        default:
        {
            return nullptr;
        }
    }
}

int Parser::programListFollowSet[] = {ENDOFFILE, '\0'};
ProgramNode *Parser::ProgramList(FunctionListNode *functions, TypeDeclNode *typelist, VarDeclNode *varlist)// OK
{
    // TODO fprintf(stdout,"PROGRAM_LIST\n");
    switch (tok)
    {
        case ENDOFFILE:
        {
            Eat(ENDOFFILE);

            ProgramNode *programNode = new ProgramNode(functions, typelist, varlist);
            programNode->setLine(lexical_analyzer_getLine());

            return programNode;
        }
        default:
        {
            return Program(functions, typelist, varlist);
        }
    }
}

int Parser::typeDeclFollowSet[] = {INT, FLOAT, BOOL, ID, CHAR, TYPEDEF, ENDOFFILE, '\0'};
TypeDeclNode *Parser::TypeDecl(TypeDeclNode *typeListNode)
{
    // TODO fprintf(stdout,"TYPE_DECL\n");
    switch (tok)
    {
        case TYPEDEF:
        {
            Eat(TYPEDEF);

            EatOrSkip(STRUCT, typeDeclFollowSet);
            EatOrSkip(LBRACE, typeDeclFollowSet);

            TypeNode *typeNode = Type();
            IdListNode *idListNode = IdList();

            EatOrSkip(SEMICOLON, typeDeclFollowSet);

            VarDeclNode *varListNode = new VarDeclNode(typeNode, idListNode, VarDecl());
            varListNode->setLine(lexical_analyzer_getLine());

            EatOrSkip(RBRACE, typeDeclFollowSet);

            TokenNode *id = new TokenNode(ID, EatOrSkip(ID, typeDeclFollowSet));
            id->setLine(lexical_analyzer_getLine());

            EatOrSkip(SEMICOLON, typeDeclFollowSet);

            typeListNode = new TypeDeclNode(varListNode, id, typeListNode);
            typeListNode->setLine(lexical_analyzer_getLine());

            return TypeDecl(typeListNode);
        }
        default:
        {
            return typeListNode;
        }
    }
}

int Parser::varDeclFollowSet[] = {IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACE, TRY, NOT, PLUS, MINUS, STAR, ADDRESS,
                                  ID, NUMINT, NUMFLOAT, LITERAL, LITERALCHAR, TRUE, FALSE, LPARENT, RBRACE, ENDOFFILE, '\0'};
VarDeclNode *Parser::VarDecl()
{
    // TODO fprintf(stdout,"VAR_DECL\n");
    switch (tok)
    {
        case INT:
        case FLOAT:
        case BOOL:
        case ID:
        case CHAR:
        {
            TypeNode *type = Type();
            IdListNode *idListNode = IdList();
            EatOrSkip(SEMICOLON, varDeclFollowSet);

            VarDeclNode *varList = new VarDeclNode(type, idListNode, VarDecl());
            varList->setLine(lexical_analyzer_getLine());

            return varList;
        }
        default:
        {
            return nullptr;
        }
    }
}

int Parser::varStmtFollowSet[] = {INT, FLOAT, BOOL, ID, CHAR, IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACE, TRY, NOT, PLUS, MINUS,
                                  ID, NUMINT, NUMFLOAT, LITERAL, LITERALCHAR, LPARENT, ADDRESS, STAR, TRUE, FALSE, RBRACE, ENDOFFILE, '\0'};
VarStmtNode *Parser::VarStmt(VarDeclNode *varList)
{
    // TODO fprintf(stdout,"VAR_STMT\n");
    switch (tok)
    {
        case ID:
        {
            TokenNode *id = new TokenNode(ID,  EatOrSkip(ID, varStmtFollowSet));
            id->setLine(lexical_analyzer_getLine());

            return VarStmtAux(id, varList);
        }
        case INT:
        case FLOAT:
        case BOOL:
        case CHAR:
        {
            TypeNode *type = TypeAux();
            IdListNode *idList = IdList();
            EatOrSkip(SEMICOLON, varStmtFollowSet);

            varList = new VarDeclNode(type, idList, varList);
            varList->setLine(lexical_analyzer_getLine());

            return VarStmt(varList);
        }

        case IF:
        case WHILE:
        case SWITCH:
        case BREAK:
        case PRINT:
        case READLN:
        case RETURN:
        case THROW:
        case LBRACE:
        case TRY:
        case NOT:
        case PLUS:
        case MINUS:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LPARENT:
        case ADDRESS:
        case STAR:
        case TRUE:
        case FALSE:
        {
            VarStmtNode *varStmtNode = new VarStmtNode(varList, StmtList());
            varStmtNode->setLine(lexical_analyzer_getLine());

            return varStmtNode;
        }
        default:
        {
            fprintf(stderr, "[SYNTAX ERROR] error(VarStmt), Token error: %s \n", token_id_to_name(tok));

            VarStmtNode *varStmtNode = new VarStmtNode(varList, nullptr);
            varStmtNode->setLine(lexical_analyzer_getLine());

            return varStmtNode;
        }
    }
}

int Parser::varStmtAuxFollowSet[] = {RBRACE, ENDOFFILE, '\0'};
VarStmtNode *Parser::VarStmtAux(TokenNode *id, VarDeclNode *varList)
{
    // TODO fprintf(stdout,"VAR_STMT_AUX\n");
    switch (tok)
    {
        case STAR:
        case ID:
        {
            IdListNode *idListNode = IdList();
            EatOrSkip(SEMICOLON, varStmtAuxFollowSet);

            id->setTypeLexeme(id->getLexeme());
            TypeNode *typeNode = new TypeNode(id);
            typeNode->setLine(lexical_analyzer_getLine());

            varList = new VarDeclNode(typeNode, idListNode, varList);
            varList->setLine(lexical_analyzer_getLine());

            return VarStmt(varList);
        }
        case LPARENT:
        case DOT:
        case POINTER:
        case LBRACE:
        case PERCENT:
        case SLASH:
        case ASSIGN:
        case SEMICOLON:
        case RPARENT:
        case PIPE:
        case PLUS:
        case MINUS:
        case NOT:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case ADDRESS:
        case TRUE:
        case FALSE:
        case EQ:
        case NE:
        case AND:
        case OR:
        case LBRACKET:
        {
            ExpNode *exp = PostFixExpr(id);
            exp = ExprMultiplicativeAUX(exp);
            exp = ExprAdditiveAUX(exp);
            exp = ExprRelationalAUX(exp);
            exp = ExprEqualityAUX(exp);
            exp = ExprAndAUX(exp);
            exp = ExprOrAUX(exp);
            exp = ExprAssignAUX(exp);

            EatOrSkip(SEMICOLON, varStmtAuxFollowSet);

            StmtNode *stmtNode = new StmtNode(exp);
            stmtNode->setLine(lexical_analyzer_getLine());

            StmtListNode *stmtList = new StmtListNode(stmtNode, StmtListAUX());
            stmtList->setLine(lexical_analyzer_getLine());

            VarStmtNode *varStmtNode = new VarStmtNode(varList, stmtList);
            varStmtNode->setLine(lexical_analyzer_getLine());

            return varStmtNode;
        }
        default:
        {
            Sync(tok, varStmtAuxFollowSet);
        }
    }
    VarStmtNode *varStmtNode = new VarStmtNode(varList, nullptr);
    varStmtNode->setLine(lexical_analyzer_getLine());

    return varStmtNode;
}

int Parser::idListFollowSet[] = {SEMICOLON, RPARENT, ENDOFFILE, '\0'};
IdListNode *Parser::IdList()
{
    // TODO fprintf(stdout,"ID_LIST\n");
    PointerNode *pointer = Pointer();
    switch (tok)
    {
        case ID:
        {
            TokenNode *id = new TokenNode(ID, EatOrSkip(ID, idListFollowSet));
            id->setLine(lexical_analyzer_getLine());
            id->setType(ID);
            id->setTypeLexeme(lexical_analyzer_last_lexeme());

            ArrayNode *array = Array();

            IdListNode *idListNode = new IdListNode(pointer, id, array, IdListAUX());
            idListNode->setLine(lexical_analyzer_getLine());

            return idListNode;
        }
        default:
        {
            return nullptr;
        }
    }
}

int Parser::idListAUXFollowSet[] = {SEMICOLON, RPARENT, ENDOFFILE, '\0'};
IdListNode *Parser::IdListAUX()
{
    // TODO fprintf(stdout,"ID_LIST_AUX\n");
    switch (tok)
    {
        case COMMA:
        {
            Eat(COMMA);
            PointerNode *pointer = Pointer();

            TokenNode *id = new TokenNode(ID, EatOrSkip(ID, idListAUXFollowSet));
            id->setLine(lexical_analyzer_getLine());

            ArrayNode *array = Array();

            IdListNode *idListNode = new IdListNode(pointer, id, array, IdListAUX());
            idListNode->setLine(lexical_analyzer_getLine());

            return idListNode;
        }
        default:
        {
            return nullptr;
        }
    }
}

int Parser::pointerFollowSet[] = {RPARENT, ID, LBRACE, COMMA, ENDOFFILE, '\0'};
PointerNode *Parser::Pointer()
{
    // TODO fprintf(stdout,"POINTER\n");
    switch (tok)
    {
        case STAR:
        {
            Eat(STAR);

            PointerNode *pointerNode = new PointerNode();
            pointerNode->setLine(lexical_analyzer_getLine());

            return pointerNode;
        }
        default:
        {
            return nullptr;
        }
    }
}

int Parser::arrayFollowSet[] = {SEMICOLON, COMMA, RPARENT, ENDOFFILE, '\0'};
ArrayNode *Parser::Array()
{
    // TODO fprintf(stdout,"ARRAY\n");
    switch (tok)
    {
        case LBRACKET:
        {
            Eat(LBRACKET);

            TokenNode *numInt = new TokenNode(NUMINT, EatOrSkip(NUMINT, arrayFollowSet));
            numInt->setLine(lexical_analyzer_getLine());

            EatOrSkip(RBRACKET, arrayFollowSet);

            ArrayNode *arrayNode = new ArrayNode(numInt);
            arrayNode->setLine(lexical_analyzer_getLine());

            return arrayNode;
        }
        default:
        {
            return nullptr;
        }
    }
}

int Parser::formalListFollowSet[] = {RPARENT, COMMA, ENDOFFILE, '\0'};
FormalListNode *Parser::FormalList()
{
    // TODO fprintf(stdout, "FORMAL_LIST\n");

    switch (tok)
    {
        case INT:
        case FLOAT:
        case BOOL:
        case ID:
        case CHAR:
        {
            TypeNode *type = Type();
            PointerNode *pointer = Pointer();

            TokenNode *id = new TokenNode(ID, EatOrSkip(ID, formalListFollowSet));
            id->setLine(lexical_analyzer_getLine());

            ArrayNode *array = Array();

            FormalListNode *list = new FormalListNode(type, pointer, id, array, FormalRest());
            list->setLine(lexical_analyzer_getLine());

            return list;
        }
        default:
        {
            return nullptr;
        }
    }
}

int Parser::formalRestFollowSet[] = {RPARENT, ENDOFFILE, '\0'};
FormalListNode *Parser::FormalRest()
{
    // TODO fprintf(stdout,"FORMAL_REST\n");
    switch (tok)
    {
        case COMMA:
        {
            Eat(COMMA);
            TypeNode *typeNode = Type();
            PointerNode *pointer = Pointer();

            TokenNode *id = new TokenNode(ID, EatOrSkip(ID, formalRestFollowSet));
            id->setLine(lexical_analyzer_getLine());

            ArrayNode *array = Array();

            FormalListNode *formalList = new FormalListNode(typeNode, pointer, id, array, FormalRest());
            formalList->setLine(lexical_analyzer_getLine());

            return formalList;
        }
        default:
        {
            return nullptr;
        }
    }
}

int Parser::typeFollowSet[] = {LPARENT, STAR, ID, ENDOFFILE, '\0'};
TypeNode *Parser::Type()
{
    // TODO fprintf(stdout,"TYPE\n");
    switch (tok)
    {
        case INT:
        {
            TokenNode *type = new TokenNode(INT, nullptr);
            type->setLine(lexical_analyzer_getLine());
            type->setType(INT);

            Eat(INT);

            TypeNode *typeNode = new TypeNode(type);
            typeNode->setLine(lexical_analyzer_getLine());

            return typeNode;
        }
        case FLOAT:
        {
            TokenNode *type = new TokenNode(FLOAT, nullptr);
            type->setLine(lexical_analyzer_getLine());
            type->setType(FLOAT);

            Eat(FLOAT);

            TypeNode *typeNode = new TypeNode(type);
            typeNode->setLine(lexical_analyzer_getLine());

            return typeNode;
        }
        case BOOL:
        {
            TokenNode *type = new TokenNode(BOOL, nullptr);
            type->setLine(lexical_analyzer_getLine());
            type->setType(BOOL);

            Eat(BOOL);

            TypeNode *typeNode = new TypeNode(type);
            typeNode->setLine(lexical_analyzer_getLine());

            return typeNode;
        }
        case ID:
        {
            TokenNode *type = new TokenNode(ID, lexical_analyzer_last_lexeme());
            type->setLine(lexical_analyzer_getLine());
            type->setType(ID);
            type->setTypeLexeme(lexical_analyzer_last_lexeme());

            Eat(ID);

            TypeNode *typeNode = new TypeNode(type);
            typeNode->getId()->getLexeme();
            typeNode->setLine(lexical_analyzer_getLine());

            return typeNode;
        }
        case CHAR:
        {
            TokenNode *type = new TokenNode(CHAR, nullptr);
            type->setLine(lexical_analyzer_getLine());
            type->setType(CHAR);

            Eat(CHAR);

            TypeNode *typeNode = new TypeNode(type);
            typeNode->setLine(lexical_analyzer_getLine());

            return typeNode;
        }
        default:
        {
            Sync(tok, typeFollowSet);
        }
    }
    return nullptr;
}

int Parser::typeAuxFollowSet[] = {LPARENT, STAR, ID, ENDOFFILE, '\0'};
TypeNode *Parser::TypeAux()
{
    // TODO fprintf(stdout,"TYPE_AUX\n");
    switch (tok)
    {
        case INT:
        {
            TokenNode *type = new TokenNode(INT, nullptr);
            type->setLine(lexical_analyzer_getLine());

            Eat(INT);

            TypeNode *typeNode = new TypeNode(type);
            typeNode->setLine(lexical_analyzer_getLine());

            return typeNode;
        }
        case FLOAT:
        {
            TokenNode *type = new TokenNode(FLOAT, nullptr);
            type->setLine(lexical_analyzer_getLine());

            Eat(FLOAT);

            TypeNode *typeNode = new TypeNode(type);
            typeNode->setLine(lexical_analyzer_getLine());

            return typeNode;
        }
        case BOOL:
        {
            TokenNode *type = new TokenNode(BOOL, nullptr);
            type->setLine(lexical_analyzer_getLine());

            Eat(BOOL);

            TypeNode *typeNode = new TypeNode(type);
            typeNode->setLine(lexical_analyzer_getLine());

            return typeNode;
        }
        case CHAR:
        {
            TokenNode *type = new TokenNode(CHAR, nullptr);
            type->setLine(lexical_analyzer_getLine());

            Eat(CHAR);

            TypeNode *typeNode = new TypeNode(type);
            typeNode->setLine(lexical_analyzer_getLine());

            return typeNode;
        }
        default:
        {
            Sync(tok, typeAuxFollowSet);
        }
    }
    return nullptr;
}

int Parser::stmtListFollowSet[] = {RBRACE, CASE, ENDOFFILE, '\0'};
StmtListNode *Parser::StmtList()
{
    // TODO fprintf(stdout,"STMT_LIST\n");
    switch (tok)
    {
        case LITERALCHAR:
        case IF:
        case WHILE:
        case SWITCH:
        case BREAK:
        case PRINT:
        case READLN:
        case RETURN:
        case THROW:
        case LBRACE:
        case TRY:
        case NOT:
        case PLUS:
        case MINUS:
        case STAR:
        case ADDRESS:
        case ID:
        case NUMFLOAT:
        case NUMINT:
        case LITERAL:
        case TRUE:
        case FALSE:
        case LPARENT:
        {
            StmtListNode *stmtListNode = new StmtListNode(Stmt(), StmtListAUX());
            stmtListNode->setLine(lexical_analyzer_getLine());

            return stmtListNode;
        }
        default:
        {
            Sync(tok, stmtListFollowSet);
        }
    }
    return nullptr;
}

int Parser::stmtListAUXFollowSet[] = {RBRACKET, CASE, ENDOFFILE, '\0'};
StmtListNode *Parser::StmtListAUX()
{
    // TODO fprintf(stdout,"STMT_LIST_AUX\n");
    switch (tok)
    {
        case LITERALCHAR:
        case IF:
        case WHILE:
        case SWITCH:
        case BREAK:
        case PRINT:
        case READLN:
        case RETURN:
        case THROW:
        case LBRACE:
        case TRY:
        case NOT:
        case PLUS:
        case MINUS:
        case STAR:
        case ADDRESS:
        case ID:
        case NUMFLOAT:
        case NUMINT:
        case LITERAL:
        case TRUE:
        case FALSE:
        case LPARENT:
        {
            return StmtList();
        }
        default:
        {
            return nullptr;
        }
    }
}

int Parser::stmtFollowSet[] = {CATCH, ELSE, IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACE, TRY, NOT, PLUS, MINUS, ID,
                               NUMINT, NUMFLOAT, LITERAL, LITERALCHAR, LPARENT,  ADDRESS, STAR, TRUE, FALSE, RBRACE, CASE, ENDOFFILE, '\0'};
StmtNode *Parser::Stmt()
{
    // TODO fprintf(stdout,"STMT\n");
    switch (tok)
    {
        case IF:
        {
            Eat(IF);

            EatOrSkip(LPARENT, stmtFollowSet);
            ExpNode *exp = ExprAssign();
            EatOrSkip(RPARENT, stmtFollowSet);

            StmtNode *ifStmt = Stmt();
            StmtNode *elseStmt = ElseStmt();

            IfNode *stmt = new IfNode(exp, ifStmt, elseStmt);
            stmt->setLine(lexical_analyzer_getLine());

            StmtNode *stmtNode = new StmtNode(stmt);
            stmtNode->setLine(lexical_analyzer_getLine());

            return stmtNode;
        }

        case LITERALCHAR:
        case WHILE:
        case SWITCH:
        case BREAK:
        case PRINT:
        case READLN:
        case RETURN:
        case THROW:
        case LBRACE:
        case TRY:
        case NOT:
        case PLUS:
        case MINUS:
        case STAR:
        case ADDRESS:
        case ID:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case TRUE:
        case FALSE:
        case LPARENT:
        {
            return StmtAUX();
        }
        default:
        {
            Sync(tok, stmtFollowSet);
        }
    }
    return nullptr;
}


int Parser::stmtAUXFollowSet[] = {CATCH, ELSE, IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACE, TRY, NOT, PLUS, MINUS, ID, NUMINT,
                                  NUMFLOAT, LITERAL, LITERALCHAR, LPARENT, ADDRESS, STAR, TRUE, FALSE, RBRACE, CASE, ENDOFFILE, '\0'};
StmtNode *Parser::StmtAUX()
{
    // TODO fprintf(stdout,"STMT_AUX\n");
    switch (tok)
    {
        case WHILE:
        {
            Eat(WHILE);

            EatOrSkip(LPARENT, stmtAUXFollowSet);
            ExpNode *exp = ExprAssign();
            EatOrSkip(RPARENT, stmtAUXFollowSet);

            WhileNode *aux = new WhileNode(exp, Stmt());
            aux->setLine(lexical_analyzer_getLine());

            StmtNode *stmtNode = new StmtNode(aux);
            stmtNode->setLine(lexical_analyzer_getLine());

            return stmtNode;
        }
        case SWITCH:
        {
            Eat(SWITCH);

            EatOrSkip(LPARENT, stmtAUXFollowSet);
            ExpNode *exp = ExprAssign();
            EatOrSkip(RPARENT, stmtAUXFollowSet);

            EatOrSkip(LBRACE, stmtAUXFollowSet);
            CaseBlockNode *cbNode = CaseBlock();
            EatOrSkip(RBRACE, stmtAUXFollowSet) ;

            SwitchNode *aux = new SwitchNode(exp, cbNode);
            aux->setLine(lexical_analyzer_getLine());

            StmtNode *stmtNode = new StmtNode(aux);
            stmtNode->setLine(lexical_analyzer_getLine());

            return stmtNode;
        }
        case BREAK:
        {
            Eat(BREAK);

            BreakNode *breakNode = new BreakNode();
            breakNode->setLine(lexical_analyzer_getLine());

            EatOrSkip(SEMICOLON, stmtAUXFollowSet);

            StmtNode *stmtNode = new StmtNode(breakNode);
            stmtNode->setLine(lexical_analyzer_getLine());

            return stmtNode;
        }
        case PRINT:
        {
            Eat(PRINT);

            EatOrSkip(LPARENT, stmtAUXFollowSet);
            ExpListNode *expList = ExprList();
            EatOrSkip(RPARENT, stmtAUXFollowSet);

            EatOrSkip(SEMICOLON, stmtAUXFollowSet);

            PrintNode *print = new PrintNode(expList);
            print->setLine(lexical_analyzer_getLine());

            StmtNode *stmtNode = new StmtNode(print);
            stmtNode->setLine(lexical_analyzer_getLine());

            return stmtNode;
        }
        case READLN:
        {
            Eat(READLN);

            EatOrSkip(LPARENT, stmtAUXFollowSet);
            ExpNode *exp = ExprAssign();
            EatOrSkip(RPARENT, stmtAUXFollowSet);

            EatOrSkip(SEMICOLON, stmtAUXFollowSet);

            ReadLnNode *readLnNode = new ReadLnNode(exp);
            readLnNode->setLine(lexical_analyzer_getLine());

            StmtNode *stmtNode = new StmtNode(readLnNode);
            stmtNode->setLine(lexical_analyzer_getLine());

            return stmtNode;
        }
        case RETURN:
        {
            Eat(RETURN);
            ExpNode *exp = ExprAssign();
            EatOrSkip(SEMICOLON, stmtAUXFollowSet);

            ReturnNode *returnNode = new ReturnNode(exp);
            returnNode->setLine(lexical_analyzer_getLine());

            StmtNode *stmtNode = new StmtNode(returnNode);
            stmtNode->setLine(lexical_analyzer_getLine());

            return stmtNode;
        }
        case THROW:
        {
            Eat(THROW);
            EatOrSkip(SEMICOLON, stmtAUXFollowSet);

            ThrowNode *throwNode = new ThrowNode;
            throwNode->setLine(lexical_analyzer_getLine());

            StmtNode *stmtNode = new StmtNode(throwNode);
            stmtNode->setLine(lexical_analyzer_getLine());

            return stmtNode;
        }
        case LBRACE:
        {
            Eat(LBRACE);
            StmtListNode *stmtList = StmtList();
            EatOrSkip(RBRACE, stmtAUXFollowSet);

            StmtNode *stmtNode = new StmtNode(stmtList);
            stmtNode->setLine(lexical_analyzer_getLine());

            return stmtNode;
        }
        case TRY:
        {
            Eat(TRY);
            StmtNode *tryStmt = Stmt();
            EatOrSkip(CATCH, stmtAUXFollowSet);
            EatOrSkip(LPARENT, stmtAUXFollowSet);

            EatOrSkip(DOT, stmtAUXFollowSet);
            EatOrSkip(DOT, stmtAUXFollowSet);
            EatOrSkip(DOT, stmtAUXFollowSet);

            EatOrSkip(RPARENT, stmtAUXFollowSet);
            StmtNode *exception = Stmt();

            TryNode *tryNode = new TryNode(tryStmt, exception);
            tryNode->setLine(lexical_analyzer_getLine());

            StmtNode *stmtNode = new StmtNode(tryNode);
            stmtNode->setLine(lexical_analyzer_getLine());

            return stmtNode;
        }
        case NOT:
        case PLUS:
        case MINUS:
        case ID:
        case STAR:
        case ADDRESS:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:
        {
            ExpNode *exp = ExprAssign();
            EatOrSkip(SEMICOLON, stmtAUXFollowSet);

            StmtNode *stmtNode = new StmtNode(exp);
            stmtNode->setLine(lexical_analyzer_getLine());

            return stmtNode;
        }
        default:
        {
            Sync(tok, stmtAUXFollowSet);
        }
    }
    return nullptr;
}

int Parser::elseStmtFollowSet[] = {RBRACKET, ENDOFFILE, '\0'};
StmtNode *Parser::ElseStmt()
{
    // TODO fprintf(stdout,"ELSE_STMT\n");
    switch (tok)
    {
        case ELSE:
        {
            Eat(ELSE);
            return Stmt();
        }
        default:
        {
            return nullptr;
        }
    }
}

int Parser::caseBlockFollowSet[] = {RBRACE, ENDOFFILE, '\0'};
CaseBlockNode *Parser::CaseBlock()
{
    // TODO fprintf(stdout,"CASE_BLOCK\n");
    switch (tok)
    {
        case CASE:
        {
            Eat(CASE);

            TokenNode *token = new TokenNode(NUMINT, EatOrSkip(NUMINT, caseBlockFollowSet));
            token->setLine(lexical_analyzer_getLine());

            EatOrSkip(COLON, caseBlockFollowSet);
            return CaseBlockAUX(token);
        }
        case  RBRACE:
        {
            return nullptr;
        }
        default:
        {
            Sync(tok, caseBlockFollowSet);
        }
    }
    return nullptr;
}

int Parser::caseBlockAUXFollowSet[] = {RBRACE, ENDOFFILE, '\0'};
CaseBlockNode *Parser::CaseBlockAUX(TokenNode *num)
{
    // TODO fprintf(stdout,"CASE_BLOCK_AUX\n");
    switch (tok)
    {
        case CASE:
        {
            CaseBlockNode *caseBlockNode = new CaseBlockNode(num, nullptr, CaseBlock());
            caseBlockNode->setLine(lexical_analyzer_getLine());

            return caseBlockNode;
        }

        case IF:
        case WHILE:
        case SWITCH:
        case BREAK:
        case PRINT:
        case READLN:
        case RETURN:
        case THROW:
        case LBRACE:
        case TRY:
        case NOT:
        case PLUS:
        case MINUS:
        case STAR:
        case ADDRESS:
        case ID:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:
        {
            CaseBlockNode *caseBlockNode = new CaseBlockNode(num, StmtList(), CaseBlock());
            caseBlockNode->setLine(lexical_analyzer_getLine());

            return caseBlockNode;
        }
        default:
        {
            return nullptr;
        }
    }
}

int Parser::exprListFollowSet[] = {RPARENT, ENDOFFILE, '\0'};
ExpListNode *Parser::ExprList()
{
    // TODO fprintf(stdout,"EXPR_LIST\n");

    switch (tok)
    {
        case NOT:
        case PLUS:
        case MINUS:
        case STAR:
        case ADDRESS:
        case ID:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:
        {
            return ExprListTail();
        }
        default:
        {
            return nullptr;
        }
    }
}

int Parser::exprListTailFollowSet[] = {RPARENT, ENDOFFILE, '\0'};
ExpListNode *Parser::ExprListTail()
{
    // TODO fprintf(stdout,"EXPR_LIST_TAIL\n");
    switch (tok)
    {
        case NOT:
        case PLUS:
        case MINUS:
        case STAR:
        case ADDRESS:
        case ID:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:
        {
            ExpListNode *expListNode = new ExpListNode(ExprAssign(), ExprListTailAUX());
            expListNode->setLine(lexical_analyzer_getLine());

            return expListNode;
        }
        default:
        {
            Sync(tok, exprListTailFollowSet);
        }
    }
    return nullptr;
}

int Parser::exprListTailAUXFollowSet[] = {RPARENT, ENDOFFILE, '\0'};
ExpListNode *Parser::ExprListTailAUX()
{
    // TODO fprintf(stdout,"EXPR_LIST_AUX\n");
    switch (tok)
    {
        case COMMA:
        {
            Eat(COMMA);
            return ExprListTail();
        }
        default:
        {
            return nullptr;
        }
    }
}

int Parser::exprAssignFollowSet[] = {SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};
ExpNode *Parser::ExprAssign()
{
    // TODO fprintf(stdout,"EXPR_ASSING\n");
    switch (tok)
    {
        case NOT:
        case PLUS:
        case MINUS:
        case STAR:
        case ADDRESS:
        case ID:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:
        {
            return ExprAssignAUX(ExprOr());
        }
        default:
        {
            Sync(tok, exprAssignFollowSet);
        }
    }
    return nullptr;
}

int Parser::exprAssignAUXFollowSet[] = {SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};
ExpNode *Parser::ExprAssignAUX(ExpNode *expr)
{
    // TODO fprintf(stdout,"EXPR_ASSING_AUX\n");
    ExpNode *exp2 = nullptr;
    ExpNode *node = expr;
    switch (tok)
    {
        case ASSIGN:
        {
            Eat(ASSIGN);
            exp2 = ExprAssign();

            expr->setLValue(true);
            node = new AssignNode(expr, exp2);
            node->setLine(lexical_analyzer_getLine());

            break;
        }
    }
    return node;
}

int Parser::exprOrFollowSet[] = {EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};
ExpNode *Parser::ExprOr()
{
    // TODO fprintf(stdout,"EXPR_OR\n");

    switch (tok)
    {
        case NOT:
        case PLUS:
        case MINUS:
        case STAR:
        case ADDRESS:
        case ID:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:
        {
            return ExprOrAUX(ExprAnd());
        }
        default:
        {
            Sync(tok, exprOrFollowSet);
        }
    }
    return nullptr;
}

int Parser::exprOrAUXFollowSet[] = {EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};
ExpNode *Parser::ExprOrAUX(ExpNode *expr)
{
    TokenNode *token = nullptr;
    ExpNode *exp2 = nullptr;
    ExpNode *node = expr;
    switch (tok)
    {
        case OR:
        {
            Eat(OR);

            token = new TokenNode(OR, nullptr);
            token->setLine(lexical_analyzer_getLine());

            exp2 = ExprOr();

            node = new BooleanOPNode(token, expr, exp2);
            node->setLine(lexical_analyzer_getLine());
            node->setType(BOOL);

            break;
        }
    }
    //delete(token);
    //delete(exp2);
    return node;
}

int Parser::exprAndFollowSet[] = {OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};
ExpNode *Parser::ExprAnd()
{
    // TODO fprintf(stdout,"EXPR_AND\n");
    switch (tok)
    {
        case NOT:
        case PLUS:
        case MINUS:
        case STAR:
        case ADDRESS:
        case ID:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:
        {
            return ExprAndAUX(ExprEquality());
        }
        default:
        {
            Sync(tok, exprAndFollowSet);
        }
    }
    return nullptr;
}

int Parser::exprAndAUXFollowSet[] = {OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};
ExpNode *Parser::ExprAndAUX(ExpNode *expr)
{
    // TODO fprintf(stdout,"EXPR_AND_AUX\n");

    TokenNode *token = nullptr;
    ExpNode *exp2 = nullptr;
    ExpNode *node = expr;
    switch (tok)
    {
        case AND:
        {
            Eat(AND);

            token = new TokenNode(AND, nullptr);
            token->setLine(lexical_analyzer_getLine());

            exp2 = ExprAnd();

            node = new BooleanOPNode(token, expr, exp2);
            node->setLine(lexical_analyzer_getLine());
            node->setType(BOOL);

            break;
        }
    }
    return node;
}

int Parser::exprEqualityFollowSet[] = {AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};
ExpNode *Parser::ExprEquality()
{
    // TODO fprintf(stdout,"EXPR_EQUALITY\n");

    switch (tok)
    {
        case NOT:
        case PLUS:
        case MINUS:
        case STAR:
        case ADDRESS:
        case ID:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:
        {
            return ExprEqualityAUX(ExprRelational());
        }
        default:
        {
            Sync(tok, exprEqualityFollowSet);
        }
    }
    return nullptr;
}

int Parser::exprEqualityAUXFollowSet[] = {AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};
ExpNode *Parser::ExprEqualityAUX(ExpNode *expr)
{
    // TODO fprintf(stdout,"EXPR_EQUALITY_AUX\n");

    TokenNode *token = nullptr;
    ExpNode *exp2 = nullptr;
    ExpNode *node = expr;
    switch (tok)
    {
        case EQ:
        {
            Eat(EQ);

            token = new TokenNode(EQ, nullptr);
            token->setLine(lexical_analyzer_getLine());

            exp2 = ExprEquality();

            node = new BooleanOPNode(token, expr, exp2);
            node->setLine(lexical_analyzer_getLine());
            node->setType(BOOL);

            break;
        }
        case NE:
        {
            Eat(NE);

            token = new TokenNode(NE, nullptr);
            token->setLine(lexical_analyzer_getLine());

            exp2 = ExprEquality();

            node = new BooleanOPNode(token, expr, exp2);
            node->setLine(lexical_analyzer_getLine());
            node->setType(BOOL);

            break;
        }
    }
    return node;
}

int Parser::exprRelationalFollowSet[] = {EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};
ExpNode *Parser::ExprRelational()
{
    // TODO fprintf(stdout,"EXPR_RELATIONAL\n");

    switch (tok)
    {
        case NOT:
        case PLUS:
        case MINUS:
        case STAR:
        case ADDRESS:
        case ID:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:
        {
            return ExprRelationalAUX(ExprAdditive());
        }
        default:
        {
            Sync(tok, exprRelationalFollowSet);
        }
    }
    return nullptr;
}

int Parser::exprRelationalAUXFollowSet[] = {EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};
ExpNode *Parser::ExprRelationalAUX(ExpNode *expr)
{
    // TODO fprintf(stdout,"EXPR_RELATIONAL_AUX\n");

    TokenNode *token = nullptr;
    ExpNode *exp2 = nullptr;
    ExpNode *node = expr;
    switch (tok)
    {
        case LT:
        {
            Eat(LT);

            token = new TokenNode(LT, nullptr);
            token->setLine(lexical_analyzer_getLine());

            exp2 = ExprRelational();

            node = new BooleanOPNode(token, expr, exp2);
            node->setLine(lexical_analyzer_getLine());
            node->setType(BOOL);

            break;
        }
        case GT:
        {
            Eat(GT);

            token = new TokenNode(GT, nullptr);
            token->setLine(lexical_analyzer_getLine());

            exp2 = ExprRelational();

            node = new BooleanOPNode(token, expr, exp2);
            node->setLine(lexical_analyzer_getLine());
            node->setType(BOOL);

            break;
        }
        case LE:
        {
            Eat(LE);

            token = new TokenNode(LE, nullptr);
            token->setLine(lexical_analyzer_getLine());

            exp2 = ExprRelational();

            node = new BooleanOPNode(token, expr, exp2);
            node->setLine(lexical_analyzer_getLine());
            node->setType(BOOL);

            break;
        }
        case GE:
        {
            Eat(GE);

            token = new TokenNode(GE, nullptr);
            token->setLine(lexical_analyzer_getLine());

            exp2 = ExprRelational();

            node = new BooleanOPNode(token, expr, exp2);
            node->setLine(lexical_analyzer_getLine());
            node->setType(BOOL);

            break;
        }
    }
    return node;
}

int Parser::exprAdditiveFollowSet[] = {LT, GT, LE, GE, EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};
ExpNode *Parser::ExprAdditive()
{
    // TODO fprintf(stdout,"EXPR_ADDITIVE\n");

    switch (tok)
    {
        case NOT:
        case PLUS:
        case MINUS:
        case STAR:
        case ADDRESS:
        case ID:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:
        {
            return ExprAdditiveAUX(ExprMultiplicative());
        }
        default:
        {
            Sync(tok, exprAdditiveFollowSet);
        }
    }
    return nullptr;
}

int Parser::exprAdditiveAUXFollowSet[] = {LT, GT, LE, GE, EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};
ExpNode *Parser::ExprAdditiveAUX(ExpNode *expr)
{
    // TODO fprintf(stdout,"EXPR_ADDITIVE_AUX\n");

    TokenNode *token = nullptr;
    ExpNode *exp2 = nullptr;
    ExpNode *node = expr;
    switch (tok)
    {
        case PIPE:
        {
            Eat(PIPE);

            token = new TokenNode(PIPE, nullptr);
            token->setLine(lexical_analyzer_getLine());

            exp2 = ExprAdditive();

            node = new AdditionOPNode(token, expr, exp2);
            node->setLine(lexical_analyzer_getLine());

            break;
        }
        case PLUS:
        {
            Eat(PLUS);

            token = new TokenNode(PLUS, nullptr);
            token->setLine(lexical_analyzer_getLine());

            exp2 = ExprAdditive();

            node = new AdditionOPNode(token, expr, exp2);
            node->setLine(lexical_analyzer_getLine());

            break;
        }
        case MINUS:
        {
            Eat(MINUS);

            token = new TokenNode(MINUS, nullptr);
            token->setLine(lexical_analyzer_getLine());

            exp2 = ExprAdditive();

            node = new AdditionOPNode(token, expr, exp2);
            node->setLine(lexical_analyzer_getLine());

            break;
        }
    }
    return node;
}

int Parser::exprMultiplicativeFollowSet[] = {PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};
ExpNode *Parser::ExprMultiplicative()
{
    // TODO fprintf(stdout,"EXPR_MULTIPLICATIVE\n");

    switch (tok)
    {
        case NOT:
        case PLUS:
        case MINUS:
        case STAR:
        case ADDRESS:
        case ID:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:
        {
            return ExprMultiplicativeAUX(ExprUnary());
        }
        default:
        {
            Sync(tok, exprMultiplicativeFollowSet);
        }
    }
    return nullptr;
}

int Parser::exprMultiplicativeAUXFollowSet[] = {PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};
ExpNode *Parser::ExprMultiplicativeAUX(ExpNode *expr)
{
    // TODO fprintf(stdout,"EXPR_MULTIPLICATIVE_AUX\n");

    TokenNode *token = nullptr;
    ExpNode *exp2 = nullptr;
    ExpNode *node = expr;
    switch (tok)
    {
        case ADDRESS:// e bit a bit
        {
            Eat(ADDRESS);

            token = new TokenNode(ADDRESS, nullptr);
            token->setLine(lexical_analyzer_getLine());

            exp2 = ExprMultiplicative();

            node = new MultiplicationOPNode(token, expr, exp2);
            node->setLine(lexical_analyzer_getLine());
            //TODO definir o tipo das operacoes no semantico
            break;
        }
        case STAR:// multiplicacao
        {
            Eat(STAR);

            token = new TokenNode(STAR, nullptr);
            token->setLine(lexical_analyzer_getLine());

            exp2 = ExprMultiplicative();

            node = new MultiplicationOPNode(token, expr, exp2);
            node->setLine(lexical_analyzer_getLine());

            break;
        }
        case SLASH:// divisao
        {
            Eat(SLASH);

            token = new TokenNode(SLASH, nullptr);
            token->setLine(lexical_analyzer_getLine());

            exp2 = ExprMultiplicative();

            node = new MultiplicationOPNode(token, expr, exp2);
            node->setLine(lexical_analyzer_getLine());

            break;
        }
        case PERCENT://mod
        {
            Eat(PERCENT);

            token = new TokenNode(PERCENT, nullptr);
            token->setLine(lexical_analyzer_getLine());

            exp2 = ExprMultiplicative();

            node = new MultiplicationOPNode(token, expr, exp2);
            node->setLine(lexical_analyzer_getLine());

            break;
        }
    }
    return node;
}

int Parser::exprUnaryFollowSet[] = {ADDRESS, STAR, SLASH, PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};
ExpNode *Parser::ExprUnary()
{
    // TODO fprintf(stdout,"EXPR_UNARY\n");

    switch (tok)
    {
        case NOT:
        {
            Eat(NOT);

            NotNode *notNode = new NotNode(ExprUnary());
            notNode->setLine(lexical_analyzer_getLine());
            notNode->setType(BOOL);
            return notNode;
        }
        case PLUS:
        {
            Eat(PLUS);
            return ExprUnary();
        }
        case MINUS:
        {
            Eat(MINUS);

            SignNode *signNode = new SignNode(ExprUnary());
            signNode->setLine(lexical_analyzer_getLine());

            return signNode;
        }
        case STAR:
        {
            Eat(STAR);

            PointerValNode *pointerValNode = new PointerValNode(ExprUnary());
            pointerValNode->setPointer(false); // TODO sem confianca
            pointerValNode->setLine(lexical_analyzer_getLine());

            return pointerValNode;
        }
        case ADDRESS:
        {
            Eat(ADDRESS);

            AddressValNode *addressValNode = new AddressValNode(ExprUnary());
            addressValNode->setPointer(true); // TODO sem confianca
            addressValNode->setLine(lexical_analyzer_getLine());

            return addressValNode;
        }
        case ID:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:
        {
            return Primary();
        }
        default:
        {
            Sync(tok, exprUnaryFollowSet);
        }
    }
    return nullptr;
}

int Parser::primaryFollowSet[] = {DOT, POINTER, LBRACE, LPARENT, ADDRESS, STAR, SLASH, PIPE, PLUS, MINUS, LT, GT, LE, GE,
                                  EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};
ExpNode *Parser::Primary()
{
    // TODO fprintf(stdout,"PRIMARY\n");
    switch (tok)
    {
        case ID:
        {
            TokenNode *id = new TokenNode(ID, lexical_analyzer_last_lexeme());
            id->setType(ID);
            id->setLine(lexical_analyzer_getLine());

            Eat(ID);

            return PostFixExpr(id);
        }
        case NUMINT:
        {
            TokenNode *numInt = new TokenNode(NUMINT, lexical_analyzer_last_lexeme());
            numInt->setLine(lexical_analyzer_getLine());
            numInt->setType(INT);

            Eat(NUMINT);

            PrimaryNode* primaryNode = new PrimaryNode(numInt);
            primaryNode->setType(INT);
            return PostFixExprAUX(primaryNode);
        }
        case NUMFLOAT:
        {
            TokenNode *numFloat = new TokenNode(NUMFLOAT, lexical_analyzer_last_lexeme());
            numFloat->setLine(lexical_analyzer_getLine());
            numFloat->setType(FLOAT);

            Eat(NUMFLOAT);

            PrimaryNode *primaryNode = new PrimaryNode(numFloat);
            primaryNode->setType(FLOAT);
            return PostFixExprAUX(primaryNode);
        }
        case LITERAL:
        {

            TokenNode *literalString = new TokenNode(LITERAL, lexical_analyzer_last_lexeme());
            literalString->setLine(lexical_analyzer_getLine());
            literalString->setType(CHAR);
            literalString->setPointer(true);

            Eat(LITERAL);

            PrimaryNode *node = new PrimaryNode(literalString);
            node->setType(CHAR);
            node->setPointer(true); // TODO eh ponteiro ou array?

            return PostFixExprAUX(node);
        }
        case LITERALCHAR:
        {

            TokenNode *literalString = new TokenNode(LITERALCHAR, lexical_analyzer_last_lexeme());
            literalString->setLine(lexical_analyzer_getLine());
            literalString->setType(CHAR);
            
            Eat(LITERALCHAR);

            PrimaryNode *node = new PrimaryNode(literalString);
            node->setType(CHAR);

            return PostFixExprAUX(node);
        }
        case TRUE:
        {
            TokenNode *token = new TokenNode(TRUE, nullptr);
            token->setType(BOOL);
            token->setLexeme(lexical_analyzer_last_lexeme());

            Eat(TRUE);

            ExpNode *expNode = new PrimaryNode(token);
            expNode->setType(BOOL);
            expNode->setLine(lexical_analyzer_getLine());
            return PostFixExprAUX(expNode);
        }
        case FALSE:
        {
            TokenNode *token = new TokenNode(FALSE, nullptr);
            token->setType(BOOL);
            token->setLexeme(lexical_analyzer_last_lexeme());

            Eat(FALSE);

            ExpNode *expNode = new PrimaryNode(token);
            expNode->setType(BOOL);
            expNode->setLine(lexical_analyzer_getLine());
            return PostFixExprAUX(expNode);
        }
        case STAR:
        {
            Eat(STAR);

            PointerValNode *pointerValNode = new PointerValNode(ExprAssign());
            pointerValNode->setLine(lexical_analyzer_getLine());

            return PostFixExprAUX(pointerValNode);
        }
        case ADDRESS:
        {
            Eat(ADDRESS);

            AddressValNode *addressValNode = new AddressValNode(ExprAssign());
            addressValNode->setLine(lexical_analyzer_getLine());

            return PostFixExprAUX(addressValNode);
        }
        case LPARENT:
        {
            Eat(LPARENT);
            ExpNode *exp = ExprAssign();
            EatOrSkip(RPARENT, stmtAUXFollowSet); // TODO @arthur dá uma olhada se eu fiz certo aqui
            return  exp;
        }
        default:
        {
            Sync(tok, primaryFollowSet);
        }
    }
    return nullptr;
}

int Parser::postFixExprAUXFollowSet[] = {ADDRESS, STAR, SLASH, PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ,
                                         NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};
ExpNode *Parser::PostFixExprAUX(ExpNode *exp)
{
    // TODO fprintf(stdout,"POST_FIX_EXPR_AUX\n");

    switch (tok)
    {
        case DOT:
        {
            Eat(DOT);

            TokenNode *id = new TokenNode(ID, EatOrSkip(ID, postFixExprAUXFollowSet));
            id->setLine(lexical_analyzer_getLine());

            return PostFixExprAUX(new NameExpNode(exp, id));
        }
        case POINTER:
        {
            Eat(POINTER);

            TokenNode *id = new TokenNode(ID, EatOrSkip(ID, postFixExprAUXFollowSet));
            id->setLine(lexical_analyzer_getLine());

            PointerExpNode *pointerExpNode = new PointerExpNode(exp, id);
            pointerExpNode->setLine(lexical_analyzer_getLine());

            return PostFixExprAUX(pointerExpNode);
        }
        case LBRACKET:
        {
            Eat(LBRACKET);
            ExpNode *index = ExprAssign();
            EatOrSkip(RBRACKET, postFixExprAUXFollowSet);

            ArrayCallNode *arrayCallNode = new ArrayCallNode(exp, index);
            arrayCallNode->setLine(lexical_analyzer_getLine());

            return PostFixExprAUX(arrayCallNode);
        }
        default:
        {
            return exp;
        }
    }
}

int Parser::postFixExprFollowSet[] = {ADDRESS, STAR, SLASH, PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ,
                                      NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};
ExpNode *Parser::PostFixExpr(TokenNode *id)
{
    // TODO fprintf(stdout,"POST_FIX_EXPR\n");

    switch (tok)
    {
        case LPARENT:
        {
            Eat(LPARENT);
            ExpListNode *expList = ExprList();
            EatOrSkip(RPARENT, postFixExprFollowSet);

            CallNode *callNode = new CallNode(id, expList);
            callNode->setLine(lexical_analyzer_getLine());

            PrimaryNode *primary = new PrimaryNode(PostFixExprAUX(callNode));
            primary->setLine(lexical_analyzer_getLine());

            return primary;
        }
        case RPARENT:
        case DOT:
        case POINTER:
        case LBRACKET:
        {
            PrimaryNode *primaryNode = new PrimaryNode(id);

            primaryNode->setLine(lexical_analyzer_getLine());

            return PostFixExprAUX(primaryNode);
        }
        case SEMICOLON:
        default:
        {
            PrimaryNode *primaryNode = new PrimaryNode(id);

            primaryNode->setLine(lexical_analyzer_getLine());

            return primaryNode;
        }
    }
}
