#pragma once

#include "statements_base.hh"
#include "visitor.h"


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


class TGtExpression : public TDoublePositionExpression {
public:
    TGtExpression(std::unique_ptr<TExpression>&& first, std::unique_ptr<TExpression>&& second)
        : TDoublePositionExpression(std::move(first), std::move(second)) {}
    std::unique_ptr<TType> Accept(TVisitor* visitor) final { return visitor->Visit(this); }
};


class TLtExpression : public TDoublePositionExpression {
public:
    TLtExpression(std::unique_ptr<TExpression>&& first, std::unique_ptr<TExpression>&& second)
        : TDoublePositionExpression(std::move(first), std::move(second)) {}
    std::unique_ptr<TType> Accept(TVisitor* visitor) final { return visitor->Visit(this); }
};


class TEqvExpression : public TDoublePositionExpression {
public:
    TEqvExpression(std::unique_ptr<TExpression>&& first, std::unique_ptr<TExpression>&& second)
        : TDoublePositionExpression(std::move(first), std::move(second)) {}
    std::unique_ptr<TType> Accept(TVisitor* visitor) final { return visitor->Visit(this); }
};


class TAndExpression : public TDoublePositionExpression {
public:
    TAndExpression(std::unique_ptr<TExpression>&& first, std::unique_ptr<TExpression>&& second)
            : TDoublePositionExpression(std::move(first), std::move(second)) {}
    std::unique_ptr<TType> Accept(TVisitor* visitor) final { return visitor->Visit(this); }
};


class TOrExpression : public TDoublePositionExpression {
public:
    TOrExpression(std::unique_ptr<TExpression>&& first, std::unique_ptr<TExpression>&& second)
            : TDoublePositionExpression(std::move(first), std::move(second)) {}
    std::unique_ptr<TType> Accept(TVisitor* visitor) final { return visitor->Visit(this); }
};


class TNotExpression : public TExpression {
public:
    std::unique_ptr<TExpression> Expression;
    explicit TNotExpression(std::unique_ptr<TExpression>&& exp) : Expression(std::move(exp)) {}
    std::unique_ptr<TType> Accept(TVisitor* visitor) final { return visitor->Visit(this); }
};


class TValueExpression : public TExpression {
public:
    std::unique_ptr<TType> Value;
    explicit TValueExpression(std::unique_ptr<TType>&& value) : Value(std::move(value)) {}
    std::unique_ptr<TType> Accept(TVisitor* visitor) final { return visitor->Visit(this); }
};
