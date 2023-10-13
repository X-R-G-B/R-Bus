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

int Maths::removeIntegerDecimals(int num)
{
    return num / 100;
}

int Maths::addIntegerDecimals(int num)
{
    return num * 100;
}

int Maths::additionWithDecimal(int a, int b)
{
    float tempA = integrerToDecimalWithTwoDecimals(a);
    float tempB = integrerToDecimalWithTwoDecimals(b);
    float result = tempA + tempB;
    return decimalToIntegrerWithTwoDecimals(result);
}

int Maths::subtractionWithDecimal(int a, int b)
{
    float tempA = integrerToDecimalWithTwoDecimals(a);
    float tempB = integrerToDecimalWithTwoDecimals(b);
    float result = tempA - tempB;
    return decimalToIntegrerWithTwoDecimals(result);
}

int Maths::multiplicationWithDecimal(int a, int b)
{
    float tempA = integrerToDecimalWithTwoDecimals(a);
    float tempB = integrerToDecimalWithTwoDecimals(b);
    float result = tempA * tempB;
    return decimalToIntegrerWithTwoDecimals(result);
}

int Maths::divisionWithDecimal(int a, int b)
{
    float tempA = integrerToDecimalWithTwoDecimals(a);
    float tempB = integrerToDecimalWithTwoDecimals(b);
    float result = tempA / tempB;
    return decimalToIntegrerWithTwoDecimals(result);
}
