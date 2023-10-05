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
        istringstream ss(input);
        string word;
        while (ss >> word){
            args.push_back(word);
        }
        clean_input(args);
        
        // for (int i = 0; i < args.size(); i++){
        //     cout << args[i] << " ";
        // }

        exec(args);
    }
}