#pragma once

#include <driver.hh>
#include <unordered_map>
#include <vector>

#include "driver.hh"
#include "statements.hh"
#include "types.hh"
#include "visitor.h"


class RuntimeError : public std::logic_error {
public:
    explicit RuntimeError(const std::string& what_arg) : logic_error(what_arg) {}
};


class TInterpreter : public TVisitor {
    std::unordered_map<std::string, std::unique_ptr<TType>> Vars_;
    int Status_ = 0;
    std::istream& Cin_;
    std::ostream& Cout_;
    std::ostream& Cerr_;

public:
    int Status() { return Status_; }

    explicit TInterpreter(std::istream& cin = std::cin, std::ostream& cout = std::cout,
                         std::ostream& cerr = std::cout)
            : Cin_(cin), Cout_(cout), Cerr_(cerr) {}
    void Run();

    void Visit(TSumExpression* expression) final {
        expression->first->Accept(this);
        expression->second->Accept(this);
        std::unique_ptr<TType> first = std::move(expression->first->ResultValue);
        std::unique_ptr<TType> second = std::move(expression->second->ResultValue);

        expression->ResultValue = *(first) + *(second);
    }

    void Visit(TDeclaration* declaration) final {
        if (Vars_.count(declaration->VarName)) { throw RuntimeError("redefinition of " + declaration->VarName); }
        Vars_[declaration->VarName] = std::unique_ptr<TType>(new TNone());
    }

    void Visit(TSubExpression* expression) final {
        // TODO
    }

    void Visit(TAssignStatement* assign) final {
        assign->Expr->Accept(this);
        if (Vars_.count(assign->VarName) == 0) {
            throw RuntimeError("Use of undeclared identifier " + assign->VarName);
        }
        Vars_[assign->VarName] = std::move(assign->Expr->ResultValue);
    }

    void Visit(TIdentifierExpression* identifier) final {
        if (Vars_.count(identifier->Identifier) == 0) {
            throw RuntimeError("Use of undeclared identifier " + identifier->Identifier);
        }
        identifier->ResultValue = Vars_[identifier->Identifier]->Clone();
    }

    void Visit(TValueExpression* value) final { value->ResultValue = value->Value->Clone(); }

    void Visit(TPrintStatement* print) final {
        for (auto* expr : print->Exprs) {
            expr->Accept(this);
            Cout_ << *(expr->ResultValue) << " ";
        }
        Cout_ << std::endl;
    }

    void Visit(std::vector<TStatement*>& operatorList) {
        auto NextOperator_ = operatorList.begin();
        while (NextOperator_ != operatorList.end()) {
            (*NextOperator_)->Accept(this);
            NextOperator_++;
        }
    }

    void Visit(TIfStatement* ifStat) {
        for (auto& simpleIf: ifStat->Components) {
            simpleIf->Cond->Accept(this);
            if (*(simpleIf->Cond->ResultValue)) {
                Visit(simpleIf->Statements);
                return;
            }
        }
    }
};