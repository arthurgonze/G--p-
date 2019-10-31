#ifndef COMPILADOR_2019_3_VISITOR_H
#define COMPILADOR_2019_3_VISITOR_H

#ifndef VISITOR_H_INCLUDED
#define VISITOR_H_INCLUDED

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

class Visitor
{
public:
    virtual ~Visitor() = default;;
    virtual void visit(ProgramNode *node) = 0;
    virtual void visit(TypeNode *node) = 0;
    virtual void visit(FunctionListNode *node) = 0;
    virtual void visit(PointerNode *node) = 0;
    virtual void visit(StmtListNode *node) = 0;
    virtual void visit(IfNode *node) = 0;
    virtual void visit(WhileNode *node) = 0;
    virtual void visit(SwitchNode *node) = 0;
    virtual void visit(BreakNode *node) = 0;
    virtual void visit(PrintNode *node) = 0;
    virtual void visit(ReadLnNode *node) = 0;
    virtual void visit(ReturnNode *node) = 0;
    virtual void visit(CaseBlockNode *node) = 0;
    virtual void visit(ThrowNode *node) = 0;
    virtual void visit(ExpListNode *node) = 0;
    virtual void visit(TryNode *node) = 0;
    virtual void visit(TokenNode *node) = 0;
    virtual void visit(AssignNode *node) = 0;
    virtual void visit(ArrayNode *node) = 0;
    virtual void visit(CallNode *node) = 0;
    virtual void visit(AdditionOPNode *node) = 0;
    virtual void visit(MultiplicationOPNode *node) = 0;
    virtual void visit(BooleanOPNode *node) = 0;
    virtual void visit(NotNode *node) = 0;
    virtual void visit(SignNode *node) = 0;
    virtual void visit(ArrayCallNode *node) = 0;
    virtual void visit(FormalListNode *node) = 0;
    virtual void visit(IdListNode *node) = 0;
    virtual void visit(PrimaryNode *node) = 0;
    virtual void visit(VarStmtNode *node) = 0;
    virtual void visit(FunctionNode *node) = 0;
    virtual void visit(PointerExpNode *node) = 0;
    virtual void visit(NameExpNode *node) = 0;
    virtual void visit(VarDeclNode *node) = 0;
    virtual void visit(TypeDeclNode *node) = 0;
    virtual void visit(AddressValNode *node) = 0;
    virtual void visit(PointerValNode *node) = 0;
    virtual void visit(StmtNode *node) = 0;

};

class PrintAST : public Visitor
{
private:
    unsigned int level;
    void printAST(const char *node_name, const char * aux);
    void up_level();
    void down_level();

public:
    PrintAST();

    void visit(ProgramNode *node) override;
    void visit(TypeNode *node) override;
    void visit(FunctionListNode *node) override;
    void visit(PointerNode *node) override;
    void visit(StmtListNode *node) override;
    void visit(IfNode *node) override;
    void visit(WhileNode *node) override;
    void visit(SwitchNode *node) override;
    void visit(BreakNode *node) override;
    void visit(PrintNode *node) override;
    void visit(ReadLnNode *node) override;
    void visit(ReturnNode *node) override;
    void visit(CaseBlockNode *node) override;
    void visit(ThrowNode *node) override;
    void visit(ExpListNode *node) override;
    void visit(TryNode *node) override;
    void visit(TokenNode *node) override;
    void visit(AssignNode *node) override;
    void visit(ArrayNode *node) override;
    void visit(CallNode *node) override;
    void visit(AdditionOPNode *node) override;
    void visit(MultiplicationOPNode *node) override;
    void visit(BooleanOPNode *node) override;
    void visit(NotNode *node) override;
    void visit(SignNode *node) override;
    void visit(ArrayCallNode *node)override;
    void visit(FormalListNode *node)override;
    void visit(IdListNode *node)override;
    void visit(PrimaryNode *node)override;
    void visit(VarStmtNode *node)override;
    void visit(FunctionNode *node) override;
    void visit(PointerExpNode *node) override;
    void visit(NameExpNode *node) override;

    // TODO
    void visit(VarDeclNode *node) override;
    void visit(TypeDeclNode *node) override;
    void visit(AddressValNode *node) override;
    void visit(PointerValNode *node) override;
    void visit(StmtNode *node) override;
};

#endif //VISITOR_H_INCLUDED

#endif //COMPILADOR_2019_3_VISITOR_H
