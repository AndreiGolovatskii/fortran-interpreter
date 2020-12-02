#pragma once

#include <fstream>
#include <map>
#include <string>
#include <unordered_map>

#include "grammar/scanner.h"
#include "interpreter.h"
#include "parser.hh"
#include "types.hh"


class TDriver {
public:
    TDriver();
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
        TInterpreter interpreter;
        interpreter.Visit(Statements_);
        return interpreter.Status();
    }

    void AddStatement(TStatement* op) { Statements_.push_back(op); }

private:
    std::ifstream stream;
    std::vector<TStatement*> Statements_;
};
