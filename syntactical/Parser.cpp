#include "Parser.h"
#include "Visitor.h"

PrintAST *visitor;
ProgramNode *program;

ProgramNode *Parser::StartParser()
{
    Advance();
    fprintf(stdout, "PROGRAM\n");
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
        //print_syntactic_error(expected_token, -1); ///generic error
        fprintf(stderr, "[SYNTAX ERROR] Error(EatOrSkip), Token error: Esperado: %s, Processado: %s \n", token_id_to_name(expectedToken), token_id_to_name(tok)); // TODO conferir
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
            //printf("-->ERROR");
        }
        return nullptr;
    }
}

/**
* Skips all tokens until it finds one that is in the sync set.
*/
void Parser::Sync(int token, const int *syncSet)
{
    // TODO print_syntactic_error(-1, error_id); /// specific error
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
            fprintf(stdout, "MATCH - %s.%s\n", token_id_to_name(t), lexical_analyzer_last_lexeme());
        }
        else
        {
            fprintf(stdout, "MATCH - %s\n", token_id_to_name(t));
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

            EatOrSkip(RBRACE, programFollowSet);

            TokenNode *id = new TokenNode(ID, EatOrSkip(ID, programFollowSet));

            EatOrSkip(SEMICOLON, programFollowSet);
            typeList = new TypeDeclNode(varListNode, id, typeList);
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

            ASTNode *ast = ProgramAUX(type, pointer, id, varList);
            if (dynamic_cast<FunctionNode *>(ast))
            {
                FunctionNode *f = (FunctionNode *) ast;
                functionList = new FunctionListNode(f, functionList);
                return ProgramList(functionList, typeList, varList);

            }
            varList = (VarDeclNode *) ast;
            return Program(functionList, typeList, varList);
        }
        case ENDOFFILE:
        {
            Eat(ENDOFFILE);
            return new ProgramNode(functionList, typeList, varList);
        }
        default:
        {
            fprintf(stderr, "[SYNTAX ERROR] error(Program), Token error: %s \n", token_id_to_name(tok));
            return new ProgramNode(functionList, typeList, varList);
        }
    }

}

int Parser::programAUXFollowSet[] = {ENDOFFILE, '\0'};
ASTNode *Parser::ProgramAUX(TypeNode *type, PointerNode *pointer, TokenNode *id, VarDeclNode *varFunc)
{
    fprintf(stdout,"PROGRAM_AUX\n");
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

            EatOrSkip(RBRACE, programAUXFollowSet);
            return func;
        }
        case COMMA:
        case LBRACE:
        {
            ArrayNode *array = Array();
            IdListNode *idList = new IdListNode(pointer, id, array, IdListAUX());

            EatOrSkip(SEMICOLON, programAUXFollowSet);

            VarDeclNode *list = new VarDeclNode(type, idList, varFunc);
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
    fprintf(stdout,"PROGRAM_LIST\n");
    switch (tok)
    {
        case ENDOFFILE:
        {
            Eat(ENDOFFILE);
            return new ProgramNode(functions, typelist, varlist);
        }
        default:
        {
            //fprintf(stderr, "[SYNTAX ERROR] errorProgramList(), Token error: %s \n", token_id_to_name(tok));
            return Program(functions, typelist, varlist);
        }
    }
}

int Parser::typeDeclFollowSet[] = {INT, FLOAT, BOOL, ID, CHAR, TYPEDEF, ENDOFFILE, '\0'};
TypeDeclNode *Parser::TypeDecl(TypeDeclNode *typeListNode)
{
    fprintf(stdout,"TYPE_DECL\n");
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
            EatOrSkip(RBRACE, typeDeclFollowSet);

            TokenNode *id = new TokenNode(ID, EatOrSkip(ID, typeDeclFollowSet));

            EatOrSkip(SEMICOLON, typeDeclFollowSet);
            typeListNode = new TypeDeclNode(varListNode, id, typeListNode);
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
    fprintf(stdout,"VAR_DECL\n");
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
    fprintf(stdout,"VAR_STMT\n");
    switch (tok)
    {
        case ID:
        {
            TokenNode *id = new TokenNode(ID,  EatOrSkip(ID, varStmtFollowSet));
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
            return new VarStmtNode(varList, StmtList());
        }
        default:
        {
            fprintf(stderr, "[SYNTAX ERROR] error(VarStmt), Token error: %s \n", token_id_to_name(tok));
            return new VarStmtNode(varList, nullptr);
        }
    }
}

int Parser::varStmtAuxFollowSet[] = {RBRACE, ENDOFFILE, '\0'};
VarStmtNode *Parser::VarStmtAux(TokenNode *id, VarDeclNode *varList)
{
    fprintf(stdout,"VAR_STMT_AUX\n");
    switch (tok)
    {
        case STAR:
        case ID:
        {
            IdListNode *idListNode = IdList();
            EatOrSkip(SEMICOLON, varStmtAuxFollowSet);
            varList = new VarDeclNode(new TypeNode(id), idListNode, varList);
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
            StmtListNode *stmtList = new StmtListNode(new StmtNode(exp), StmtListAUX());
            VarStmtNode *varStmtNode = new VarStmtNode(varList, stmtList);
            return varStmtNode;
        }
        default:
        {
            return new VarStmtNode(varList, nullptr);
        }
    }
}

int Parser::idListFollowSet[] = {SEMICOLON, RPARENT, ENDOFFILE, '\0'};// TODO talvez remover rparent
IdListNode *Parser::IdList()
{
    fprintf(stdout,"ID_LIST\n");
    PointerNode *pointer = Pointer();
    switch (tok)
    {
        case ID:
        {
            TokenNode *id = new TokenNode(ID, EatOrSkip(ID, idListFollowSet));
            ArrayNode *array = Array();
            IdListNode *idListNode = new IdListNode(pointer, id, array, IdListAUX());
            return idListNode;
        }
        default:
        {
            fprintf(stderr, "[SYNTAX ERROR] error(IdList), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::idListAUXFollowSet[] = {SEMICOLON, RPARENT, ENDOFFILE, '\0'};// TODO talvez remover rparent
IdListNode *Parser::IdListAUX()
{
    fprintf(stdout,"ID_LIST_AUX\n");
    switch (tok)
    {
        case COMMA:
        {
            Eat(COMMA);
            PointerNode *pointer = Pointer();

            TokenNode *id = new TokenNode(ID, EatOrSkip(ID, idListAUXFollowSet));

            ArrayNode *array = Array();
            IdListNode *idListNode = new IdListNode(pointer, id, array, IdListAUX());
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
    fprintf(stdout,"POINTER\n");
    switch (tok)
    {
        case STAR:
        {
            Eat(STAR);
            return new PointerNode();
        }
        default:
        {
            return nullptr;
        }
    }
}

int Parser::arrayFollowSet[] = {SEMICOLON, COMMA, RPARENT, ENDOFFILE, '\0'};// TODO talvez remover rparent
ArrayNode *Parser::Array()
{
    fprintf(stdout,"ARRAY\n");
    switch (tok)
    {
        case LBRACKET:
        {
            Eat(LBRACKET);

            TokenNode *numInt = new TokenNode(NUMINT, EatOrSkip(NUMINT, arrayFollowSet));

            EatOrSkip(RBRACKET, arrayFollowSet);
            return new ArrayNode(numInt);
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
    fprintf(stdout, "FORMAL_LIST\n");

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

            ArrayNode *array = Array();
            FormalListNode *list = new FormalListNode(type, pointer, id, array, FormalRest());
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
    fprintf(stdout,"FORMAL_REST\n");
    switch (tok)
    {
        case COMMA:
        {
            Eat(COMMA);
            TypeNode *typeNode = Type();
            PointerNode *pointer = Pointer();

            TokenNode *id = new TokenNode(ID, EatOrSkip(ID, formalRestFollowSet));

            ArrayNode *array = Array();
            FormalListNode *formalList = new FormalListNode(typeNode, pointer, id, array, FormalRest());
            return formalList;
        }
        default:
        {
            return nullptr;
        }
    }
}

int Parser::typeFollowSet[] = {LPARENT, STAR, ID, ENDOFFILE, '\0'};// TODO talvez remover LPARENT
TypeNode *Parser::Type()
{
    fprintf(stdout,"TYPE\n");
    switch (tok)
    {
        case INT:
        {
            TokenNode *type = new TokenNode(INT, nullptr);
            Eat(INT);
            return new TypeNode(type);
        }
        case FLOAT:
        {
            TokenNode *type = new TokenNode(FLOAT, nullptr);
            Eat(FLOAT);
            return new TypeNode(type);
        }
        case BOOL:
        {
            TokenNode *type = new TokenNode(BOOL, nullptr);
            Eat(BOOL);
            return new TypeNode(type);
        }
        case ID:
        {
            TokenNode *type = new TokenNode(ID, lexical_analyzer_last_lexeme());
            Eat(ID);
            return new TypeNode(type);
        }
        case CHAR:
        {
            TokenNode *type = new TokenNode(CHAR, nullptr);
            Eat(CHAR);
            return new TypeNode(type);
        }
        default:
        {
            fprintf(stderr, "[SYNTAX ERROR] error(Type), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::typeAuxFollowSet[] = {LPARENT, STAR, ID, ENDOFFILE, '\0'};// TODO talvez remover rparent
TypeNode *Parser::TypeAux()
{
    fprintf(stdout,"TYPE_AUX\n");
    switch (tok)
    {
        case INT:
        {
            TokenNode *type = new TokenNode(INT, nullptr);
            Eat(INT);
            return new TypeNode(type);
        }
        case FLOAT:
        {
            TokenNode *type = new TokenNode(FLOAT, nullptr);
            Eat(FLOAT);
            return new TypeNode(type);
        }
        case BOOL:
        {
            TokenNode *type = new TokenNode(BOOL, nullptr);
            Eat(BOOL);
            return new TypeNode(type);
        }
        case CHAR:
        {
            TokenNode *type = new TokenNode(CHAR, nullptr);
            Eat(CHAR);
            return new TypeNode(type);
        }
        default:
        {
            fprintf(stderr, "[SYNTAX ERROR] error(TypeAUX), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::stmtListFollowSet[] = {RBRACE, CASE, ENDOFFILE, '\0'};
StmtListNode *Parser::StmtList()
{
    fprintf(stdout,"STMT_LIST\n");
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
            return stmtListNode;
        }
        default:
        {
            fprintf(stderr, "[SYNTAX ERROR] error(StmtList), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::stmtListAUXFollowSet[] = {RBRACKET, CASE, ENDOFFILE, '\0'};
StmtListNode *Parser::StmtListAUX()
{
    fprintf(stdout,"STMT_LIST_AUX\n");
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
                               NUMINT, NUMFLOAT, LITERAL, LITERALCHAR, LPARENT,  ADDRESS, STAR, TRUE, FALSE, RBRACE, CASE, ENDOFFILE, '\0'};// TODO talevez tirar RBRACE, CASE
StmtNode *Parser::Stmt()
{
    fprintf(stdout,"STMT\n");
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
            return new StmtNode(stmt);
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
            fprintf(stderr, "[SYNTAX ERROR] error(Stmt), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

// CATCH, ELSE, IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, ABRE_CHAVE, TRY, NEGACAO, MAIS, MENOS, ID, NUM_INT, NUM_REAL,
// LITERAL, ABRE_PARENTESES, E_BIT, MULTIPLICACAO, TRUE_TOKEN, FALSE_TOKEN, TOKEN_EOF
int Parser::stmtAUXFollowSet[] = {CATCH, ELSE, IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACE, TRY, NOT, PLUS, MINUS, ID, NUMINT,
                                  NUMFLOAT, LITERAL, LITERALCHAR, LPARENT, ADDRESS, STAR, TRUE, FALSE, RBRACE, CASE, ENDOFFILE, '\0'}; // TODO talvez tirar RBRACE e CASE
StmtNode *Parser::StmtAUX()
{
    fprintf(stdout,"STMT_AUX\n");
    switch (tok)
    {
        case WHILE:
        {
            Eat(WHILE);

            EatOrSkip(LPARENT, stmtAUXFollowSet);
            ExpNode *exp = ExprAssign();
            EatOrSkip(RPARENT, stmtAUXFollowSet);

            WhileNode *aux = new WhileNode(exp, Stmt());
            return new StmtNode(aux);
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
            return new StmtNode(aux);
        }
        case BREAK:
        {
            Eat(BREAK);
            BreakNode *breakNode = new BreakNode();
            EatOrSkip(SEMICOLON, stmtAUXFollowSet);
            return new StmtNode(breakNode);
        }
        case PRINT:
        {
            Eat(PRINT);

            EatOrSkip(LPARENT, stmtAUXFollowSet);
            ExpListNode *expList = ExprList();
            EatOrSkip(RPARENT, stmtAUXFollowSet);

            EatOrSkip(SEMICOLON, stmtAUXFollowSet);
            PrintNode *print = new PrintNode(expList);
            return new StmtNode(print);
        }
        case READLN:
        {
            Eat(READLN);

            EatOrSkip(LPARENT, stmtAUXFollowSet);
            ExpNode *exp = ExprAssign();
            EatOrSkip(RPARENT, stmtAUXFollowSet);

            EatOrSkip(SEMICOLON, stmtAUXFollowSet);
            return new StmtNode(new ReadLnNode(exp));
        }
        case RETURN:
        {
            Eat(RETURN);
            ExpNode *exp = ExprAssign();
            EatOrSkip(SEMICOLON, stmtAUXFollowSet);
            return new StmtNode(new ReturnNode(exp));
        }
        case THROW:
        {
            Eat(THROW);
            EatOrSkip(SEMICOLON, stmtAUXFollowSet);
            return new StmtNode(new ThrowNode);
        }
        case LBRACE:
        {
            Eat(LBRACE);
            StmtListNode *stmtList = StmtList();
            EatOrSkip(RBRACE, stmtAUXFollowSet);
            return new StmtNode(stmtList);
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
            return new StmtNode(new TryNode(tryStmt, exception));
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
            return new StmtNode(exp);
        }
        default:
        {
            fprintf(stderr, "[SYNTAX ERROR] error(StmtAUX), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::elseStmtFollowSet[] = {RBRACKET, ENDOFFILE, '\0'};
StmtNode *Parser::ElseStmt()
{
    fprintf(stdout,"ELSE_STMT\n");
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
    fprintf(stdout,"CASE_BLOCK\n");
    switch (tok)
    {
        case CASE:
        {
            Eat(CASE);

            TokenNode *token = new TokenNode(NUMINT, lexical_analyzer_last_lexeme());
            EatOrSkip(NUMINT, caseBlockFollowSet);
            EatOrSkip(COLON, caseBlockFollowSet);
            return CaseBlockAUX(token);
        }
        default:
        {
            fprintf(stderr, "[SYNTAX ERROR] error(caseBlock), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::caseBlockAUXFollowSet[] = {RBRACE, ENDOFFILE, '\0'};
CaseBlockNode *Parser::CaseBlockAUX(TokenNode *num)
{
    fprintf(stdout,"CASE_BLOCK_AUX\n");
    switch (tok)
    {
        case CASE:
        {
            return CaseBlock();
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
            return new CaseBlockNode(num, StmtList(), CaseBlock());
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
    fprintf(stdout,"EXPR_LIST\n");

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
    fprintf(stdout,"EXPR_LIST_TAIL\n");
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
            return new ExpListNode(ExprAssign(), ExprListTailAUX());
        }
        default:
        {
            fprintf(stderr, "[SYNTAX ERROR] error(ExprListTail), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::exprListTailAUXFollowSet[] = {RPARENT, ENDOFFILE, '\0'};
ExpListNode *Parser::ExprListTailAUX()
{
    fprintf(stdout,"EXPR_LIST_AUX\n");
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

int Parser::exprAssignFollowSet[] = {SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};// TODO talvez tirar COMMA e RBRACE
ExpNode *Parser::ExprAssign()
{
    fprintf(stdout,"EXPR_ASSING\n");
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
            fprintf(stderr, "[SYNTAX ERROR] error(ExprAssign), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::exprAssignAUXFollowSet[] = {SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};
ExpNode *Parser::ExprAssignAUX(ExpNode *expr)
{
    fprintf(stdout,"EXPR_ASSING_AUX\n");
    ExpNode *exp2 = nullptr;
    ExpNode *node = expr;
    switch (tok)
    {
        case ASSIGN:
        {
            Eat(ASSIGN);
            exp2 = ExprAssign();
            node = new AssignNode(expr, exp2);
            break;
        }
            // Epsilon default: fprintf(stderr, "[SYNTAX ERROR] error(ExprAssignAUX), Token error: %s \n",token_id_to_name(tok));
    }
    return node;
}

int Parser::exprOrFollowSet[] = {EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};// TODO talvez tirar SEMICOLON, COMMA, RPARENT, RBRACE
ExpNode *Parser::ExprOr()
{
    fprintf(stdout,"EXPR_OR\n");

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
            fprintf(stderr, "[SYNTAX ERROR] error(ExprOr), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
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
            exp2 = ExprOr();
            node = new BooleanOPNode(token, expr, exp2);
            break;
        }
            // Epsilon default: fprintf(stderr, "[SYNTAX ERROR] error(ExprOrAUX), Token error: %s \n",token_id_to_name(tok));
    }
    return node;
}

int Parser::exprAndFollowSet[] = {OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};// TODO talvez tirar  EQ, SEMICOLON, COMMA, RPARENT, RBRACE
ExpNode *Parser::ExprAnd()
{
    fprintf(stdout,"EXPR_AND\n");
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
            fprintf(stderr, "[SYNTAX ERROR] error(ExprAnd), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::exprAndAUXFollowSet[] = {OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};
ExpNode *Parser::ExprAndAUX(ExpNode *expr)
{
    fprintf(stdout,"EXPR_AND_AUX\n");

    TokenNode *token = nullptr;
    ExpNode *exp2 = nullptr;
    ExpNode *node = expr;
    switch (tok)
    {
        case AND:
        {
            Eat(AND);
            token = new TokenNode(AND, nullptr);
            exp2 = ExprAnd();
            node = new BooleanOPNode(token, expr, exp2);
            break;
        }
            // Epsilon default: fprintf(stderr, "[SYNTAX ERROR] error(ExprAndAUX), Token error: %s \n",token_id_to_name(tok));
    }
    return node;
}

int Parser::exprEqualityFollowSet[] = {AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};// TODO talvez tirar  OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE
ExpNode *Parser::ExprEquality()
{
    fprintf(stdout,"EXPR_EQUALITY\n");

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
            fprintf(stderr, "[SYNTAX ERROR] error(ExprEquality), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::exprEqualityAUXFollowSet[] = {AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};
ExpNode *Parser::ExprEqualityAUX(ExpNode *expr)
{
    fprintf(stdout,"EXPR_EQUALITY_AUX\n");

    TokenNode *token = nullptr;
    ExpNode *exp2 = nullptr;
    ExpNode *node = expr;
    switch (tok)
    {
        case EQ:
        {
            Eat(EQ);
            token = new TokenNode(EQ, nullptr);
            exp2 = ExprEquality();
            node = new BooleanOPNode(token, expr, exp2);
            break;
        }
        case NE:
        {
            Eat(NE);
            token = new TokenNode(NE, nullptr);
            exp2 = ExprEquality();
            node = new BooleanOPNode(token, expr, exp2);
            break;
        }
            // Epsilon default: fprintf(stderr, "[SYNTAX ERROR] error(ExprEqualityAUX), Token error: %s \n",token_id_to_name(tok));
    }
    return node;
}

int Parser::exprRelationalFollowSet[] = {EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};// TODO talvez tirar AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE
ExpNode *Parser::ExprRelational()
{
    fprintf(stdout,"EXPR_RELATIONAL\n");

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
            fprintf(stderr, "[SYNTAX ERROR] error(ExprRelational), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::exprRelationalAUXFollowSet[] = {EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};
ExpNode *Parser::ExprRelationalAUX(ExpNode *expr)
{
    fprintf(stdout,"EXPR_RELATIONAL_AUX\n");

    TokenNode *token = nullptr;
    ExpNode *exp2 = nullptr;
    ExpNode *node = expr;
    switch (tok)
    {
        case LT:
        {
            Eat(LT);
            token = new TokenNode(LT, nullptr);
            exp2 = ExprRelational();
            node = new BooleanOPNode(token, expr, exp2);
            break;
        }
        case GT:
        {
            Eat(GT);
            token = new TokenNode(GT, nullptr);
            exp2 = ExprRelational();
            node = new BooleanOPNode(token, expr, exp2);
            break;
        }
        case LE:
        {
            Eat(LE);
            token = new TokenNode(LE, nullptr);
            exp2 = ExprRelational();
            node = new BooleanOPNode(token, expr, exp2);
            break;
        }
        case GE:
        {
            Eat(GE);
            token = new TokenNode(GE, nullptr);
            exp2 = ExprRelational();
            node = new BooleanOPNode(token, expr, exp2);
            break;
        }
            // Epsilon default: fprintf(stderr, "[SYNTAX ERROR] error(ExprRelationalAUX), Token error: %s \n",token_id_to_name(tok));
    }
    return node;
}

int Parser::exprAdditiveFollowSet[] = {LT, GT, LE, GE, EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'}; // TODO talvez tirar EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE
ExpNode *Parser::ExprAdditive()
{
    fprintf(stdout,"EXPR_ADDITIVE\n");

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
            fprintf(stderr, "[SYNTAX ERROR] error(ExprAdditive), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::exprAdditiveAUXFollowSet[] = {LT, GT, LE, GE, EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};
ExpNode *Parser::ExprAdditiveAUX(ExpNode *expr)
{
    fprintf(stdout,"EXPR_ADDITIVE_AUX\n");

    TokenNode *token = nullptr;
    ExpNode *exp2 = nullptr;
    ExpNode *node = expr;
    switch (tok)
    {
        case PIPE:
        {
            Eat(PIPE);
            token = new TokenNode(PIPE, nullptr);
            exp2 = ExprAdditive();
            node = new AdditionOPNode(token, expr, exp2);
            break;
        }
        case PLUS:
        {
            Eat(PLUS);
            token = new TokenNode(PLUS, nullptr);
            exp2 = ExprAdditive();
            node = new AdditionOPNode(token, expr, exp2);
            break;
        }
        case MINUS:
        {
            Eat(MINUS);
            token = new TokenNode(MINUS, nullptr);
            exp2 = ExprAdditive();
            node = new AdditionOPNode(token, expr, exp2);
            break;
        }
            // Epsilon default: fprintf(stderr, "[SYNTAX ERROR] error(ExprAdditiveAUX), Token error: %s \n",token_id_to_name(tok));
    }
    return node;
}

int Parser::exprMultiplicativeFollowSet[] = {PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};// TODO talvez tirar  LT, GT, LE, GE, EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE,
ExpNode *Parser::ExprMultiplicative()
{
    fprintf(stdout,"EXPR_MULTIPLICATIVE\n");

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
            fprintf(stderr, "[SYNTAX ERROR] error(ExprMultiplicative), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::exprMultiplicativeAUXFollowSet[] = {PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};
ExpNode *Parser::ExprMultiplicativeAUX(ExpNode *expr)
{
    fprintf(stdout,"EXPR_MULTIPLICATIVE_AUX\n");

    TokenNode *token = nullptr;
    ExpNode *exp2 = nullptr;
    ExpNode *node = expr;
    switch (tok)
    {
        case ADDRESS:
        {
            Eat(ADDRESS);
            token = new TokenNode(ADDRESS, nullptr);
            exp2 = ExprMultiplicative();
            node = new MultiplicationOPNode(token, expr, exp2);
            break;
        }
        case STAR:
        {
            Eat(STAR);
            token = new TokenNode(STAR, nullptr);
            exp2 = ExprMultiplicative();
            node = new MultiplicationOPNode(token, expr, exp2);
            break;
        }
        case SLASH:
        {
            Eat(SLASH);
            token = new TokenNode(SLASH, nullptr);
            exp2 = ExprMultiplicative();
            node = new MultiplicationOPNode(token, expr, exp2);
            break;
        }
        case PERCENT:
        {
            Eat(PERCENT);
            token = new TokenNode(PERCENT, nullptr);
            exp2 = ExprMultiplicative();
            node = new MultiplicationOPNode(token, expr, exp2);
            break;
        }
            // Epsilon default: fprintf(stderr, "[SYNTAX ERROR] error(ExprMultiplicativeAUX), Token error: %s \n",token_id_to_name(tok));
    }
    return node;
}

int Parser::exprUnaryFollowSet[] = {ADDRESS, STAR, SLASH, PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};
ExpNode *Parser::ExprUnary()
{
    fprintf(stdout,"EXPR_UNARY\n");

    switch (tok)
    {
        case NOT:
        {
            Eat(NOT);
            return new SignNode(ExprUnary());
        }
        case PLUS:
        {
            Eat(PLUS);
            return ExprUnary();
        }
        case MINUS:
        {
            Eat(MINUS);
            return new SignNode(ExprUnary());
        }
        case STAR:
        {
            Eat(STAR);
            return new PointerValNode(ExprUnary());
        }
        case ADDRESS:
        {
            Eat(ADDRESS);
            return new AddressValNode(ExprUnary());
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
            fprintf(stderr, "[SYNTAX ERROR] error(ExprUnary), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::primaryFollowSet[] = {DOT, POINTER, LBRACE, LPARENT, ADDRESS, STAR, SLASH, PIPE, PLUS, MINUS, LT, GT, LE, GE,
                                  EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};
ExpNode *Parser::Primary()
{
    fprintf(stdout,"PRIMARY\n");
    switch (tok)
    {
        case ID:
        {
            TokenNode *id = new TokenNode(ID, lexical_analyzer_last_lexeme());
            Eat(ID);
            return PostFixExpr(id);
        }
        case NUMINT:
        {
            TokenNode *numInt = new TokenNode(NUMINT, lexical_analyzer_last_lexeme());
            Eat(NUMINT);
            return PostFixExprAUX(new PrimaryNode(numInt));
        }
        case NUMFLOAT:
        {
            TokenNode *numFloat = new TokenNode(NUMFLOAT, lexical_analyzer_last_lexeme());
            Eat(NUMFLOAT);
            return PostFixExprAUX(new PrimaryNode(numFloat));
        }
        case LITERAL:
        {

            TokenNode *literalString = new TokenNode(LITERAL, lexical_analyzer_last_lexeme());
            Eat(LITERAL);
            return PostFixExprAUX(new PrimaryNode(literalString));
        }
        case LITERALCHAR:
        {

            TokenNode *literalString = new TokenNode(LITERALCHAR, lexical_analyzer_last_lexeme());
            Eat(LITERALCHAR);
            return PostFixExprAUX(new PrimaryNode(literalString));
        }
        case TRUE:
        {
            Eat(TRUE);
            ExpNode *expNode = new PrimaryNode(new TokenNode(TRUE, nullptr));
            return PostFixExprAUX(expNode);
        }
        case FALSE:
        {
            Eat(FALSE);
            ExpNode *expNode = new PrimaryNode(new TokenNode(FALSE, nullptr));
            return PostFixExprAUX(expNode);
            break;
        }
        case STAR:
        {
            Eat(STAR);
            return PostFixExprAUX(new PointerValNode(ExprAssign()));
        }
        case ADDRESS:
        {
            Eat(ADDRESS);
            return PostFixExprAUX(new AddressValNode(ExprAssign()));
        }
        default:
        {
            fprintf(stderr, "[SYNTAX ERROR] error(Primary), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::postFixExprAUXFollowSet[] = {ADDRESS, STAR, SLASH, PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ,
                                         NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};
ExpNode *Parser::PostFixExprAUX(ExpNode *exp)
{
    fprintf(stdout,"POST_FIX_EXPR_AUX\n");

    switch (tok)
    {
        case DOT:
        {
            Eat(DOT);
            TokenNode *id = new TokenNode(ID, EatOrSkip(ID, postFixExprAUXFollowSet));
            return PostFixExprAUX(new NameExpNode(exp, id));
        }
        case POINTER:
        {
            Eat(POINTER);
            TokenNode *id = new TokenNode(ID, EatOrSkip(ID, postFixExprAUXFollowSet));
            return PostFixExprAUX(new PointerExpNode(exp, id));
        }
        case LBRACKET:
        {
            Eat(LBRACKET);
            ExpNode *index = ExprAssign();
            EatOrSkip(RBRACKET, postFixExprAUXFollowSet);
            return PostFixExprAUX(new ArrayCallNode(exp, index));
        }
        default:
        {
//            fprintf(stderr, "[SYNTAX ERROR] error(PostFixExprAUX), Token error: %s \n", token_id_to_name(tok));
            return exp;
        }
    }
}

int Parser::postFixExprFollowSet[] = {ADDRESS, STAR, SLASH, PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ,
                                      NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE, ENDOFFILE, '\0'};
ExpNode *Parser::PostFixExpr(TokenNode *id)
{
    fprintf(stdout,"POST_FIX_EXPR\n");

    switch (tok)
    {
        case LPARENT:
        {
            Eat(LPARENT);
            ExpListNode *expList = ExprList();
            EatOrSkip(RPARENT, postFixExprFollowSet);
            CallNode *callNode = new CallNode(id, expList);
            PrimaryNode *primary = new PrimaryNode(PostFixExprAUX(callNode));
            return primary;
        }
        case RPARENT:
        case SEMICOLON:
        case DOT:
        case POINTER:
        case LBRACKET:
        {
            return PostFixExprAUX(new PrimaryNode(id));
        }
        default:
        {
            fprintf(stderr, "[SYNTAX ERROR] error(PostFixExpr), Token error: %s \n", token_id_to_name(tok));
            return new PrimaryNode(id);
        }
    }
}
