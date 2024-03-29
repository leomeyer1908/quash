#include "jobs.hpp"

#include <sys/wait.h>

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

void jobs_cmd(std::vector<std::vector<std::string> > jobs) {
    for (int i = 0; i < jobs.size(); i++) {
        std::vector<std::string> job = jobs[i];
        if (job[0] != "0") {
            if (is_process_running(atoi((const char *) job[1].c_str()))) {
                std::cout << "[" + job[0] + "] " + job[1] + " " + job[2] << std::endl;
            } else {
                jobs[i][0] = "0";
            }
        }
    }
}
