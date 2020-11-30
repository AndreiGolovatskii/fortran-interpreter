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


class Visitor {
public:
    virtual void Visit(Declaration* declaration) = 0;
    virtual void Visit(AssignOperator* declaration) = 0;
    virtual void Visit(SumExpression* declaration) = 0;
    virtual void Visit(SubExpression* declaration) = 0;
    virtual void Visit(IdentifierExpression* declaration) = 0;
    virtual void Visit(ValueExpression* declaration) = 0;
    virtual void Visit(PrintOperator* write) = 0;
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

class IfStatement: public Operator {
};