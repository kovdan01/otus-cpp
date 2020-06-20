#include "controller.h"

#include <iostream>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "argc not 2" << std::endl;
        return 1;
    }

    my::Controller controller(5, 3);
    std::size_t ans = controller.work(argv[1]);
    if (ans == std::size_t(-1))
        std::cout << "Cannot detect minimum prefix" << std::endl;
    else
        std::cout << "Minimum prefix " << ans << std::endl;
}
