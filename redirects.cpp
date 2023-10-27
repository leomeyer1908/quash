#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/wait.h>

using namespace std;

void redirectInput(vector<string> cmd, vector<vector<string>> jobs)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        // Redirect input from file
        FILE *fin = freopen(cmd[cmd.size() - 1].c_str(), "r", stdin);
        if (fin == NULL)
        {
            exit(EXIT_FAILURE);
        }

        // Remove input file name from command arguments
        cmd.pop_back();
        cmd.pop_back();

        // Convert vector<string> to char**
        char **args = new char *[cmd.size() + 1];
        for (int i = 0; i < cmd.size(); i++)
        {
            args[i] = new char[cmd[i].length() + 1];
            strcpy(args[i], cmd[i].c_str());
        }
        args[cmd.size()] = NULL;

        // Execute command
        exec(cmd, jobs);
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        // Parent process
        waitpid(pid, NULL, 0);
    }
    else
    {
        exit(EXIT_FAILURE);
    }
}

void redirectOutput(vector<string> cmd, vector<vector<string>> jobs)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        // Child process 1: handle redirection

        // Redirect output to file
        FILE *fout = freopen(cmd[cmd.size() - 1].c_str(), "w", stdout);
        if (fout == NULL)
        {
            exit(EXIT_FAILURE);
        }

        // Remove output file name from command arguments
        cmd.pop_back();
        cmd.pop_back();

        // Convert vector<string> to char**
        char **args = new char *[cmd.size() + 1];
        for (int i = 0; i < cmd.size(); i++)
        {
            args[i] = new char[cmd[i].length() + 1];
            strcpy(args[i], cmd[i].c_str());
        }
        args[cmd.size()] = NULL;

        // Execute command
        // execvp(args[0], args);
        exec(cmd, jobs);
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        // Parent process
        waitpid(pid, NULL, 0);
    }
    else
    {
        exit(EXIT_FAILURE);
    }
}
