#include <iostream>
#include <vector>
#include <sstream>
#include <string>

#include "utils.cpp"

using namespace std;

int main()
{
    cout << "Welcome...\n";

    vector<string> jobs;

    while (true)
    {
        prefix(cout);

        vector<string> args;
        string input;

        getline(cin, input, '\n');
        bool is_strlit = false;
        bool is_storing_variable = false;
        string current_variable = "";
        string current_token;
        for(int i = 0; i < input.size(); i++){
            char c = input[i];
            if (is_storing_variable) {
                if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                    current_variable += c;
                } else {
                    const char* env_var = getenv((const char*) current_variable.c_str());
                    if (env_var) {
                        args.push_back(env_var);
                    }
                    if (c != '$') {
                        cout << c;
                        is_storing_variable = false;
                    }
                    current_variable = "";
                }
            }
            else if (input[i] == '$') {
                is_storing_variable = true;
            }
            else if (input[i] == '"'){
                is_strlit = !is_strlit;
            }
            else{
                if (!is_strlit && input[i] == ' ' && current_token.length() > 0){
                    args.push_back(current_token);
                    current_token = "";
                }else if (input[i] != '\''){
                    current_token += input[i];
                }
            }
        }
        const char* env_var = getenv((const char*) current_variable.c_str());
        if (env_var) {
            args.push_back(env_var);
        }
        args.push_back(current_token);
        // istringstream ss(input);
        // string word;
        // while (ss >> word){
        //     args.push_back(word);
        // }

        clean_input(args);
        
        // for (int i = 0; i < args.size(); i++){
        //     cout << args[i] << " ";
        // }

        exec(args, jobs);
    }
}