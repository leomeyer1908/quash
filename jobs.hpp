#ifndef JOBS_HPP
#define JOBS_HPP
#include <vector>
#include <string>
#include <iostream>

bool is_process_running(int pid);

void jobs_cmd(std::vector<std::vector<std::string> > jobs);
#endif