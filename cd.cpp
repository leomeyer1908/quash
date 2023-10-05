#include "cd.hpp"

int cd(std::string input) {
    if (chdir((const char *) input.c_str()) != 0) {
        std::cerr << "cd: " << input << ": No such file or directory" << std::endl;
        return 1;
    }
    return 0;
}
