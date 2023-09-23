/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** main
*/

#include <iostream>
#include <unistd.h>
#include "SystemManager.hpp"
#include "Collison.hpp"
#include "Movement.hpp"
#include "Nitwork.hpp"

int main()
{
//    Registry *registry = new Registry();
//    Registry::array<int> arrInt = registry->registerComponent<int>();
//    arrInt->add(4);
//    arrInt->add(69);
//    Registry::array<float> arrFloat = registry->registerComponent<float>();
//    arrFloat->add(69.69);
//    Registry::array<float> scdContainer = registry->getComponents<float>();
//    for (auto begin = arrInt->begin(); begin != arrInt->end(); begin++) {
//        std::cout << *begin << std::endl;
//    }
//    for (auto begin = scdContainer->begin(); begin != scdContainer->end(); begin++) {
//        std::cout << *begin << std::endl;
//    }
//
//    System::SystemManager manager;
//
//    manager.addSystem(std::make_unique<System::Collison>(registry));
//    manager.addSystem(std::make_unique<System::Movement>(registry));
//
//    while (1) {
//        manager.updateSystems();
//        sleep(1);
//    }

    Nitwork::Nitwork network;
    const bool result = network.initConnection("127.0.0.1", 4242);
    std::cout << "result: " << result << std::endl;
    return 0;
}
