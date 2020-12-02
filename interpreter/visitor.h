#pragma once

#include "statements_forward_declaration.hh"

class TVisitor {
public:
    virtual void Visit(TDeclaration*) = 0;
    virtual void Visit(TAssignStatement*) = 0;
    virtual void Visit(TSumExpression*) = 0;
    virtual void Visit(TSubExpression*) = 0;
    virtual void Visit(TIdentifierExpression*) = 0;
    virtual void Visit(TValueExpression*) = 0;
    virtual void Visit(TPrintStatement* write) = 0;
    virtual void Visit(TIfStatement*) = 0;
};