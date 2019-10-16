#ifndef COMPILADOR_2019_3_AST_H
#define COMPILADOR_2019_3_AST_H

// Foward Declarations
class Node;

// implements node
class StatementNode;
class TypeNode;
class VarDeclNode;
class IdListNode;
class VarStmtNode;
class VarFuncListNode;
class CaseBlockNode;
class FunctionDeclNode;
class ClassListNode;
class TypeListNode;
class ProgramNode;
class PointerNode;
class FormalListNode;

// implements statementNode
class ExpNode;
class IfNode;
class WhileNode;
class SwitchNode;
class BreakNode;
class PrintNode;
class ReadNode;
class ReturnNode;
class ThrowNode;
class TryNode;
class StatementListNode;

// implements expNode
class ExpListNode;
class IdentifierNode;
class IntegerNode;
class RealNode;
class LiteralNode;
class CharacterNode;
class AssignNode;
class PrimaryNode;
class FunctionCallNode;
class PointerValueExpNode;
class AddressValueNode;
class PointerValueNode;
class ArrayAccessNode;
class ArrayDeclNode;
class NewNode;
class RelationalOpNode;
class AdditionOpNode;
class MultiplicationOpNode;
class BooleanOpNode;
class BitwiseOpNode;
class TrueNode;
class FalseNode;
class ThisNode;
class NotNode;
class SignNode;

/// Base abstract node class.
class Node {
public:
    virtual ~Node() = default;
    // TODO virtual void accept(Visitor *visitor) = 0;
};

// Inherits from class Node. Abstract statement node class.
class StatementNode : public Node {
public:
    virtual ~StatementNode() = default;
    // TODO virtual void accept(Visitor *visitor) = 0;
};

// Inherits from class Node. Can be an INT, FLOAT, BOOL, ID or CHAR. If it's an ID the lexeme will be different of nullptr.
class TypeNode : public Node {
private:
    int token;
    const char *lexeme;
public:
    TypeNode(int token, const char *lexeme);
    ~TypeNode() override { delete this->lexeme; };
    int get_token() const { return token; }
    const char *get_lexeme() { return lexeme; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class Node. Represents a variable declaration.
class VarDeclNode : public Node {
private:
    TypeNode *type;
    IdListNode *id_list;
    VarDeclNode *next;
public:
    VarDeclNode(TypeNode *type, IdListNode *id_list, VarDeclNode *var_decl);
    ~VarDeclNode() override;
    TypeNode *get_type() { return type; }
    IdListNode *get_id_list() const { return id_list; }
    VarDeclNode *get_next() const { return next; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class Node. Represents an identifier in an identifier list.
class IdListNode : public Node {
private:
    PointerNode *pointer;
    IdentifierNode *id;
    ArrayDeclNode *array;
    IdListNode *next;
public:
    IdListNode(PointerNode *pointer, IdentifierNode *id, ArrayDeclNode *array, IdListNode *id_list);
    ~IdListNode() override;
    PointerNode *get_pointer() { return pointer; }
    IdentifierNode *get_id() { return id; }
    ArrayDeclNode *get_array() { return array; }
    IdListNode *get_next() { return next; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class Node. Because of it's constructors it will either have a VarDeclNode or a StatementListNode. It can be an item in a chained list.
class VarStmtNode : public Node {
private:
    VarDeclNode *decl = nullptr;
    StatementListNode *stmt_list = nullptr;
    VarStmtNode *next = nullptr;
public:
    VarStmtNode(VarDeclNode *decl, VarStmtNode *next);
    VarStmtNode(StatementListNode *stmt_list, VarStmtNode *next);
    ~VarStmtNode() override;
    VarDeclNode *get_decl() { return decl; }
    StatementListNode *getStmt_list() { return stmt_list; }
    VarStmtNode *get_next() { return next; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class Node. Because of it's constructors it will either have a VarDeclNode or a FunctionDeclNode. It can be an item in a chained list.
class VarFuncListNode : public Node {
private:
    VarDeclNode *decl = nullptr;
    FunctionDeclNode *func_list = nullptr;
    VarFuncListNode *next = nullptr;
public:
    VarFuncListNode(VarDeclNode *decl, VarFuncListNode *next);
    VarFuncListNode(FunctionDeclNode *func_list, VarFuncListNode *next);
    ~VarFuncListNode() override;
    VarDeclNode *get_decl() { return decl; }
    FunctionDeclNode *getFunc_list() { return func_list; }
    VarFuncListNode *get_next() { return next; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class Node. Represents a case block inside a switch structure. It can be an item in a chained list of case blocks.
class CaseBlockNode : public Node {
private:
    IntegerNode *num;
    StatementListNode *statement_list;
    CaseBlockNode *case_block;
public:
    CaseBlockNode(IntegerNode *num, StatementListNode *statement_list, CaseBlockNode *case_block);
    ~CaseBlockNode() override;
    IntegerNode *get_num() { return num; }
    StatementListNode *get_statement_list() { return statement_list; }
    CaseBlockNode *get_case_block() { return case_block; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class Node. Represents a declaration of a function.
class FunctionDeclNode : public Node {
private:
    TypeNode *type;
    PointerNode *pointer;
    IdentifierNode *id;
    FormalListNode *formal_list;
    VarStmtNode *var_stmt;
public:
    FunctionDeclNode(TypeNode *type, PointerNode *pointer, IdentifierNode *id, FormalListNode *formal_list,
                     VarStmtNode *var_stmt);
    ~FunctionDeclNode() override;
    TypeNode *get_type() { return type; }
    PointerNode *get_pointer() { return pointer; }
    IdentifierNode *get_id() { return id; }
    FormalListNode *get_formal_list() { return formal_list; }
    VarStmtNode *getVar_stmt() { return var_stmt; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class Node. Represents a class structure, it can be a item in a chained list.
class ClassListNode : public Node {
private:
    IdentifierNode *id;
    VarFuncListNode *var_func;
    IdentifierNode *parent_id;
    ClassListNode *class_list;
public:
    ClassListNode(IdentifierNode *id, VarFuncListNode *var_func,
                  IdentifierNode *parent_id, ClassListNode *class_list);
    ~ClassListNode() override;
    IdentifierNode *get_id() { return id; }
    VarFuncListNode *get_var_func() { return var_func; }
    IdentifierNode *get_parent_id() { return parent_id; }
    ClassListNode *get_class_list() { return class_list; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class Node. Represents a typedef struct structure. It can be an item in a chained list.
class TypeListNode : public Node {
private:
    VarDeclNode *var_list;
    IdentifierNode *id;
    TypeListNode *next;
public:
    TypeListNode(VarDeclNode *var_list, IdentifierNode *id, TypeListNode *next);
    ~TypeListNode() override;
    VarDeclNode *get_var_list() { return var_list; }
    IdentifierNode *get_id() { return id; }
    TypeListNode *get_next() { return next; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class Node. Represents the main program being analyzed. This will be the root of the AST returned by the parser.
class ProgramNode : public Node {
private:
    ClassListNode *class_list;
    TypeListNode *type_list;
    VarFuncListNode *var_func_list;
public:
    ProgramNode(ClassListNode *class_list,
                TypeListNode *type_list, VarFuncListNode *var_func_list);
    ~ProgramNode() override;
    ClassListNode *get_class_list() { return class_list; }
    TypeListNode *get_type_list() { return type_list; }
    VarFuncListNode *get_var_func_list() { return var_func_list; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class Node. Represents a * before a identifier to indicate that it's a pointer.
class PointerNode : public Node {
public:
    explicit PointerNode() = default;
    ~PointerNode() override = default;
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class Node. Represents a parameter in a function. It can be a item in a chained list.
class FormalListNode : public Node {
private:
    TypeNode *type;
    PointerNode *pointer;
    IdentifierNode *id;
    ArrayDeclNode *array;
    FormalListNode *next;
public:
    FormalListNode(TypeNode *type, PointerNode *pointer, IdentifierNode *id, ArrayDeclNode *array,
                   FormalListNode *next);
    ~FormalListNode() override;
    TypeNode *get_type() { return type; }
    PointerNode *get_pointer() { return pointer; }
    IdentifierNode *get_id() { return id; }
    ArrayDeclNode *get_array() { return array; }
    FormalListNode *get_next() { return next; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class StatementNode. Abstract expression node class.
class ExpNode : public StatementNode {
public:
    virtual ~ExpNode() = default;
    // TODO virtual void accept(Visitor *visitor) = 0;
};

// Inherits from class StatementNode. Represents an If/Else structure.
class IfNode : public StatementNode {
    ExpNode *exp;
    StatementNode *if_stmt;
    StatementNode *else_stmt;
public:
    IfNode(ExpNode *exp, StatementNode *if_stmt, StatementNode *else_stmt);
    ~IfNode() override;
    ExpNode *get_exp() { return exp; }
    StatementNode *get_statement() { return if_stmt; }
    StatementNode *get_second_statement() { return else_stmt; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class StatementNode. Represents a While structure.
class WhileNode : public StatementNode {
private:
    ExpNode *exp;
    StatementNode *statement;
public:
    WhileNode(ExpNode *exp, StatementNode *statement);
    ~WhileNode() override;
    ExpNode *get_exp() { return exp; }
    StatementNode *get_statement() { return statement; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class StatementNode. Represents a switch structure.
class SwitchNode : public StatementNode {
private:
    ExpNode *exp;
    CaseBlockNode *case_block;
public:
    SwitchNode(ExpNode *exp, CaseBlockNode *case_block);
    ~SwitchNode() override;
    ExpNode *get_exp() { return exp; }
    CaseBlockNode *get_case_block() { return case_block; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class StatementNode. Represents a break statement.
class BreakNode : public StatementNode {
public:
    BreakNode() = default;
    ~BreakNode() override = default;
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class StatementNode. Represents a print statement.
class PrintlnNode : public StatementNode {
private:
    ExpListNode *exp_list;
public:
    explicit PrintlnNode(ExpListNode *exp_list) { this->exp_list = exp_list; };
    ~PrintlnNode() override { delete this->exp_list; };
    ExpListNode *get_exp_list() { return exp_list; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class StatementNode. Represents a read statement.
class ReadNode : public StatementNode {
private:
    ExpNode *exp;
public:
    explicit ReadNode(ExpNode *exp) { this->exp = exp; };
    ~ReadNode() override { delete this->exp; };
    ExpNode *get_exp() { return exp; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class StatementNode. Represents a return statement.
class ReturnNode : public StatementNode {
private:
    ExpNode *exp;
public:
    explicit ReturnNode(ExpNode *exp) { this->exp = exp; };
    ~ReturnNode() override { delete this->exp; };
    ExpNode *get_exp() { return exp; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class StatementNode. Represents a throw statement.
class ThrowNode : public StatementNode {
public:
    ThrowNode() = default;
    ~ThrowNode() override = default;
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class StatementNode. Represents a try-catch structure.
class TryNode : public StatementNode {
private:
    StatementNode *try_stmt;
    StatementNode *catch_stmt;
public:
    TryNode(StatementNode *try_stmt, StatementNode *catch_stmt);
    ~TryNode() override;
    StatementNode *get_try_stmt() { return try_stmt; }
    StatementNode *get_catch_stmt() { return catch_stmt; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class StatementNode. It is one item in a chained list of statements.
class StatementListNode : public StatementNode {
private:
    StatementNode *statement;
    StatementListNode *statement_list;
public:
    StatementListNode(StatementNode *statement, StatementListNode *statement_list);
    ~StatementListNode() override;
    StatementNode *get_statement() { return statement; }
    StatementListNode *get_statement_list() { return statement_list; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class ExpNode. It is one item in a chained list of expressions.
class ExpListNode : public ExpNode {
private:
    ExpNode *exp;
    ExpListNode *exp_list;
public:
    ExpListNode(ExpNode *exp, ExpListNode *exp_list);
    ~ExpListNode() override;
    ExpNode *get_exp() { return exp; }
    ExpListNode *get_exp_list() { return exp_list; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class ExpNode. Represents an identifier.
class IdentifierNode : public ExpNode {
private:
    const char *lexeme;
public:
    explicit IdentifierNode(const char *lexeme) { this->lexeme = lexeme; };
    ~IdentifierNode() override { this->lexeme = nullptr; };
    const char *get_lexeme() { return lexeme; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class ExpNode. Represents an integer.
class IntegerNode : public ExpNode {
private:
    const char *lexeme;
public:
    explicit IntegerNode(const char *lexeme) { this->lexeme = lexeme; };
    ~IntegerNode() override { this->lexeme = nullptr; };
    const char *get_lexeme() { return lexeme; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class ExpNode. Represents a real.
class RealNode : public ExpNode {
private:
    const char *lexeme;
public:
    explicit RealNode(const char *lexeme) { this->lexeme = lexeme; };
    ~RealNode() override { this->lexeme = nullptr; };
    const char *get_lexeme() { return lexeme; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class ExpNode. Represents a literal.
class LiteralNode : public ExpNode {
private:
    const char *lexeme;
public:
    explicit LiteralNode(const char *lexeme) { this->lexeme = lexeme; };
    ~LiteralNode() override { this->lexeme = nullptr; };
    const char *get_lexeme() { return lexeme; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class ExpNode. Represents a character.
class CharacterNode : public ExpNode {
private:
    const char *lexeme;
public:
    explicit CharacterNode(const char *lexeme) { this->lexeme = lexeme; };
    ~CharacterNode() override { this->lexeme = nullptr; };
    const char *get_lexeme() { return lexeme; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class ExpNode. Represents an assignment expression.
class AssignNode : public ExpNode {
private:
    ExpNode *left_exp;
    ExpNode *right_exp;
public:
    AssignNode(ExpNode *left_exp, ExpNode *right_exp);
    ~AssignNode() override;
    ExpNode *get_left_exp() { return left_exp; }
    ExpNode *get_right_exp() { return right_exp; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class ExpNode. Represents a node from the Primary production. It can be an item in a chained list.
class PrimaryNode : public ExpNode {
private:
    ExpNode *exp;
    ExpNode *next;
public:
    PrimaryNode(ExpNode *exp, ExpNode *next);
    ~PrimaryNode() override;
    ExpNode *get_exp() { return exp; }
    ExpNode *get_next() { return next; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class ExpNode. Represents the calling of a function. It can be a item in a chained list of expressions.
class FunctionCallNode : public ExpNode {
private:
    ExpNode *exp;
    ExpNode *next;
public:
    FunctionCallNode(ExpNode *exp, ExpNode *next);
    ~FunctionCallNode() override;
    ExpNode *get_exp() { return exp; }
    ExpNode *get_next() { return next; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class ExpNode. Represents an access to a field with . or ->. It can be an item in a chained list.
class PointerValueExpNode : public ExpNode {
private:
    ExpNode *exp;
    ExpNode *next;
public:
    PointerValueExpNode(ExpNode *exp, ExpNode *next);
    ~PointerValueExpNode() override;
    ExpNode *get_exp() { return exp; }
    ExpNode *get_next() { return next; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class ExpNode. Represents a & before a identifier to indicate that it is accessing the address.
class AddressValueNode : public ExpNode {
private:
    ExpNode *exp;
public:
    explicit AddressValueNode(ExpNode *exp) { this->exp = exp; }
    ~AddressValueNode() override { delete this->exp; }
    ExpNode *get_exp() const { return exp; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class ExpNode. Represents an access to a pointer value.
class PointerValueNode : public ExpNode {
private:
    ExpNode *exp;
public:
    explicit PointerValueNode(ExpNode *exp) { this->exp = exp; }
    ~PointerValueNode() override { delete this->exp; }
    ExpNode *get_exp() const { return exp; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class ExpNode. Represents a access to a array.
class ArrayAccessNode : public ExpNode {
private:
    ExpNode *exp;
    ExpNode *index_exp;
public:
    ArrayAccessNode(ExpNode *exp, ExpNode *index_exp);
    ~ArrayAccessNode() override;
    ExpNode *get_exp() { return exp; }
    ExpNode *get_index_exp() { return index_exp; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class ExpNode. Represents a array declaration.
class ArrayDeclNode : public ExpNode {
private:
    ExpNode *index_exp;
public:
    explicit ArrayDeclNode(ExpNode *index_exp) { this->index_exp = index_exp; }
    ExpNode *get_index_exp() { return index_exp; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class ExpNode. Represents the creation of an instance with the new keyword.
class NewNode : public ExpNode {
private:
    IdentifierNode *id;
    ExpListNode *exp_list;
public:
    NewNode(IdentifierNode *id, ExpListNode *exp_list);
    ~NewNode() override;
    IdentifierNode *get_id() { return id; }
    ExpListNode *get_exp_list() { return exp_list; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class ExpNode. Represents a relational operation (==, !=, <=, >=, <, >).
class RelationalOpNode : public ExpNode {
private:
    int token_op;
    ExpNode *left_exp;
    ExpNode *right_exp;
public:
    RelationalOpNode(int token_op, ExpNode *left_exp, ExpNode *right_exp);
    ~RelationalOpNode() override;
    int get_op() { return token_op; }
    ExpNode *get_left_exp() { return left_exp; }
    ExpNode *get_right_exp() { return right_exp; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class ExpNode. Represents an addition operation (+, -).
class AdditionOpNode : public ExpNode {
private:
    int token_op;
    ExpNode *left_exp;
    ExpNode *right_exp;
public:
    AdditionOpNode(int token_op, ExpNode *left_exp, ExpNode *right_exp);
    ~AdditionOpNode() override;
    int get_op() { return token_op; }
    ExpNode *get_left_exp() { return left_exp; }
    ExpNode *get_right_exp() { return right_exp; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class ExpNode. Represents a multiplication or division operation (*, /, %).
class MultiplicationOpNode : public ExpNode {
private:
    int token_op;
    ExpNode *left_exp;
    ExpNode *right_exp;
public:
    MultiplicationOpNode(int token_op, ExpNode *left_exp, ExpNode *right_exp);
    ~MultiplicationOpNode() override;
    int get_op() { return token_op; }
    ExpNode *get_left_exp() { return left_exp; }
    ExpNode *get_right_exp() { return right_exp; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class ExpNode. Represents a boolean operation (&&, ||).
class BooleanOpNode : public ExpNode {
private:
    int token_op;
    ExpNode *left_exp;
    ExpNode *right_exp;
public:
    BooleanOpNode(int token_op, ExpNode *left_exp, ExpNode *right_exp);
    ~BooleanOpNode() override;
    int get_op() { return token_op; }
    ExpNode *get_left_exp() { return left_exp; }
    ExpNode *get_right_exp() { return right_exp; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class ExpNode. Represents a bitwise operation (&, |).
class BitwiseOpNode : public ExpNode {
private:
    int token_op;
    ExpNode *left_exp;
    ExpNode *right_exp;
public:
    BitwiseOpNode(int token_op, ExpNode *left_exp, ExpNode *right_exp);
    ~BitwiseOpNode() override;
    int get_op() { return token_op; }
    ExpNode *get_left_exp() { return left_exp; }
    ExpNode *get_parent_exp() { return right_exp; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class ExpNode. Represents a true expression.
class TrueNode : public ExpNode {
public:
    TrueNode() = default;
    ~TrueNode() override = default;
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class ExpNode. Represents a false expression.
class FalseNode : public ExpNode {
public:
    FalseNode() = default;
    ~FalseNode() override = default;
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class ExpNode. Represents a this expression.
class ThisNode : public ExpNode {
public:
    ThisNode() = default;
    ~ThisNode() override = default;
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class ExpNode. Represents a not expression.
class NotNode : public ExpNode {
private:
    ExpNode *exp;
public:
    explicit NotNode(ExpNode *exp) { this->exp = exp; }
    ~NotNode() override { delete this->exp; }
    ExpNode *get_exp() const { return exp; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

// Inherits from class ExpNode. Represents a sign expression.
class SignNode : public ExpNode {
private:
    ExpNode *exp;
public:
    explicit SignNode(ExpNode *exp) { this->exp = exp; }
    ~SignNode() override { delete this->exp; }
    ExpNode *get_exp() const { return exp; }
    // TODO void accept(Visitor *visitor) override { visitor->visit(this); }
};

#endif //COMPILADOR_2019_3_AST_H
