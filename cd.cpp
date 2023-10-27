#include "cd.hpp"

int cd(std::string input) {
    int res = chdir((const char *) input.c_str());
    if (res == 0) {
        char wd[PATH_MAX];
        getwd(wd);
        setenv("PWD", wd, true);
    } else {
        std::cerr << "cd: " << input << ": No such file or directory" << std::endl;
        return 1;
    }
    return 0;
}
