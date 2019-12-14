#ifndef COMPILADOR_2019_3_TRANSLATOR_H
#define COMPILADOR_2019_3_TRANSLATOR_H

#include "intermediate_code.h"

// AST NODES

class ASTNode;

class TokenNode;

class ProgramNode;

class VarDeclNode;

class IdListNode;

class FunctionListNode;

class FunctionNode;

class FormalListNode;

class TypeDeclNode;

class TypeNode;

class PointerNode;

class StmtListNode;

class StmtNode;

class IfNode;

class WhileNode;

class SwitchNode;

class BreakNode;

class PrintNode;

class ReadLnNode;

class ReturnNode;

class CaseBlockNode;

class ThrowNode;

class ExpListNode;

class TryNode;

class ExpNode;

class AssignNode;

class NameExpNode;

class PointerExpNode;

class AddressValNode;

class PointerValNode;

class ArrayNode;

class ArrayCallNode;

class CallNode;

class AdditionOPNode;

class MultiplicationOPNode;

class BooleanOPNode;

class NotNode;

class SignNode;

class VarStmtNode;

class PrimaryNode;

class VarTable;

class FunctionTable;

class StructTable;

class FunctionSymbol;

class Translator {
private:
    Fragment *fragmentList;
    VarTable *varTable;
    FunctionTable *functionTable;
    StructTable *structTable;
    FrameMIPS *currentFrame;
    FunctionSymbol *activeFunction;
public:
    Translator(VarTable *varTable, FunctionTable *functionTable, StructTable *structTable);

    ~Translator() = default;

    void printFragmentList();

    inline Fragment *getFragmentList() const { return fragmentList; }

    void visit(ProgramNode *programNode);

    void visit(TypeNode *typeNode);

    void visit(FunctionListNode *functionListNode);

    void visit(PointerNode *pointerNode);

    void visit(VarDeclNode *varDeclNode);

    void visit(TypeDeclNode *typeDeclNode);

    void visit(CaseBlockNode *caseBlockNode);

    void visit(FormalListNode *formalListNode);

    void visit(IdListNode *idListNode);

    void visit(VarStmtNode *varStmtNode);

    void visit(FunctionNode *functionNode);

//    StmNode * visit(StmtNode *stmtNode);

    StmNode *visit(StmtListNode *stmtListNode);

    StmNode *visit(IfNode *ifNode);

    StmNode *visit(WhileNode *whileNode);

    StmNode *visit(SwitchNode *switchNode);

    StmNode *visit(BreakNode *breakNode);

    StmNode *visit(PrintNode *printNode);

    StmNode *visit(ReadLnNode *readLnNode);

    StmNode *visit(ReturnNode *returnNode);

    StmNode *visit(ThrowNode *throwNode);

    ExprNode *visit(ExpListNode *expListNode);

    StmNode *visit(TryNode *tryNode);

    ExprNode *visit(TokenNode *tokenNode);

    ExprNode *visit(AssignNode *assignNode);

    ExprNode *visit(ArrayNode *arrayNode);

    ExprNode *visit(CallNode *callNode);

    ExprNode *visit(AdditionOPNode *additionOPNode);

    ExprNode *visit(MultiplicationOPNode *multiplicationOPNode);

    ExprNode *visit(BooleanOPNode *booleanOPNode);

    ExprNode *visit(NotNode *notNode);

    ExprNode *visit(SignNode *signNode);

    ExprNode *visit(ArrayCallNode *arrayCallNode);

    ExprNode *visit(PrimaryNode *primaryNode);

    ExprNode *visit(PointerExpNode *pointerExpNode);

    ExprNode *visit(NameExpNode *nameExpNode);

    ExprNode *visit(AddressValNode *addressValNode);

    ExprNode *visit(PointerValNode *pointerValNode);

//    ExprNode* visit(ExpNode *expNode);

};

void startTranslator(ProgramNode *ast, Translator *translator);

void endTranslator(Translator *translator);


#endif //COMPILADOR_2019_3_TRANSLATOR_H
