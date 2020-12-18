#pragma once

#include <string>
#include <utility>
#include <vector>

#include "types.hh"
#include "visitor.h"

#include "statements_base.hh"

#include "expressions.hh"

class TPrintStatement : public TStatement {
public:
    std::vector<std::unique_ptr<TExpression>> Exprs;
    explicit TPrintStatement(std::vector<std::unique_ptr<TExpression>> exprs) : Exprs(std::move(exprs)) {}
    void Accept(TVisitor* visitor) final { visitor->Visit(this); }
};


class TAssignStatement : public TStatement {
public:
    TAssignStatement(const std::string& varName, std::unique_ptr<TExpression>&& expr)
        : VarName(LoverCase(varName)), Expr(std::move(expr)) {}
    void Accept(TVisitor* visitor) final { visitor->Visit(this); }
    std::string VarName;
    std::unique_ptr<TExpression> Expr;
};


class TDeclaration : public TStatement {
public:
    std::string VarName;
    std::shared_ptr<TTypeDescription> VarType;
    TDeclaration(const std::string& varName, std::shared_ptr<TTypeDescription> type)
        : VarName(LoverCase(varName)), VarType(type) {}
    void Accept(TVisitor* visitor) final { visitor->Visit(this); }
};


class TSimpleIfStatement {
public:
    std::unique_ptr<TExpression> Cond;
    std::vector<std::unique_ptr<TStatement>> Statements;
    TSimpleIfStatement(std::unique_ptr<TExpression>&& cond, std::vector<std::unique_ptr<TStatement>>&& ops)
        : Cond(std::move(cond)), Statements(std::move(ops)) {}
};

class TIfStatement : public TStatement {
public:
    std::vector<std::unique_ptr<TSimpleIfStatement>> Components;
    TIfStatement() {}
    void Accept(TVisitor* visitor) final { visitor->Visit(this); }
    void AddElse(std::vector<std::unique_ptr<TStatement>>&& ops) {
        AddIf(std::make_unique<TValueExpression>(std::make_unique<TLogical>(true)), std::move(ops));
    }
    void AddIf(std::unique_ptr<TExpression>&& pred, std::vector<std::unique_ptr<TStatement>>&& ops) {
        Components.emplace_back(std::make_unique<TSimpleIfStatement>(std::move(pred), std::move(ops)));
    }
};


class TDoLoopStatement : public TStatement {
public:
    std::string VarName;
    std::unique_ptr<TExpression> StartExpression;
    std::unique_ptr<TExpression> EndExpression;
    std::unique_ptr<TExpression> StepExpression;
    std::vector<std::unique_ptr<TStatement>> Statements;
    TDoLoopStatement(std::string varName, std::unique_ptr<TExpression>&& startExpression,
                     std::unique_ptr<TExpression>&& endExpression, std::unique_ptr<TExpression>&& stepExpression,
                     std::vector<std::unique_ptr<TStatement>>&& statements)
        : VarName(std::move(varName)), StartExpression(std::move(startExpression)),
          EndExpression(std::move(endExpression)), StepExpression(std::move(stepExpression)),
          Statements(std::move(statements)) {}
    void Accept(TVisitor* visitor) final { visitor->Visit(this); }
};

class TDoWhileLoopStatement : public TStatement {
public:
    std::unique_ptr<TExpression> Condition;
    std::vector<std::unique_ptr<TStatement>> Statements;
    TDoWhileLoopStatement(std::unique_ptr<TExpression>&& condition,
                          std::vector<std::unique_ptr<TStatement>>&& statements)
        : Condition(std::move(condition)), Statements(std::move(statements)) {}
    void Accept(TVisitor* visitor) final { visitor->Visit(this); }
};