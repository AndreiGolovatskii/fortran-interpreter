#pragma once

#include <fstream>
#include <map>
#include <string>
#include <unordered_map>

#include "scanner.h"
#include "interpreter.h"
#include "parser.hh"
#include "types.hh"


class TDriver {
public:
    explicit TDriver(std::istream& cin = std::cin, std::ostream& cout = std::cout, std::ostream& cerr = std::cerr)
        : scanner(*this), parser(scanner, *this), Cin_(cin), Cout_(cout), Cerr_(cerr) {}

    int parse(const std::string& f);

    std::string file;
    bool trace_parsing = false;
    bool trace_scanning = false;
    yy::location location;
    Scanner scanner;
    yy::parser parser;

    void scan_begin();

    void scan_end();

    friend class Scanner;

    int Evaluate() {
        TInterpreter interpreter(Cin_, Cout_, Cerr_);
        interpreter.Visit(Declarations_);
        interpreter.Visit(Statements_);
        return interpreter.Status();
    }

    void SetStatements(std::vector<std::unique_ptr<TStatement>>&& statements) {
        Statements_ = std::move(statements);
    }

    void SetDeclarations(std::vector<std::unique_ptr<TStatement>>&& declarations) {
        Declarations_ = std::move(declarations);
    }

private:
    std::ifstream stream;
    std::istream& Cin_;
    std::ostream& Cout_;
    std::ostream& Cerr_;
    std::vector<std::unique_ptr<TStatement>> Declarations_;
    std::vector<std::unique_ptr<TStatement>> Statements_;
};
