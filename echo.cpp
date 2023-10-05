#include "echo.hpp"

void echo(std::string input) {
    // bool is_storing_variable = false;
    // std::string current_variable = "";
    // bool is_in_single_quote = false;
    // bool is_in_double_quote = false;
    // for (char c : input) {
    //     if (is_storing_variable) {
    //         if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
    //             current_variable += c;
    //         } else {
    //             //RETRIVE VARIABLE HERE
    //             const char* env_var = std::getenv((const char*) current_variable.c_str());
    //             if (env_var) {
    //                 std::cout << env_var;
    //             }
    //             if (c != '$') {
    //                 std::cout << c;
    //                 is_storing_variable = false;
    //             }
    //             current_variable = "";
    //         }
    //     }
    //     else if (c == '\'') {
    //         if (is_in_single_quote) {
    //             is_in_single_quote = false;
    //         } else {
    //             is_in_single_quote = true;
    //         }
    //     }
    //     else if (c == '"') {
    //         if (is_in_double_quote) {
    //             is_in_double_quote = false;
    //         } else {
    //             is_in_double_quote = true;
    //         }
    //     }
    //     else if (c == '$' && !is_in_single_quote) {

    //         is_storing_variable = true;
    //     }
    //     else {
    //         std::cout << c;
    //     }
    // }
    // if (is_storing_variable) {
    //     std::cout << std::getenv(current_variable.c_str());
    // }
    std::cout << input << std::endl;
}
