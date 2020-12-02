#include "types.hh"

std::ostream& operator<<(std::ostream& out, const TType& type) {
    type.Print(out);
    return out;
}


std::string LoverCase(std::string s) {
    for (char& c : s) {
        c = std::tolower(c);
    }
    return s;
}

std::unique_ptr<TType> GetDefaultValue(const std::string& type) {
    if (type == "integer") {
        return std::unique_ptr<TType>(new TInteger());
    } else if (type == "none") {
        return std::unique_ptr<TType>(new TNone());
    } else {
        throw std::logic_error("Unknown Type");
    }
}