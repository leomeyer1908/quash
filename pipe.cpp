#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/wait.h>

using namespace std;

void pipeCommands(char ***cmds, int num_cmds, vector<vector<string>> jobs) {

    int fds[num_cmds - 1][2]; // One less pipe than commands

    // Create pipes for each command except the last one
    for (int i = 0; i < num_cmds - 1; i++) {
        if (pipe(fds[i]) == -1) {
            exit(1);
        }
    }

    for (int i = 0; i < num_cmds; i++) {
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
            if (i != num_cmds - 1) {
                dup2(fds[i][1], STDOUT_FILENO);
            }

            // Close all fds
            for (int j = 0; j < num_cmds - 1; j++) {
                close(fds[j][0]);
                close(fds[j][1]);
            }

            // Execute the command
            int res = execvp(cmds[i][0], cmds[i]);
            if (res == -1) {
                cout << cmds[i][0] << ": command not found\n";
                return;
            }
        }
    }

    // Close all fds
    for (int j = 0; j < num_cmds - 1; j++) {
        close(fds[j][0]);
        close(fds[j][1]);
    }

    // Wait for all children to finish
    for (int i = 0; i < num_cmds; i++) {
        wait(NULL);
    }
}




