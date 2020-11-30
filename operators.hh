#pragma once

#include <string>
#include <vector>

#include "types.hh"


class Driver;
class Operator;
class PrintOperator;
class Declaration;
class Expression;
class AssignOperator;
class SubExpression;
class SumExpression;
class IdentifierExpression;
class ValueExpression;
class SimpleIfStatement;
class IfStatement;


class Visitor {
public:
    virtual void Visit(Declaration*) = 0;
    virtual void Visit(AssignOperator*) = 0;
    virtual void Visit(SumExpression*) = 0;
    virtual void Visit(SubExpression*) = 0;
    virtual void Visit(IdentifierExpression*) = 0;
    virtual void Visit(ValueExpression*) = 0;
    virtual void Visit(PrintOperator* write) = 0;
    virtual void Visit(IfStatement*) = 0;
};


class Operator {
public:
    virtual void Accept(Visitor* visitor) = 0;
    virtual ~Operator() = default;
};


class PrintOperator : public Operator {
public:
    std::vector<Expression*> Exprs;
    explicit PrintOperator(std::vector<Expression*> exprs) : Exprs(std::move(exprs)) {}
    void Accept(Visitor* visitor) final { visitor->Visit(this); }
};


class AssignOperator : public Operator {
public:
    AssignOperator(const std::string& varName, Expression* expr) : VarName(LoverCase(varName)), Expr(expr) {}
    void Accept(Visitor* visitor) final { visitor->Visit(this); }
    std::string VarName;
    Expression* Expr;
};


class Declaration : public Operator {
public:
    std::string VarName;
    std::string VarType;
    Declaration(const std::string& varName, const std::string& type)
        : VarName(LoverCase(varName)), VarType(LoverCase(type)) {}
    void Accept(Visitor* visitor) final { visitor->Visit(this); }
};


class Expression : public Operator {
public:
    std::unique_ptr<Type> ResultValue;
};


class IdentifierExpression : public Expression {
public:
    std::string Identifier;
    explicit IdentifierExpression(const std::string& id) : Identifier(LoverCase(id)) {}
    void Accept(Visitor* visitor) final { visitor->Visit(this); }
};


class DoublePositionExpression : public Expression {
public:
    DoublePositionExpression(Expression* first, Expression* second) : first(first), second(second) {}
    Expression *first, *second;
};


class SumExpression : public DoublePositionExpression {
public:
    SumExpression(Expression* first, Expression* second) : DoublePositionExpression(first, second) {}
    void Accept(Visitor* visitor) final { visitor->Visit(this); }
};


class SubExpression : public DoublePositionExpression {
public:
    SubExpression(Expression* first, Expression* second) : DoublePositionExpression(first, second) {}
    void Accept(Visitor* visitor) final { visitor->Visit(this); }
};


class ValueExpression : public Expression {
public:
    Type* Value;
    ValueExpression(Type* value) : Value(value) {}
    void Accept(Visitor* visitor) final { visitor->Visit(this); }
};

class SimpleIfStatement { // TODO
public:
    Expression* Cond;
    std::vector<Operator*> Operators;
    SimpleIfStatement(Expression* cond, std::vector<Operator*>&& ops): Cond(cond), Operators(std::move(ops)) {}
};

class IfStatement: public Operator {
public:
    std::vector<SimpleIfStatement*> Components;
    IfStatement() {}
    void Accept(Visitor* visitor) final { visitor->Visit(this); }
    void AddElse(std::vector<Operator*>&& ops) {
        AddIf(new ValueExpression(new Integer(1)), std::move(ops)); // TODO
    }
    void AddIf(Expression* pred, std::vector<Operator*>&& ops) {
        Components.emplace_back(new SimpleIfStatement(pred,   std::move(ops)));
    }
};
