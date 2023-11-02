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
#include "pwd.hpp"
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
    if (input[0] == "exit" || input[0] == "quit")
    {
        exit(0);
    }
    else if (input[0] == "export")
    {
        if (input.size() > 1){
            exportFunction(input[1]);
        }
            
    }
    else if (input[0] == "echo")
    {
        string result;
        for (int i = 1; i < input.size(); i++)
        {
            result += input[i] + " ";
        }
        if (result.size() > 0) {
            result.pop_back();
        }
        echoFunction(result);
    }
    else if (input[0] == "cd")
    {
        if (input.size() > 1){
            cd(input[1]);
        }
    }
    else if (input[0] == "pwd")
    {
        pwd();
    }
    else if (input[0] == "jobs")
    {
        jobs_cmd(jobs);
    }
    else if (input[0] == "kill")
    {
        if (input.size() > 2) {
            kill_cmd(jobs, input[1], stoi(input[2]));
        }   
        else {
            cout << "Usage: kill <JOBID> <PID>" << endl;
        }
    }
    else
    {
        pid_t pid = fork();

        if (pid == -1) {
            exit(1);
        }
        else if (pid == 0) {
            char **result = new char *[input.size() + 1];
            for (size_t i = 0; i < input.size(); i++)
            {
                result[i] = new char[input[i].length() + 1];
                strcpy(result[i], input[i].c_str());
            }
            result[input.size()] = NULL;

            std::string file = input[0];
            int res = execvp(file.c_str(), (char *const *)result);
            if (res == -1)
            {
                cout << input[0] << ": command not found\n";
                return -1;
            }
            for (size_t i = 0; i < input.size(); i++)
            {
                delete[] result[i];
            }
            delete[] result;
        }
        else {
            wait(NULL);
        }
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