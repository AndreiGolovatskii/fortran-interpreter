#include "types.hh"

std::ostream& operator<<(std::ostream& out, const TType& type) {
    type.Print(out);
    return out;
}

std::string LoverCase(std::string s) {
    for (char& c : s) { c = std::tolower(c); }
    return s;
}

std::unique_ptr<TType> TypeAdd(const std::unique_ptr<TType>& first, const std::unique_ptr<TType>& second) {
    return std::unique_ptr<TType>(std::make_unique<TInteger>(dynamic_cast<const TInteger*>(first.get())->Value +
                                                             dynamic_cast<const TInteger*>(second.get())->Value));
}
std::unique_ptr<TType> TypeSub(const std::unique_ptr<TType>& first, const std::unique_ptr<TType>& second) {
    return std::unique_ptr<TType>(std::make_unique<TInteger>(dynamic_cast<const TInteger*>(first.get())->Value -
                                                             dynamic_cast<const TInteger*>(second.get())->Value));
}
std::unique_ptr<TType> TypeMul(const std::unique_ptr<TType>& first, const std::unique_ptr<TType>& second) {
    return std::unique_ptr<TType>(std::make_unique<TInteger>(dynamic_cast<const TInteger*>(first.get())->Value *
                                                             dynamic_cast<const TInteger*>(second.get())->Value));
}
std::unique_ptr<TType> TypeDiv(const std::unique_ptr<TType>& first, const std::unique_ptr<TType>& second) {
    return std::unique_ptr<TType>(std::make_unique<TInteger>(dynamic_cast<const TInteger*>(first.get())->Value /
                                                             dynamic_cast<const TInteger*>(second.get())->Value));
}


std::unique_ptr<TLogical> Gt(const std::unique_ptr<TType>& first, const std::unique_ptr<TType>& second) {
    try {
        return std::make_unique<TLogical>(dynamic_cast<TInteger&>(*first).Value >
                                          dynamic_cast<TInteger&>(*second).Value);
    } catch (const std::bad_cast& _) {}
    try {
        return std::make_unique<TLogical>(dynamic_cast<TCharacter&>(*first).Value >
                                          dynamic_cast<TCharacter&>(*second).Value);
    } catch (const std::bad_cast& _) {}
    throw std::bad_cast();
}


std::unique_ptr<TLogical> Lt(const std::unique_ptr<TType>& first, const std::unique_ptr<TType>& second) {
    try {
        return std::make_unique<TLogical>(dynamic_cast<TInteger&>(*first).Value <
                                          dynamic_cast<TInteger&>(*second).Value);
    } catch (const std::bad_cast& _) {}
    try {
        return std::make_unique<TLogical>(dynamic_cast<TCharacter&>(*first).Value <
                                          dynamic_cast<TCharacter&>(*second).Value);
    } catch (const std::bad_cast& _) {}
    throw std::bad_cast();
}


std::unique_ptr<TLogical> Eqv(const std::unique_ptr<TType>& first, const std::unique_ptr<TType>& second) {
    return std::make_unique<TLogical>(dynamic_cast<TLogical&>(*first).Value == dynamic_cast<TLogical&>(*second).Value);
}


std::unique_ptr<TLogical> And(const std::unique_ptr<TType>& first, const std::unique_ptr<TType>& second) {
    return std::make_unique<TLogical>(dynamic_cast<TLogical&>(*first).Value && dynamic_cast<TLogical&>(*second).Value);
}

std::unique_ptr<TLogical> Or(const std::unique_ptr<TType>& first, const std::unique_ptr<TType>& second) {
    return std::make_unique<TLogical>(dynamic_cast<TLogical&>(*first).Value || dynamic_cast<TLogical&>(*second).Value);
}


std::unique_ptr<TLogical> Not(const std::unique_ptr<TType>& first) {
    try {
        return std::make_unique<TLogical>(!(dynamic_cast<TLogical&>(*first).Value));
    } catch (const std::bad_cast& _) {}
    throw std::bad_cast();
}


bool GetLogicalValue(const std::unique_ptr<TType>& obj) { return dynamic_cast<TLogical&>(*obj).Value; }