#pragma once

#include "types.hh"
#include "visitor.h"


class TStatement {
public:
    virtual void Accept(TVisitor* visitor) = 0;
    virtual ~TStatement() = default;
};


class TExpression {
public:
    virtual std::unique_ptr<TType> Accept(TVisitor* visitor) = 0;
    virtual ~TExpression() = default;
};
