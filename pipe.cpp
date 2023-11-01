#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/wait.h>

#include "redirects.cpp"

using namespace std;

void pipeCommands(vector<vector<string>> commands, vector<vector<string>> jobs) {

    if (commands.size() > 1) {
        int fds[commands.size() - 1][2]; // One less pipe than commands

        // Create pipes for each command except the last one
        for (int i = 0; i < commands.size() - 1; i++) {
            if (pipe(fds[i]) == -1) {
                exit(1);
            }
        }

        for (int i = 0; i < commands.size(); i++) {
            for (int j = commands[i].size()-1; j >= 0; j--)
            {
                if (commands[i][j] == ">")
                {
                    redirectOutput(commands[i]);
                    commands[i].pop_back();
                    commands[i].pop_back();
                }
                if (commands[i][j] == "<")
                {
                    redirectInput(commands[i]);
                    commands[i].pop_back();
                    commands[i].pop_back();
                }
                if (commands[i][j] == ">>")
                {
                    redirectAppend(commands[i]);
                    commands[i].pop_back();
                    commands[i].pop_back();
                }
            }
            pid_t pid = fork();

            if (pid == -1) {
                exit(1);
            }
            else if (pid == 0) {
                // Child process
                // If not the first command, read from the previous pipe
                if (i != 0) {
                    dup2(fds[i - 1][0], STDIN_FILENO);
                }

                // If not the last command, write to the next pipe
                if (i != commands.size() - 1) {
                    dup2(fds[i][1], STDOUT_FILENO);
                }
                // Close all fds
                for (int j = 0; j < commands.size() - 1; j++) {
                    close(fds[j][0]);
                    close(fds[j][1]);
                }

                
                // Execute the command
                int res = exec(commands[i], jobs);
                if (res == -1) {
                    exit(1);
                }
                else {
                    exit(0);
                }
            }
            // else {
            //     wait(NULL);
            // }
        }
        // Close all fds
        for (int j = 0; j < commands.size() - 1; j++) {
            close(fds[j][0]);
            close(fds[j][1]);
        }
        // Wait for all children to finish
        for (int i = 0; i < commands.size(); i++) {
            wait(NULL);
        }
    }
    else {
        for (int i = commands[0].size()-1; i >= 0; i--)
        {
            if (commands[0][i] == ">")
            {
                redirectOutput(commands[0]);
                commands[0].pop_back();
                commands[0].pop_back();
            }
            if (commands[0][i] == "<")
            {
                redirectInput(commands[0]);
                commands[0].pop_back();
                commands[0].pop_back();
            }
            if (commands[0][i] == ">>")
            {
                redirectAppend(commands[0]);
                commands[0].pop_back();
                commands[0].pop_back();
            }
        }
        int res = exec(commands[commands.size()-1], jobs);
        if (res == -1) {
            return;
        }
    }
}




