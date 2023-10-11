/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Maths
*/

#include "Maths.hpp"

int Maths::decimalToIntegrerWithTwoDecimals(float num)
{
    float temp = num * 100;
    int result = static_cast<int>(temp);
    return result;
}

float Maths::integrerToDecimalWithTwoDecimals(int num)
{
    float decimal = static_cast<float>(num % 100) / 100;
    float result  = static_cast<float>(num / 100) + decimal;
    return result;
}
