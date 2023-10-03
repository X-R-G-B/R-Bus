#include "NitworkServer.hpp"

int main()
{
    try {
        std::cout << "Starting server..." << std::endl;
        NitworkServer::NitworkServer::getInstance().start(4242);
        std::cout << "Server started." << std::endl;
        while (true);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
