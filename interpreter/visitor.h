#pragma once

#include "statements_forward_declaration.hh"

class TVisitor {
public:
    virtual void Visit(TDeclaration*) = 0;
    virtual void Visit(TAssignStatement*) = 0;
    virtual void Visit(TPrintStatement* write) = 0;
    virtual void Visit(TIfStatement*) = 0;
    virtual void Visit(TDoLoopStatement*) = 0;
    virtual void Visit(TDoWhileLoopStatement*) = 0;

    virtual std::unique_ptr<TType> Visit(TSumExpression*) = 0;
    virtual std::unique_ptr<TType> Visit(TSubExpression*) = 0;
    virtual std::unique_ptr<TType> Visit(TMulExpression*) = 0;
    virtual std::unique_ptr<TType> Visit(TDivExpression*) = 0;
    virtual std::unique_ptr<TType> Visit(TGtExpression*) = 0;
    virtual std::unique_ptr<TType> Visit(TLtExpression*) = 0;
    virtual std::unique_ptr<TType> Visit(TEqvExpression*) = 0;
    virtual std::unique_ptr<TType> Visit(TNotExpression*) = 0;
    virtual std::unique_ptr<TType> Visit(TAndExpression*) = 0;
    virtual std::unique_ptr<TType> Visit(TOrExpression*) = 0;
    virtual std::unique_ptr<TType> Visit(TIdentifierExpression*) = 0;
    virtual std::unique_ptr<TType> Visit(TValueExpression*) = 0;
};