//
// Created by jorao on 17/10/2019.
//

#ifndef COMPILADOR_2019_3_VISITOR_H
#define COMPILADOR_2019_3_VISITOR_H

#ifndef VISITOR_H_INCLUDED
#define VISITOR_H_INCLUDED

class ProgramNode;
class ClassListNode;
class VarDeclNode;
class IdListNode;
class TypeNode;
class FunctionDeclNode;
class TypeListNode;
class PointerNode;
class StatementListNode;
class VarStmtNode;
class VarFuncListNode;
class IfNode;
class WhileNode;
class SwitchNode;
class BreakNode;
class PrintlnNode;
class ReadNode;
class ReturnNode;
class CaseBlockNode;
class ThrowNode;
class ExpListNode;
class TryNode;
class IdentifierNode;
class IntegerNode;
class RealNode;
class LiteralNode;
class CharacterNode;
class PrimaryNode;
class FunctionCallNode;
class FieldAccessNode;
class PointerAccessNode;
class AddressValueNode;
class PointerValueNode;
class ArrayAccessNode;
class ArrayDeclNode;
class AssignNode;
class RelationalOpNode;
class AdditionOpNode;
class MultiplicationOpNode;
class BooleanOpNode;
class BitwiseOpNode;
class NewNode;
class NotNode;
class SignNode;
class TrueNode;
class FalseNode;
class ThisNode;
class FormalListNode;

class Visitor {
public:
    virtual ~Visitor() = default;;
    virtual void visit(ProgramNode *node) {};
    virtual void visit(ClassListNode *node) {};
    virtual void visit(VarDeclNode *node) {};
    virtual void visit(IdListNode *node) {};
    virtual void visit(TypeNode *node) {};
    virtual void visit(FunctionDeclNode *node) {};
    virtual void visit(TypeListNode *node) {};
    virtual void visit(PointerNode *node) {};
    virtual void visit(StatementListNode *node) {};
    virtual void visit(VarStmtNode *node) {};
    virtual void visit(VarFuncListNode *node) {};
    virtual void visit(IfNode *node) {};
    virtual void visit(WhileNode *node) {};
    virtual void visit(SwitchNode *node) {};
    virtual void visit(BreakNode *node) {};
    virtual void visit(PrintlnNode *node) {};
    virtual void visit(ReadNode *node) {};
    virtual void visit(ReturnNode *node) {};
    virtual void visit(CaseBlockNode *node) {};
    virtual void visit(ThrowNode *node) {};
    virtual void visit(ExpListNode *node) {};
    virtual void visit(TryNode *node) {};
    virtual void visit(IdentifierNode *node) {};
    virtual void visit(IntegerNode *node) {};
    virtual void visit(RealNode *node) {};
    virtual void visit(LiteralNode *node) {};
    virtual void visit(CharacterNode *node) {};
    virtual void visit(PrimaryNode *node) {};
    virtual void visit(FunctionCallNode *node) {};
    virtual void visit(FieldAccessNode *node) {};
    virtual void visit(PointerAccessNode *node) {};
    virtual void visit(AddressValueNode *node) {};
    virtual void visit(PointerValueNode *node) {};
    virtual void visit(ArrayAccessNode *node) {};
    virtual void visit(ArrayDeclNode *node) {};
    virtual void visit(AssignNode *node) {};
    virtual void visit(RelationalOpNode *node) {};
    virtual void visit(AdditionOpNode *node) {};
    virtual void visit(MultiplicationOpNode *node) {};
    virtual void visit(BooleanOpNode *node) {};
    virtual void visit(BitwiseOpNode *node) {};
    virtual void visit(NewNode *node) {};
    virtual void visit(NotNode *node) {};
    virtual void visit(SignNode *node) {};
    virtual void visit(TrueNode *node) {};
    virtual void visit(FalseNode *node) {};
    virtual void visit(ThisNode *node) {};
    virtual void visit(FormalListNode *node) {};
};

class PrintAST : public Visitor {
private:
    unsigned int level;
    void printAST(const char *node_name, const char * aux);
    void up_level();
    void down_level();

public:
    PrintAST();

    void visit(ProgramNode *node) override;
    void visit(ClassListNode *node) override;
    void visit(VarDeclNode *node) override;
    void visit(IdListNode *node) override;
    void visit(TypeNode *node) override;
    void visit(FunctionDeclNode *node) override;
    void visit(TypeListNode *node) override;
    void visit(PointerNode *node) override;
    void visit(StatementListNode *node) override;
    void visit(VarStmtNode *node) override;
    void visit(VarFuncListNode *node) override;
    void visit(IfNode *node) override;
    void visit(WhileNode *node) override;
    void visit(SwitchNode *node) override;
    void visit(BreakNode *node) override;
    void visit(PrintlnNode *node) override;
    void visit(ReadNode *node) override;
    void visit(ReturnNode *node) override;
    void visit(CaseBlockNode *node) override;
    void visit(ThrowNode *node) override;
    void visit(ExpListNode *node) override;
    void visit(TryNode *node) override;
    void visit(IdentifierNode *node) override;
    void visit(IntegerNode *node) override;
    void visit(RealNode *node) override;
    void visit(LiteralNode *node) override;
    void visit(CharacterNode *node) override;
    void visit(PrimaryNode *node) override;
    void visit(FunctionCallNode *node) override;
    void visit(FieldAccessNode *node) override;
    void visit(PointerAccessNode *node) override;
    void visit(AddressValueNode *node) override;
    void visit(PointerValueNode *node) override;
    void visit(ArrayAccessNode *node) override;
    void visit(ArrayDeclNode *node) override;
    void visit(AssignNode *node) override;
    void visit(RelationalOpNode *node) override;
    void visit(AdditionOpNode *node) override;
    void visit(MultiplicationOpNode *node) override;
    void visit(BooleanOpNode *node) override;
    void visit(BitwiseOpNode *node) override;
    void visit(NewNode *node) override;
    void visit(NotNode *node) override;
    void visit(SignNode *node) override;
    void visit(TrueNode *node) override;
    void visit(FalseNode *node) override;
    void visit(ThisNode *node) override;
    void visit(FormalListNode *node) override;
};


#endif //VISITOR_H_INCLUDED


#endif //COMPILADOR_2019_3_VISITOR_H
