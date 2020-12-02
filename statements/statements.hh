#pragma once

#include <string>
#include <vector>

#include "types.hh"
#include "visitor.h"


class TStatement {
public:
    virtual void Accept(TVisitor* visitor) = 0;
    virtual ~TStatement() = default;
};


class TPrintStatement : public TStatement {
public:
    std::vector<TExpression*> Exprs;
    explicit TPrintStatement(std::vector<TExpression*> exprs) : Exprs(std::move(exprs)) {}
    void Accept(TVisitor* visitor) final { visitor->Visit(this); }
};


class TAssignStatement : public TStatement {
public:
    TAssignStatement(const std::string& varName, TExpression* expr) : VarName(LoverCase(varName)), Expr(expr) {}
    void Accept(TVisitor* visitor) final { visitor->Visit(this); }
    std::string VarName;
    TExpression* Expr;
};


class TDeclaration : public TStatement {
public:
    std::string VarName;
    std::string VarType;
    TDeclaration(const std::string& varName, const std::string& type)
        : VarName(LoverCase(varName)), VarType(LoverCase(type)) {}
    void Accept(TVisitor* visitor) final { visitor->Visit(this); }
};


class TExpression : public TStatement {
public:
    std::unique_ptr<TType> ResultValue;
};


class TIdentifierExpression : public TExpression {
public:
    std::string Identifier;
    explicit TIdentifierExpression(const std::string& id) : Identifier(LoverCase(id)) {}
    void Accept(TVisitor* visitor) final { visitor->Visit(this); }
};


class TDoublePositionExpression : public TExpression {
public:
    TDoublePositionExpression(TExpression* first, TExpression* second) : first(first), second(second) {}
    TExpression *first, *second;
};


class TSumExpression : public TDoublePositionExpression {
public:
    TSumExpression(TExpression* first, TExpression* second) : TDoublePositionExpression(first, second) {}
    void Accept(TVisitor* visitor) final { visitor->Visit(this); }
};


class TSubExpression : public TDoublePositionExpression {
public:
    TSubExpression(TExpression* first, TExpression* second) : TDoublePositionExpression(first, second) {}
    void Accept(TVisitor* visitor) final { visitor->Visit(this); }
};


class TValueExpression : public TExpression {
public:
    TType* Value;
    TValueExpression(TType* value) : Value(value) {}
    void Accept(TVisitor* visitor) final { visitor->Visit(this); }
};

class TSimpleIfStatement { // TODO
public:
    TExpression* Cond;
    std::vector<TStatement*> Statements;
    TSimpleIfStatement(TExpression* cond, std::vector<TStatement*>&& ops): Cond(cond), Statements(std::move(ops)) {}
};

class TIfStatement: public TStatement {
public:
    std::vector<TSimpleIfStatement*> Components;
    TIfStatement() {}
    void Accept(TVisitor* visitor) final { visitor->Visit(this); }
    void AddElse(std::vector<TStatement*>&& ops) {
        AddIf(new TValueExpression(new TInteger(1)), std::move(ops)); // TODO
    }
    void AddIf(TExpression* pred, std::vector<TStatement*>&& ops) {
        Components.emplace_back(new TSimpleIfStatement(pred,   std::move(ops)));
    }
};
