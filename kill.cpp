#include "kill.hpp"




void kill_cmd(std::vector<std::vector<std::string> > jobs, std::string jobid, int pid) {
    bool found_jobid = false;
    for (auto job : jobs) {
        if (job[0] == jobid) {
            if (!is_process_running(atoi((const char *) job[1].c_str()))) {
                continue;
            }
            found_jobid = true;
            if (job[1] != std::to_string(pid)) {
                std::cout << "The given process ID does not correspond with the given Job ID." << std::endl;
                return;
            }
            break;
        }
    }
    if (!found_jobid) {
        std::cout << "Job ID not found." << std::endl;
        return;
    }

    int result = kill(pid, SIGTERM);

    if (result == 0) {
        std::cout << "Process terminated successfully." << std::endl;
    } else {
        std::cerr << "Failed to terminate the process." << std::endl;
    }

}