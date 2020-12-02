#pragma once

#include <iostream>

class TType {
public:
    virtual std::unique_ptr<TType> operator+(const TType& other) const = 0;
    virtual void Print(std::ostream& out) const = 0;
    virtual std::unique_ptr<TType> Clone() const = 0;
    explicit virtual operator bool() const = 0;
    virtual ~TType() {};
};

class TNone : public TType {
public:
    std::unique_ptr<TType> operator+(const TType& other) const override {
        throw std::logic_error("Not supported + for type None");
    }
    void Print(std::ostream& out) const override {
        out << "None";
    }
    [[nodiscard]] std::unique_ptr<TType> Clone() const override {
        return std::unique_ptr<TType>(new TNone());
    }
    explicit operator bool() const override {
        return false;
    }
};

std::ostream& operator<<(std::ostream& out, const TType& type);

class TInteger : public TType {
public:
    int Value;
    explicit TInteger(int value = 0) : Value(value){};
    std::unique_ptr<TType> operator+(const TType& other) const override {
        return std::unique_ptr<TType>(new TInteger(dynamic_cast<const TInteger*>(&other)->Value + Value));
    };
    void Print(std::ostream& out) const override {
        out << Value;
    }
    [[nodiscard]] std::unique_ptr<TType> Clone() const override {
        return std::unique_ptr<TType>(new TInteger(Value));
    }
    explicit operator bool() const override {
        return Value;
    }
};

std::string LoverCase(std::string s);

std::unique_ptr<TType> GetDefaultValue(const std::string& type);