#include "NitworkServer.hpp"

int main()
{
    try {
        Nitwork::NitworkServer::getInstance().start(4242);
        while (true) {}
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
