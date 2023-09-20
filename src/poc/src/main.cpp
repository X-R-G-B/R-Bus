#include <iostream>
#include "Registry.hpp"

int main()
{
    Registry *registry = new Registry();
    Registry::array<int> arrInt = registry->registerComponent<int>();
    arrInt->add(4);
    arrInt->add(69);
    Registry::array<float> arrFloat = registry->registerComponent<float>();
    arrFloat->add(69.69);
    Registry::array<float> scdContainer = registry->getComponents<float>();
    for (auto begin = arrInt->begin(); begin != arrInt->end(); begin++) {
        std::cout << *begin << std::endl;
    }
    for (auto begin = scdContainer->begin(); begin != scdContainer->end(); begin++) {
        std::cout << *begin << std::endl;
    }


    return 0;
}
