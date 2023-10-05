#include <iostream>
#include <vector>
#include <sstream>
#include <string>

#include "utils.cpp"

using namespace std;

int main()
{
    cout << "Welcome...\n";

    while (true)
    {
        prefix(cout);

        vector<string> args;
        string input;

        getline(cin, input, '\n');
        bool is_strlit = false;
        string current_token;
        for(int i = 0; i < input.size(); i++){
            if (input[i] == '"'){
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

        exec(args);
    }
}