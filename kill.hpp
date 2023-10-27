#include <vector>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <signal.h>
#include "jobs.hpp"

void kill_cmd(std::vector<std::vector<std::string> > jobs, std::string jobid, int pid);
