#pragma once

#include <cassert>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

class TType {
public:
    virtual void Print(std::ostream& out) const = 0;
    virtual void Read(std::istream& in)         = 0;

    virtual std::unique_ptr<TType> Clone() const = 0;

    virtual void Assign(const std::unique_ptr<TType>& other) = 0;

    virtual ~TType(){};
};

class TInteger : public TType {
public:
    explicit TInteger(int value = 0) : Value(value){};

    void Print(std::ostream& out) const override {
        out << Value;
    }

    void Read(std::istream& in) override {
        in >> Value;
    }

    [[nodiscard]] std::unique_ptr<TType> Clone() const override {
        return std::unique_ptr<TType>(std::make_unique<TInteger>(Value));
    }

    void Assign(const std::unique_ptr<TType>& other) override {
        Value = dynamic_cast<TInteger&>(*other).Value;
    }

    int Value;
};


class TCharacter : public TType {
public:
    explicit TCharacter(std::string value) : Value(std::move(value)), Len(Value.size()) {}

    explicit TCharacter(int len) : Value(), Len(len) {}

    void Print(std::ostream& out) const override {
        out << Value;
    };

    void Read(std::istream& in) override {
        in >> Value;
        Normalize_();
    }

    [[nodiscard]] std::unique_ptr<TType> Clone() const override {
        return std::unique_ptr<TType>(std::make_unique<TCharacter>(*this));
    }

    void Assign(const std::unique_ptr<TType>& other) override {
        Value = dynamic_cast<TCharacter&>(*other).Value;
        Normalize_();
    }

    std::string Value;
    size_t Len;

private:
    void Normalize_() {
        Value.resize(std::min(Len, Value.size()));
    }
};


class TLogical : public TType {
public:
    explicit TLogical(bool value) : Value(value) {}

    [[nodiscard]] std::unique_ptr<TType> Clone() const override {
        return std::unique_ptr<TType>(std::make_unique<TLogical>(*this));
    }

    void Assign(const std::unique_ptr<TType>& other) override {
        Value = dynamic_cast<TLogical&>(*other).Value;
    }

    void Print(std::ostream& out) const override {
        out << (Value ? 'T' : 'F');
    }

    void Read(std::istream& in) override {
        in >> Value;
    }

    bool Value;
};


class TTypeDescription {
public:
    virtual std::unique_ptr<TType> GetDefault() const = 0;

    virtual void SetAttributes(const std::vector<std::string>& values) = 0;

    virtual void SetAttributes(const std::vector<std::pair<std::string, std::string>>& kvalues) = 0;

    virtual ~TTypeDescription() = default;
};


class TIntegerDescription : public TTypeDescription {
public:
    explicit TIntegerDescription(const std::vector<std::string>& values) {
        SetAttributes(values);
    }

    explicit TIntegerDescription(const std::vector<std::pair<std::string, std::string>>& kvalues) {
        SetAttributes(kvalues);
    }

    [[nodiscard]] std::unique_ptr<TType> GetDefault() const override {
        return std::unique_ptr<TType>(std::make_unique<TInteger>());
    }

    void SetAttributes(const std::vector<std::string>& values) override {
        if (values.size() > 1 || values.size() == 1 && values[0] != "4") {
            throw std::logic_error("only 4-byte integers supports");
        }
    }

    void SetAttributes(const std::vector<std::pair<std::string, std::string>>& kvalues) override {
        if (kvalues.size() > 1 ||
            kvalues.size() == 1 && kvalues[0] != std::make_pair(std::string("KIND"), std::string("4"))) {
            throw std::logic_error("only 4-byte integers supports");
        }
    }
};


class TCharacterDescription : public TTypeDescription {
public:
    explicit TCharacterDescription(const std::vector<std::string>& values) {
        SetAttributes(values);
    }

    explicit TCharacterDescription(const std::vector<std::pair<std::string, std::string>>& kvalues) {
        SetAttributes(kvalues);
    }

    [[nodiscard]] std::unique_ptr<TType> GetDefault() const override {
        return std::unique_ptr<TType>(std::make_unique<TCharacter>(Len));
    }

    void SetAttributes(const std::vector<std::string>& values) override {
        if (values.size() > 1) {
            throw std::logic_error("only len attribute supports");
        }
        Len = std::stol(values[0]);
        if (Len < 0) {
            Len = 0;
        }
    }

    void SetAttributes(const std::vector<std::pair<std::string, std::string>>& kvalues) override {
        if (kvalues.size() > 1 || kvalues.size() == 1 && kvalues[0].first != "LEN") {
            throw std::logic_error("only len attribute supports");
        }
        Len = std::stol(kvalues[0].second);
        if (Len < 0) {
            Len = 0;
        }
    }

    int Len = 1;
};


class TLogicalDescription : public TTypeDescription {
public:
    explicit TLogicalDescription(const std::vector<std::string>& values) {
        SetAttributes(values);
    }

    explicit TLogicalDescription(const std::vector<std::pair<std::string, std::string>>& kvalues) {
        SetAttributes(kvalues);
    }

    [[nodiscard]] std::unique_ptr<TType> GetDefault() const override {
        return std::unique_ptr<TType>(std::make_unique<TLogical>(false));
    }

    void SetAttributes(const std::vector<std::string>& values) override {
        if (!values.empty()) {
            throw std::logic_error("logical not supports attributes");
        }
    }

    void SetAttributes(const std::vector<std::pair<std::string, std::string>>& kvalues) override {
        if (!kvalues.empty()) {
            throw std::logic_error("logical not supports attributes");
        }
    }
};

std::unique_ptr<TType> TypeAdd(const std::unique_ptr<TType>&, const std::unique_ptr<TType>&);

std::unique_ptr<TType> TypeSub(const std::unique_ptr<TType>&, const std::unique_ptr<TType>&);

std::unique_ptr<TType> TypeMul(const std::unique_ptr<TType>&, const std::unique_ptr<TType>&);

std::unique_ptr<TType> TypeDiv(const std::unique_ptr<TType>&, const std::unique_ptr<TType>&);


std::unique_ptr<TLogical> Gt(const std::unique_ptr<TType>&, const std::unique_ptr<TType>&);

std::unique_ptr<TLogical> Lt(const std::unique_ptr<TType>&, const std::unique_ptr<TType>&);

std::unique_ptr<TLogical> Ge(const std::unique_ptr<TType>&, const std::unique_ptr<TType>&);

std::unique_ptr<TLogical> Le(const std::unique_ptr<TType>&, const std::unique_ptr<TType>&);

std::unique_ptr<TLogical> Eq(const std::unique_ptr<TType>&, const std::unique_ptr<TType>&);

std::unique_ptr<TLogical> Eqv(const std::unique_ptr<TType>&, const std::unique_ptr<TType>&);

std::unique_ptr<TLogical> And(const std::unique_ptr<TType>&, const std::unique_ptr<TType>&);

std::unique_ptr<TLogical> Or(const std::unique_ptr<TType>&, const std::unique_ptr<TType>&);

std::unique_ptr<TLogical> Not(const std::unique_ptr<TType>&);

bool GetLogicalValue(const std::unique_ptr<TType>&);

std::string LoverCase(std::string s);
