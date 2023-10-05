#include <iostream>
#include <cstdlib>
#include <string>

#include "export.cpp"

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