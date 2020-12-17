#pragma once

#include "visitor.h"
#include "statements_base.hh"


class TIdentifierExpression : public TExpression {
public:
    std::string Identifier;
    explicit TIdentifierExpression(const std::string& id) : Identifier(LoverCase(id)) {}
    std::unique_ptr<TType> Accept(TVisitor* visitor) final { return visitor->Visit(this); }
};


class TDoublePositionExpression : public TExpression {
public:
    TDoublePositionExpression(std::unique_ptr<TExpression>&& first, std::unique_ptr<TExpression>&& second)
        : first(std::move(first)), second(std::move(second)) {}
    std::unique_ptr<TExpression> first, second;
};


class TSumExpression : public TDoublePositionExpression {
public:
    TSumExpression(std::unique_ptr<TExpression>&& first, std::unique_ptr<TExpression>&& second)
        : TDoublePositionExpression(std::move(first), std::move(second)) {}
    std::unique_ptr<TType> Accept(TVisitor* visitor) final { return visitor->Visit(this); }
};


class TSubExpression : public TDoublePositionExpression {
public:
    TSubExpression(std::unique_ptr<TExpression>&& first, std::unique_ptr<TExpression>&& second)
        : TDoublePositionExpression(std::move(first), std::move(second)) {}
    std::unique_ptr<TType> Accept(TVisitor* visitor) final { return visitor->Visit(this); }
};


class TMulExpression : public TDoublePositionExpression {
public:
    TMulExpression(std::unique_ptr<TExpression>&& first, std::unique_ptr<TExpression>&& second)
            : TDoublePositionExpression(std::move(first), std::move(second)) {}
    std::unique_ptr<TType> Accept(TVisitor* visitor) final { return visitor->Visit(this); }
};


class TDivExpression : public TDoublePositionExpression {
public:
    TDivExpression(std::unique_ptr<TExpression>&& first, std::unique_ptr<TExpression>&& second)
            : TDoublePositionExpression(std::move(first), std::move(second)) {}
    std::unique_ptr<TType> Accept(TVisitor* visitor) final { return visitor->Visit(this); }
};


class TValueExpression : public TExpression {
public:
    std::unique_ptr<TType> Value;
    TValueExpression(std::unique_ptr<TType>&& value) : Value(std::move(value)) {}
    std::unique_ptr<TType> Accept(TVisitor* visitor) final { return visitor->Visit(this); }
};
