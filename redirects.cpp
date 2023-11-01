#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/wait.h>

using namespace std;

void redirectInput(vector<string> cmd)
{
    // Redirect input from file
    FILE *fin = freopen(cmd[cmd.size() - 1].c_str(), "r", stdin);
    if (fin == NULL)
    {
        exit(EXIT_FAILURE);
    }
}

void redirectOutput(vector<string> cmd)
{
    // Redirect output to file
    FILE *fout = freopen(cmd[cmd.size() - 1].c_str(), "w", stdout);
    if (fout == NULL)
    {
        exit(EXIT_FAILURE);
    }
}

void redirectAppend(vector<string> cmd) 
{
    FILE *fout = freopen(cmd[cmd.size() - 1].c_str(), "a", stdout);
        if (fout == NULL)
    {
        exit(EXIT_FAILURE);
    }
}
