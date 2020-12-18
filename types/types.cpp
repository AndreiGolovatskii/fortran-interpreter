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
