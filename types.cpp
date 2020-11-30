#include "types.hh"

std::ostream& operator<<(std::ostream& out, const Type& type) {
    type.Print(out);
    return out;
}


std::string LoverCase(std::string s) {
    for (char& c : s) {
        c = std::tolower(c);
    }
    return s;
}

std::unique_ptr<Type> GetDefaultValue(const std::string& type) {
    if (type == "integer") {
        return std::unique_ptr<Type>(new Integer());
    } else if (type == "none") {
        return std::unique_ptr<Type>(new None());
    } else {
        throw std::logic_error("Unknown Type");
    }
}