#pragma once

#include <fstream>
#include <map>
#include <string>
#include <unordered_map>

#include "operators.hh"
#include "parser.hh"
#include "scanner.h"
#include "types.hh"

class RuntimeError : public std::logic_error {
public:
    explicit RuntimeError(const std::string& what_arg) : logic_error(what_arg) {}
};

class Driver;

class Interpreter : public Visitor {
    Driver* Driver_;
    std::unordered_map<std::string, std::unique_ptr<Type>> Vars_;
    int Status_ = 0;
    std::istream& Cin_;
    std::ostream& Cout_;
    std::ostream& Cerr_;

public:
    int Status() { return Status_; }

    explicit Interpreter(Driver* driver, std::istream& cin = std::cin, std::ostream& cout = std::cout,
                         std::ostream& cerr = std::cout)
        : Driver_(driver), Cin_(cin), Cout_(cout), Cerr_(cerr) {}
    void Run();

    void Visit(SumExpression* expression) final {
        expression->first->Accept(this);
        expression->second->Accept(this);
        std::unique_ptr<Type> first = std::move(expression->first->ResultValue);
        std::unique_ptr<Type> second = std::move(expression->second->ResultValue);

        expression->ResultValue = *(first) + *(second);
    }

    void Visit(Declaration* declaration) final {
        if (Vars_.count(declaration->VarName)) { throw RuntimeError("redefinition of " + declaration->VarName); }
        Vars_[declaration->VarName] = std::unique_ptr<Type>(new None());
    }

    void Visit(SubExpression* expression) final {
        // TODO
    }

    void Visit(AssignOperator* assign) final {
        assign->Expr->Accept(this);
        if (Vars_.count(assign->VarName) == 0) {
            throw RuntimeError("Use of undeclared identifier " + assign->VarName);
        }
        Vars_[assign->VarName] = std::move(assign->Expr->ResultValue);
    }

    void Visit(IdentifierExpression* identifier) final {
        if (Vars_.count(identifier->Identifier) == 0) {
            throw RuntimeError("Use of undeclared identifier " + identifier->Identifier);
        }
        identifier->ResultValue = Vars_[identifier->Identifier]->Clone();
    }

    void Visit(ValueExpression* value) final { value->ResultValue = value->Value->Clone(); }

    void Visit(PrintOperator* print) final {
        for (auto* expr : print->Exprs) {
            expr->Accept(this);
            Cout_ << *(expr->ResultValue) << " ";
        }
        Cout_ << std::endl;
    }

    void Visit(std::vector<Operator*>& operatorList) {
        auto NextOperator_ = operatorList.begin();
        while (NextOperator_ != operatorList.end()) {
            (*NextOperator_)->Accept(this);
            NextOperator_++;
        }
    }

    void Visit(IfStatement* ifStat) {
        for (auto& simpleIf: ifStat->Components) {
            simpleIf->Cond->Accept(this);
            if (*(simpleIf->Cond->ResultValue)) {
                Visit(simpleIf->Operators);
                return;
            }
        }
    }
};


class Driver {
public:
    Driver();
    int parse(const std::string& f);
    std::string file;
    bool trace_parsing;

    void scan_begin();
    void scan_end();

    bool trace_scanning;
    yy::location location;

    friend class Scanner;
    Scanner scanner;
    yy::parser parser;

    int Evaluate() {
        Interpreter interpreter(this);
        interpreter.Run();
        return interpreter.Status();
    }

    void AddOperator(Operator* op) { operators.push_back(op); }

private:
    friend Interpreter;
    std::ifstream stream;
    std::vector<Operator*> operators;
};
