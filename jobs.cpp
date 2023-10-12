#include "jobs.hpp"

void jobs(std::vector<std::vector<std::string> > jobs) {
    for (auto job : jobs) {
        std::cout << "[" + job[0] + "] " + job[1] + " " + job[3];
    }
}