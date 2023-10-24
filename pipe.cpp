#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/wait.h>

using namespace std;

void pipeCommands(char ***cmds, int num_cmds, vector<string> jobs)
{
    int fds[2]; // file descriptors
    pipe(fds);

    if (fork() == 0)
    {
        // Reassign stdout to fds[1] end of pipe.
        dup2(fds[1], STDOUT_FILENO);
        close(fds[0]);
        close(fds[1]);
        // Execute the first command.
        execvp(cmds[0][0], cmds[0]);
    }
    for (int i = 1; i < num_cmds - 1; i++)
    {
        if (fork() == 0)
        {
            // Reassign stdin to fds[0] end of pipe.
            wait(NULL);
            dup2(fds[0], STDIN_FILENO);
            dup2(fds[1], STDOUT_FILENO);
            close(fds[0]);
            close(fds[1]);
            // Execute the second command.
            execvp(cmds[i][0], cmds[i]);
            // exec(charToStringList(cmds[i]), jobs);
        }
    }
    if (fork() == 0)
    {
        // Reassign stdin to fds[0] end of pipe.
        wait(NULL);
        dup2(fds[0], STDIN_FILENO);
        close(fds[1]);
        close(fds[0]);
        // Execute the second command.
        // exec(charToStringList(cmds[num_cmds - 1]), jobs);
        execvp(cmds[num_cmds - 1][0], cmds[num_cmds - 1]);
    }
    close(fds[1]);
    close(fds[0]);
    wait(NULL);
}