#include "jobs.hpp"

bool is_process_running(int pid) {
    int status;
    pid_t result = waitpid(pid, &status, WNOHANG);
    if (!result) {
        return true;
    }
    else {
        return false;
    }
};

void jobs(std::vector<std::vector<std::string> > jobs) {
    for (auto job : jobs) {
        if (job[0] != "0")
            if (is_process_running(atoi((const char *) job[1].c_str()))) {
                std::cout << "[" + job[0] + "] " + job[1] + " " + job[3];
            } else {
                job[0] = "0";
            }
        }
    }
}
