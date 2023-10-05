#include <iostream>
#include <unistd.h> // Include the header file for chdir

int main() {
    const char* newDirectory = "/path/to/your/new/directory";

    // Use chdir to change the current working directory
    if (chdir(newDirectory) != 0) {
        std::cerr << "Error changing directory to " << newDirectory << std::endl;
        return 1; // Return an error code
    }

    // At this point, the current working directory has been changed successfully

    // You can now perform operations in the new working directory

    return 0;
}