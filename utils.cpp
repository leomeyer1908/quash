#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

#include "export.cpp"
#include "echo.hpp"

using namespace std;

void prefix(ostream &out)
{
    out << "[QUASH]$ ";
}

void exec(vector<string> input, vector<vector<string > > jobs){
    bool is_background_process = false;
    if (input[input.size()-1] == "&") {
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
    else
    {
        char **result = new char *[input.size() + 1];
        for (size_t i = 0; i < input.size(); i++)
        {
            result[i] = new char[input[i].length() + 1];
            strcpy(result[i], input[i].c_str());
        }
        result[input.size()] = NULL;

        std::string file = "/bin/" + input[0];
        int pid = fork();
        if (pid == 0)
        {
            int res = execvp(file.c_str(), (char *const *)result);
            if (res == -1)
            {
                cout << input[0] << ": command not found\n";
            }
        }
        else
        {
            if (!is_background_process) {
                wait(nullptr);
            } else {
                int new_jobid = 0;
                //check if previous pid is unsued:
                for (int i = 0; i < jobs.size(); i++) {
                    if (jobs[i][0] == "0") {
                        new_jobid = i+1;
                        break;
                    }
                }
                vector<string> *job = new vector<string>();
                if (new_jobid == 0) {
                    new_jobid = jobs.size();
                    jobs.push_back(*job);
                } else {
                    delete[] &jobs[new_jobid];
                    jobs[new_jobid] = *job;
                }
                jobs[new_jobid][0] = to_string(new_jobid);
                jobs[new_jobid][1] = to_string(pid);
                jobs[new_jobid][2] = "";
                for (int i = 0; i < input.size(); i++) {
                    jobs[new_jobid][2] += input[i] + " ";
                }
                jobs[new_jobid][2] += "&";
            }
        }
    }
    return;
}

void clean_input(vector<string> &input)
{
    // remove comments
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] == "#" || input[i][0] == '#')
        {
            input = {input.begin(), input.begin() + i};
            break;
        }
    }
}
