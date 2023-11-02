#include <iostream>
#include <vector>
#include <sstream>
#include <string>

#include "utils.cpp"
#include "pipe.cpp"

using namespace std;

static vector<vector<string>> jobs = {};


int main()
{
    cout << "Welcome...\n";

    while (true)
    {
        freopen("/dev/tty", "r", stdin);
        freopen("/dev/tty", "w", stdout);
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
                        current_token += env_var;
                        current_variable = "";
                        i--;
                        is_storing_variable = false;
                    }
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
            current_token += env_var;
        }
        args.push_back(current_token);


        clean_input(args);

        if (args.size() == 0) {
            continue;
        }

        bool is_background_process = false;
        if (args[args.size() - 1] == "&")
        {
            is_background_process = true;
        }

        vector<vector<string>> commands;
        if (!is_background_process)
        {
            commands = split_pipe(args);
            pipeCommands(commands, jobs);
        }
        else {
            //removes the & for the actual command
            commands = split_pipe(vector<string>(args.begin(), args.end() -1));

            pid_t pid = fork();

            if (pid == -1) {
                exit(1);
            }
            else if (pid == 0) {
                pipeCommands(commands, jobs);
                exit(0);
            }
            else {
                int new_jobid = 1;

                //check if previous pid is unsued:
                for (int i = 0; i < jobs.size(); i++) {

                    if (is_process_running(atoi((const char *) jobs[i][1].c_str()))) {
                        new_jobid = i+2;
                    }
                };
                vector<string> job = vector<string>();
                if (new_jobid == jobs.size()+1) {
                    new_jobid = jobs.size()+1;
                    jobs.push_back(job);
                } else {
                    jobs.erase(jobs.begin() + new_jobid - 1);
                    jobs.insert(jobs.begin() + new_jobid - 1, job);
                }
                jobs[new_jobid - 1].push_back(to_string(new_jobid));
                jobs[new_jobid - 1].push_back(to_string(pid));
                jobs[new_jobid - 1].push_back("");
                for (int i = 0; i < args.size(); i++)
                {
                    jobs[new_jobid - 1][2] += args[i] + " ";
                }
                cout << "Background job started: [" << new_jobid << "] " << pid << " " << jobs[new_jobid - 1][2] << endl;
            }
        }
    }
    return 0;
}