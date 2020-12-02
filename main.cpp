#include <driver.hh>
#include <iostream>

int main(int argc, char** argv) {
    int result = 0;
    TDriver driver;

    for (int i = 1; i < argc; ++i) {
        if (argv[i] == std::string("-p")) {
            driver.trace_parsing = true;
        } else if (argv[i] == std::string("-s")) {
            driver.trace_scanning = true;
        } else if (!driver.parse(argv[i])) {
            result = driver.Evaluate();
        } else {
            result = 1;
        }
    }

    return result;
}