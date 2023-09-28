#include "boost/asio.hpp"
#include "Nitwork.hpp"

int main()
{
    try {
        Nitwork::Nitwork::getInstance().start(4242);
        while (Nitwork::Nitwork::getInstance().isRunning());
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
