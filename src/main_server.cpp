#include "NitworkServer.hpp"

int main()
{
    int port = 4242;

    try {
        Nitwork::NitworkServer::getInstance().start(port);
        while (true) { }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
