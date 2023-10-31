#include <iostream>
#include <vector>
#include <sstream>
#include <string>

#include "utils.cpp"
#include "pipe.cpp"
#include "redirects.cpp"

using namespace std;

static vector<vector<string>> jobs = {};


int main()
{
    cout << "Welcome...\n";

    while (true)
    {
        prefix(cout);

        vector<string> args;
        string input;

        getline(cin, input, '\n');

        int terminated_pid;
        while ((terminated_pid = waitpid(-1, nullptr, WNOHANG)) > 0) {
            for (auto job : jobs) {
                if (stoi(job[1]) == terminated_pid) {
                    cout << "Completed: [" << job[0] << "] " << job[1] << " " << job[2] << endl;
                    break;
                }
            }
        }

        bool is_strlit = false;
        bool is_storing_variable = false;
        string current_variable = "";
        string current_token;
        for (int i = 0; i < input.size(); i++)
        {
            char c = input[i];
            if (is_storing_variable)
            {
                if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
                {
                    current_variable += c;
                }
                else
                {
                    const char *env_var = getenv((const char *)current_variable.c_str());
                    if (env_var)
                    {
                        args.push_back(env_var);
                    }
                    if (c != '$')
                    {
                        cout << c;
                        is_storing_variable = false;
                    }
                    current_variable = "";
                }
            }
            else if (input[i] == '$')
            {
                is_storing_variable = true;
            }
            else if (input[i] == '"')
            {
                is_strlit = !is_strlit;
            }
            else
            {
                if (!is_strlit && input[i] == ' ')
                {
                    args.push_back(current_token);
                    current_token = "";
                }
                else if (input[i] != '\'')
                {
                    current_token += input[i];
                }
            }
        }
        const char *env_var = getenv((const char *)current_variable.c_str());
        if (env_var)
        {
            args.push_back(env_var);
        }
        args.push_back(current_token);

        clean_input(args);
        bool has_pipe = false;
        bool has_redirect_input = false;
        bool has_redirect_output = false;
        for (int i = 0; i < args.size(); i++)
        {
            if (args[i] == "|")
            {
                has_pipe = true;
                break;
            }
            if (args[i] == ">")
            {
                has_redirect_output = true;
                break;
            }
            if (args[i] == "<")
            {
                has_redirect_input = true;
                break;
            }
        }
        if (has_pipe) // handle pipes if a pipe is present in the input
        {
            vector<vector<string>> commands = split_pipe(args);

            if (commands.size() > 1)
            {
                char ***cmds = new char **[commands.size()];
                for (int i = 0; i < commands.size(); i++)
                {
                    cmds[i] = new char *[commands[i].size() + 1];
                    for (int j = 0; j < commands[i].size(); j++)
                    {
                        cmds[i][j] = new char[commands[i][j].length() + 1];
                        strcpy(cmds[i][j], commands[i][j].c_str());
                    }
                    cmds[i][commands[i].size()] = NULL;
                }

                pipeCommands(cmds, commands.size(), jobs);
                wait(NULL);
            }
            else
            {
                exec(commands[0], jobs);
            }
        }
        else if (has_redirect_input){
            redirectInput(args, jobs);
        }
        else if (has_redirect_output){
            redirectOutput(args, jobs);
        }
        else
        {
            exec(args, jobs);
        }

        
    }
    return 0;
}