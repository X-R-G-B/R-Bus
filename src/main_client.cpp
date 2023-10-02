/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** main
*/

#include "NitworkClient.hpp"

int main()
{
    Nitwork::NitworkClient::getInstance().start(4242);

    Nitwork::NitworkClient::getInstance().addInitMsg();
    Nitwork::NitworkClient::getInstance().addInitMsg();
    while (true);
    return 0;
}
