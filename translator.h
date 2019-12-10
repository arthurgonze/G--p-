//
// Created by joraojr on 10/12/2019.
//

#ifndef COMPILADOR_2019_3_TRANSLATOR_H
#define COMPILADOR_2019_3_TRANSLATOR_H

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

class Translator {

public:
    virtual ~Translator() = default;
    Translator();
    virtual void visit(ProgramNode *node) ;
    virtual void visit(TypeNode *node) ;
    virtual void visit(FunctionListNode *node) ;
    virtual void visit(PointerNode *node) ;
    virtual void visit(StmtListNode *node) ;
    virtual void visit(IfNode *node) ;
    virtual void visit(WhileNode *node) ;
    virtual void visit(SwitchNode *node) ;
    virtual void visit(BreakNode *node) ;
    virtual void visit(PrintNode *node) ;
    virtual void visit(ReadLnNode *node) ;
    virtual void visit(ReturnNode *node) ;
    virtual void visit(CaseBlockNode *node) ;
    virtual void visit(ThrowNode *node) ;
    virtual void visit(ExpListNode *node) ;
    virtual void visit(TryNode *node) ;
    virtual void visit(TokenNode *node) ;
    virtual void visit(AssignNode *node) ;
    virtual void visit(ArrayNode *node) ;
    virtual void visit(CallNode *node) ;
    virtual void visit(AdditionOPNode *node) ;
    virtual void visit(MultiplicationOPNode *node) ;
    virtual void visit(BooleanOPNode *node) ;
    virtual void visit(NotNode *node) ;
    virtual void visit(SignNode *node) ;
    virtual void visit(ArrayCallNode *node) ;
    virtual void visit(FormalListNode *node) ;
    virtual void visit(IdListNode *node) ;
    virtual void visit(PrimaryNode *node) ;
    virtual void visit(VarStmtNode *node) ;
    virtual void visit(FunctionNode *node) ;
    virtual void visit(PointerExpNode *node) ;
    virtual void visit(NameExpNode *node) ;
    virtual void visit(VarDeclNode *node) ;
    virtual void visit(TypeDeclNode *node) ;
    virtual void visit(AddressValNode *node) ;
    virtual void visit(PointerValNode *node) ;
    virtual void visit(StmtNode *node) ;
};


#endif //COMPILADOR_2019_3_TRANSLATOR_H
