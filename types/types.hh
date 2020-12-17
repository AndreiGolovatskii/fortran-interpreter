#pragma once

#include <iostream>

class TType {
public:
    virtual void Print(std::ostream& out) const = 0;
    virtual std::unique_ptr<TType> Clone() const = 0;
    virtual void Assign(const std::unique_ptr<TType>& other) = 0;
    virtual explicit operator bool() const = 0;
    virtual ~TType() {};
};

std::ostream& operator<<(std::ostream& out, const TType& type);

class TInteger : public TType {
public:
    int Value;
    explicit TInteger(int value = 0) : Value(value){};

    void Print(std::ostream& out) const override {
        out << Value;
    }
    [[nodiscard]] std::unique_ptr<TType> Clone() const override {
        return std::unique_ptr<TType>(std::make_unique<TInteger>(Value));
    }
    explicit operator bool() const override {
        return Value;
    }

    void Add(const TInteger& other) {
        Value += other.Value;
    }

    void Assign(const std::unique_ptr<TType>& other) override {
        Value = dynamic_cast<TInteger*>(other.get())->Value;
    }

    bool Great(const TInteger& other) const {
        return Value > other.Value;
    }
};

std::string LoverCase(std::string s);

std::unique_ptr<TType> GetDefaultValue(const std::string& type);



std::unique_ptr<TType> TypeAdd(const std::unique_ptr<TType>&, const std::unique_ptr<TType>&);
std::unique_ptr<TType> TypeSub(const std::unique_ptr<TType>&, const std::unique_ptr<TType>&);
std::unique_ptr<TType> TypeMul(const std::unique_ptr<TType>&, const std::unique_ptr<TType>&);
std::unique_ptr<TType> TypeDiv(const std::unique_ptr<TType>&, const std::unique_ptr<TType>&);
