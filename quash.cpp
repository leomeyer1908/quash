#include <iostream>
#include <vector>
#include <sstream>
#include <string>

#include "utils.cpp"

using namespace std;

int main()
{
    cout << "Welcome...\n";
    string input;
    vector<string> stream;
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
            cout << word << "\n";
        }

        // exec(&*input.begin());
    }
}