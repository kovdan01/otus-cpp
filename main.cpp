#include "lib.h"
#include <iostream>
#include <string>

int main()
{
    auto [major, minor, patch] = version();
    std::cout << message() << "\nVersion "
              << major << '.' << minor << '.' << patch << '\n';
    return 0;
}
