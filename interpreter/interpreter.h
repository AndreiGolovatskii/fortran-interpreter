#pragma once

#include <driver.hh>
#include <functional>
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
    int Status() {
        return Status_;
    }

    explicit TInterpreter(std::istream& cin = std::cin, std::ostream& cout = std::cout, std::ostream& cerr = std::cout)
        : Cin_(cin), Cout_(cout), Cerr_(cerr) {}

    std::unique_ptr<TType> VisitDoublePositionExpression(
            TDoublePositionExpression* expression,
            const std::function<std::unique_ptr<TType>(const std::unique_ptr<TType>&, const std::unique_ptr<TType>&)>&
                    operation) {
        auto fist   = expression->first->Accept(this);
        auto second = expression->second->Accept(this);

        return operation(fist, second);
    }

    std::unique_ptr<TType> Visit(TSumExpression* expression) final {
        return VisitDoublePositionExpression(expression, TypeAdd);
    }

    std::unique_ptr<TType> Visit(TSubExpression* expression) final {
        return VisitDoublePositionExpression(expression, TypeSub);
    }

    std::unique_ptr<TType> Visit(TMulExpression* expression) final {
        return VisitDoublePositionExpression(expression, TypeMul);
    }

    std::unique_ptr<TType> Visit(TDivExpression* expression) final {
        return VisitDoublePositionExpression(expression, TypeDiv);
    }

    std::unique_ptr<TType> Visit(TGtExpression* expression) final {
        return Gt(expression->first->Accept(this), expression->second->Accept(this));
    }

    std::unique_ptr<TType> Visit(TLtExpression* expression) final {
        return Lt(expression->first->Accept(this), expression->second->Accept(this));
    }

    std::unique_ptr<TType> Visit(TGeExpression* expression) final {
        return Ge(expression->first->Accept(this), expression->second->Accept(this));
    }

    std::unique_ptr<TType> Visit(TLeExpression* expression) final {
        return Le(expression->first->Accept(this), expression->second->Accept(this));
    }

    std::unique_ptr<TType> Visit(TEqExpression* expression) final {
        return Eq(expression->first->Accept(this), expression->second->Accept(this));
    }

    std::unique_ptr<TType> Visit(TEqvExpression* expression) final {
        return Eqv(expression->first->Accept(this), expression->second->Accept(this));
    }

    std::unique_ptr<TType> Visit(TNotExpression* expression) final {
        return Not(expression->Expression->Accept(this));
    }

    std::unique_ptr<TType> Visit(TAndExpression* expression) final {
        return And(expression->first->Accept(this), expression->second->Accept(this));
    }

    std::unique_ptr<TType> Visit(TOrExpression* expression) final {
        return Or(expression->first->Accept(this), expression->second->Accept(this));
    }

    void Visit(TDeclaration* declaration) final {
        if (Vars_.count(declaration->VarName)) {
            throw RuntimeError("redefinition of " + declaration->VarName);
        }
        Vars_[declaration->VarName] = declaration->VarType->GetDefault();
    }

    void Visit(TAssignStatement* assign) final {
        if (Vars_.count(assign->VarName) == 0) {
            throw RuntimeError("Use of undeclared identifier " + assign->VarName);
        }
        Vars_[assign->VarName]->Assign(assign->Expr->Accept(this));
    }

    std::unique_ptr<TType> Visit(TIdentifierExpression* identifier) final {
        if (Vars_.count(identifier->Identifier) == 0) {
            throw RuntimeError("Use of undeclared identifier " + identifier->Identifier);
        }
        return Vars_[identifier->Identifier]->Clone();
    }

    std::unique_ptr<TType> Visit(TValueExpression* value) final {
        return value->Value->Clone();
    }

    void Visit(TPrintStatement* print) final {
        for (auto& expr : print->Exprs) {
            expr->Accept(this)->Print(Cout_);
            if (expr != print->Exprs.back()) {
                Cout_ << " ";
            }
        }
        Cout_ << std::endl;
    }

    void Visit(TReadStatement* read) final {
        for (auto& var : read->Vars) {
            const auto& id = dynamic_cast<TIdentifierExpression&>(*var).Identifier;
            Vars_.at(id)->Read(Cin_);
        }
    }

    void Visit(std::vector<std::unique_ptr<TStatement>>& operatorList) {
        auto NextOperator_ = operatorList.begin();
        while (NextOperator_ != operatorList.end()) {
            (*NextOperator_)->Accept(this);
            NextOperator_++;
        }
    }

    void Visit(TIfStatement* ifStat) override {
        for (auto& simpleIf : ifStat->Components) {
            if (GetLogicalValue(simpleIf->Cond->Accept(this))) {
                Visit(simpleIf->Statements);
                return;
            }
        }
    }

    void Visit(TDoLoopStatement* doLoop) override {
        Vars_.at(doLoop->VarName)->Assign(doLoop->StartExpression->Accept(this));

        auto& iteratorValuePtr = Vars_.at(doLoop->VarName);
        auto endValuePtr       = doLoop->EndExpression->Accept(this);
        auto stepValuePtr      = doLoop->StepExpression->Accept(this);

        auto& iteratorValue = dynamic_cast<TInteger&>(*iteratorValuePtr);
        int endValue        = dynamic_cast<TInteger&>(*endValuePtr).Value;
        int stepValue       = dynamic_cast<TInteger&>(*stepValuePtr).Value;
        if (stepValue == 0) {
            throw std::runtime_error("step must be non-zero");
        }
        while (stepValue > 0 && iteratorValue.Value <= endValue || stepValue < 0 && iteratorValue.Value >= endValue) {
            Visit(doLoop->Statements);
            iteratorValue.Value += stepValue;
        }
    }

    void Visit(TDoWhileLoopStatement* doWhileLoop) override {
        while (GetLogicalValue(doWhileLoop->Condition->Accept(this))) {
            Visit(doWhileLoop->Statements);
        }
    }
};