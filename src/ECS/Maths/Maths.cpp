/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Maths
*/

#include "Maths.hpp"

int Maths::floatToIntConservingDecimals(float normalFloat)
{
    float temp = normalFloat * getMultiplier();
    int result = static_cast<int>(temp);
    return result;
}

float Maths::intToFloatConservingDecimals(int decimalInt)
{
    float decimal = static_cast<float>(decimalInt % getMultiplier()) / getMultiplier();
    float result  = static_cast<float>(decimalInt / getMultiplier()) + decimal;
    return result;
}

int Maths::removeIntDecimals(int decimalInt)
{
    return decimalInt / getMultiplier();
}

int Maths::addIntDecimals(int normalInt)
{
    return normalInt * getMultiplier();
}

int Maths::additionWithTwoIntDecimals(int decimalInt, int otherDecimalInt)
{
    float tempA  = intToFloatConservingDecimals(decimalInt);
    float tempB  = intToFloatConservingDecimals(otherDecimalInt);
    float result = tempA + tempB;
    return floatToIntConservingDecimals(result);
}

int Maths::subtractionWithTwoIntDecimals(int minuend, int subtrahend)
{
    float tempA  = intToFloatConservingDecimals(minuend);
    float tempB  = intToFloatConservingDecimals(subtrahend);
    float result = tempA - tempB;
    return floatToIntConservingDecimals(result);
}

int Maths::multiplicationWithTwoIntDecimals(int decimalInt, int otherDecimalInt)
{
    float tempA  = intToFloatConservingDecimals(decimalInt);
    float tempB  = intToFloatConservingDecimals(otherDecimalInt);
    float result = tempA * tempB;
    return floatToIntConservingDecimals(result);
}

int Maths::divisionWithTwoIntDecimals(int dividend, int divisor)
{
    float tempA  = intToFloatConservingDecimals(dividend);
    float tempB  = intToFloatConservingDecimals(divisor);
    float result = tempB == 0 ? tempA : tempA / tempB;
    return floatToIntConservingDecimals(result);
}

int Maths::addNormalIntToDecimalInt(int decimalInt, int normalInt)
{
    return decimalInt + addIntDecimals(normalInt);
}

int Maths::subNormalIntToDecimalInt(int decimalInt, int normalInt)
{
    return decimalInt - addIntDecimals(normalInt);
}
