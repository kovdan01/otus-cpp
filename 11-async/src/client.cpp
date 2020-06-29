#include "async.h"

int main()
{
    std::size_t bulk = 5;
    async::handle_t h = async::connect(bulk);
    async::handle_t h2 = async::connect(bulk);
    async::receive(h, "1", 1);
    async::receive(h2, "1\n", 2);
    async::receive(h, "", 0);
    async::receive(h, "\n", 1);
    async::receive(h, "2\n3\n4\n5\n6\n{\na\n", 14);
    async::receive(h, "b\nc\nd\n}\n89\n", 11);
    async::disconnect(h);
    async::disconnect(h2);

    return 0;
}

