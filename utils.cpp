#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

#include "export.cpp"
#include "echo.hpp"

using namespace std;

void prefix(ostream& out){
    out << "[QUASH]$ ";
}

void exec(vector<string> input){
    if (input[0] == "exit" || input[0] == "quit")
    {
        exit(0);
    }else if (input[0] == "export"){
        exportFunction(input[1]);
    }else if (input[0] == "echo"){
        string result;
        for(int i = 1; i < input.size(); i++){
            result += input[i] + " ";
        }
        result.pop_back();
        echoFunction(result);
        
    }else if (input[0] == "ls"){
        char* result[input.size() + 1];

        // result[0] = (char *)"ls";

        for(int i = 0; i < input.size(); i++){
            result[i] = (char *)input[i].c_str();
        }
        result[input.size() - 1] = NULL;
        // result.pop_back();
        cout << result[0] << "\n";
        cout << result[1] << "\n";
        cout << result[2] << "\n";
        char* argument_list[] = {"ls", "-l", NULL};
        if (fork() == 0){
            execvp("/bin/ls", result);
        }else{
            wait(nullptr);
        }
    }
    return;
}

void clean_input(vector<string>& input){
    // remove comments
    for (int i = 0; i < input.size(); i++){
        if (input[i] == "#" || input[i][0] == '#'){
            // cout << "comment found\n";
            input = {input.begin(), input.begin() + i};
            break;
        }
    }
}