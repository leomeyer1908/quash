#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

#include "export.cpp"
#include "echo.hpp"
#include "cd.hpp"
#include "jobs.hpp"
#include "kill.hpp"

using namespace std;

void prefix(ostream &out)
{
    out << "[QUASH]$ ";
}

vector<string> charToStringList(char **input)
{
    vector<string> result;
    for (int i = 0; input[i] != NULL; i++)
    {
        result.push_back(input[i]);
    }
    return result;
}

int exec(vector<string> input, vector<vector<string>> &jobs)
{
    bool is_background_process = false;
    if (input[input.size() - 1] == "&")
    {
        is_background_process = true;
        input.pop_back();
    }

    if (input[0] == "exit" || input[0] == "quit")
    {
        exit(0);
    }
    else if (input[0] == "export")
    {
        exportFunction(input[1]);
    }
    else if (input[0] == "echo")
    {
        string result;
        for (int i = 1; i < input.size(); i++)
        {
            result += input[i] + " ";
        }
        result.pop_back();
        echoFunction(result);
    }
    else if (input[0] == "cd")
    {
        cd(input[1]);
    }
    else if (input[0] == "jobs")
    {
        jobs_cmd(jobs);
    }
    else if (input[0] == "kill")
    {
        kill_cmd(jobs, input[1], stoi(input[2]));
    }
    else
    {
        char **result = new char *[input.size() + 1];
        for (size_t i = 0; i < input.size(); i++)
        {
            result[i] = new char[input[i].length() + 1];
            strcpy(result[i], input[i].c_str());
        }
        result[input.size()] = NULL;

        std::string file = input[0];
        int pid = fork();
        if (pid == 0)
        {
            int res = execvp(file.c_str(), (char *const *)result);
            if (res == -1)
            {
                cout << input[0] << ": command not found\n";
                return 1;
            }
        }
        else
        {
            if (!is_background_process)
            {
                wait(nullptr);
            }
            else
            {
                int new_jobid = 0;

                //check if previous pid is unsued:
                for (int i = 0; i < jobs.size(); i++) {

                    if (jobs[i][0] == "0" || !is_process_running(atoi((const char *) jobs[i][1].c_str()))) {
                        new_jobid = i+1;
                        break;
                    }
                };
                vector<string> job = vector<string>();
                if (new_jobid == 0) {
                    new_jobid = jobs.size()+1;
                    jobs.push_back(job);
                } else {
                    jobs.erase(jobs.begin() + new_jobid - 1);
                    jobs.insert(jobs.begin() + new_jobid - 1, job);
                }
                jobs[new_jobid - 1].push_back(to_string(new_jobid));
                jobs[new_jobid - 1].push_back(to_string(pid));
                jobs[new_jobid - 1].push_back("");
                for (int i = 0; i < input.size(); i++)
                {
                    jobs[new_jobid - 1][2] += input[i] + " ";
                }
                jobs[new_jobid - 1][2] += "&";
                cout << "Background job started: [" << new_jobid << "] " << pid << " " << jobs[new_jobid - 1][2] << endl;
                // //create a new process to print when background process finishes
                // int checker_pid = fork();
                // if (checker_pid == 0) {
                //     auto result = waitpid(pid, NULL, 0);
                //     cout << "FORK 1:" << pid << endl;
                //     cout << "RESULT: "<< result << endl;
                //     for (auto job : jobs) {
                //         if (stoi(job[1]) == pid) {
                //             cout << "Completed: [" << job[0] << "] " << job[1] << " " << job[2] << endl;
                //             break;
                //         }
                //     }
                //     exit(0);
                // }
            }
        }
        for (size_t i = 0; i < input.size(); i++)
        {
            delete[] result[i];
        }
        delete[] result;
    }
    return 0;
}

void clean_input(vector<string> &input)
{
    // remove comments, whitespace, and empty strings
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] == "#" || input[i][0] == '#')
        {
            input = {input.begin(), input.begin() + i};
            break;
        }
        if (input[i] == "" || input[i] == " ")
        {
            input.erase(input.begin() + i);
            i -= 1;
        }
    }
}

vector<vector<string>> split_pipe(vector<string> input)
{
    vector<vector<string>> result;
    vector<string> current_command;
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] == "|")
        {
            result.push_back(current_command);
            current_command.clear();
        }
        else
        {
            current_command.push_back(input[i]);
        }
    }
    result.push_back(current_command);

    return result;
}