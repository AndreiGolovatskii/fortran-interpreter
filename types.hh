#pragma once

#include <iostream>

class Type {
public:
    virtual std::unique_ptr<Type> operator+(const Type& other) const = 0;
    virtual void Print(std::ostream& out) const = 0;
    virtual std::unique_ptr<Type> Clone() const = 0;
    explicit virtual operator bool() const = 0;
    virtual ~Type() {};
};

class None: public Type {
public:
    std::unique_ptr<Type> operator+(const Type& other) const override {
        throw std::logic_error("Not supported + for type None");
    }
    void Print(std::ostream& out) const override {
        out << "None";
    }
    [[nodiscard]] std::unique_ptr<Type> Clone() const override {
        return std::unique_ptr<Type>(new None());
    }
    explicit operator bool() const override {
        return false;
    }
};

std::ostream& operator<<(std::ostream& out, const Type& type);

class Integer : public Type {
public:
    int Value;
    explicit Integer(int value = 0) : Value(value){};
    std::unique_ptr<Type> operator+(const Type& other) const override {
        return std::unique_ptr<Type>(new Integer(dynamic_cast<const Integer*>(&other)->Value + Value));
    };
    void Print(std::ostream& out) const override {
        out << Value;
    }
    [[nodiscard]] std::unique_ptr<Type> Clone() const override {
        return std::unique_ptr<Type>(new Integer(Value));
    }
    explicit operator bool() const override {
        return Value;
    }
};

std::string LoverCase(std::string s);

std::unique_ptr<Type> GetDefaultValue(const std::string& type);