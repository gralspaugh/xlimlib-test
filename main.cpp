#include <iostream>
#include <string>

#include "XlimlibUtils.h"
#include "Config.h"

int main(int, char**) {
    // No input arguments - just print function 
    std::cout << "Hello from xlimlib-test and " 
        << std::endl;
    testroutines::say_hello();
    return 0;
}
