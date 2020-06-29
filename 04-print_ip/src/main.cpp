#include "print_ip.h"

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <tuple>

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
    my::print_ip(std::string{"trololo"});
    std::cout << std::endl;
    my::print_ip(std::vector{"print", "from", "vector"});
    std::cout << std::endl;
    my::print_ip(std::list{"print", "from", "list"});
    std::cout << std::endl;
    my::print_ip(std::make_tuple("print", "from", "tuple"));
    std::cout << std::endl;
}
