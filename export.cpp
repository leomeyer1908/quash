#include <cstdlib>
#include <string>
#include <stdlib.h>
#include <iostream>

void exportFunction(std::string arg){
    std::string name;
    std::string value;
    bool name_complete = false;
    for (int i = 0; i < arg.length(); i++){
        if (name_complete){
            value += arg[i];
        }
        else{
            if (arg[i] == '='){
                name_complete = true;
            }else{
                name += arg[i];
            }
        }
    }

    setenv((char*)name.c_str(), (char*)value.c_str(), true);
}