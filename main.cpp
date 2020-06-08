#include "print_ip.h"
#include <iostream>

int main()
{
    my::print_ip(char(-1));
    std::cout << std::endl;
    my::print_ip(short(0));
    std::cout << std::endl;
    my::print_ip(int(2130706433));
    std::cout << std::endl;
    my::print_ip(long(8875824491850138409));
    std::cout << std::endl;
}
