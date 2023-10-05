#include "pwd.hpp"

void pwd() {
    char wd[PATH_MAX];

    getwd(wd);
    std::cout << wd << std::endl;
}
