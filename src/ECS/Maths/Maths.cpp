/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Maths
*/

#include "Maths.hpp"
#include <cmath>
#include <stdexcept>

int Maths::floatToIntConservingDecimals(const float normalFloat)
{
    float temp = normalFloat * getMultiplier();
    int result = static_cast<int>(std::round(temp));
    return result;
}

float Maths::intToFloatConservingDecimals(const int decimalInt)
{
    float decimal = static_cast<float>(decimalInt % getMultiplier()) / getMultiplier();
    float result  = static_cast<float>(decimalInt / getMultiplier()) + decimal;
    return result;
}

int Maths::removeIntDecimals(const int decimalInt)
{
    return decimalInt / getMultiplier();
}

int Maths::addIntDecimals(const int normalInt)
{
    return normalInt * getMultiplier();
}

int Maths::additionWithTwoIntDecimals(const int decimalInt, const int otherDecimalInt)
{
    float tempA  = intToFloatConservingDecimals(decimalInt);
    float tempB  = intToFloatConservingDecimals(otherDecimalInt);
    float result = tempA + tempB;
    return floatToIntConservingDecimals(result);
}

int Maths::subtractionWithTwoIntDecimals(const int minuend, const int subtrahend)
{
    float tempA  = intToFloatConservingDecimals(minuend);
    float tempB  = intToFloatConservingDecimals(subtrahend);
    float result = tempA - tempB;
    return floatToIntConservingDecimals(result);
}

int Maths::multiplicationWithTwoIntDecimals(const int decimalInt, const int otherDecimalInt)
{
    float tempA  = intToFloatConservingDecimals(decimalInt);
    float tempB  = intToFloatConservingDecimals(otherDecimalInt);
    float result = tempA * tempB;
    return floatToIntConservingDecimals(result);
}

int Maths::divisionWithTwoIntDecimals(const int dividend, const int divisor)
{
    if (divisor == 0) {
        throw std::overflow_error("Division by zero");
    } else {
        float tempA  = intToFloatConservingDecimals(dividend);
        float tempB  = intToFloatConservingDecimals(divisor);
        float result = tempA / tempB;
        return floatToIntConservingDecimals(result);
    }
}

void Maths::addNormalIntToDecimalInt(int &decimalInt, const int normalInt)
{
    decimalInt += addIntDecimals(normalInt);
}

void Maths::subNormalIntToDecimalInt(int &decimalInt, const int normalInt)
{
    decimalInt -= addIntDecimals(normalInt);
}

void Maths::addFloatToDecimalInt(int &decimalInt, const float normalFloat)
{
    decimalInt += floatToIntConservingDecimals(normalFloat);
}

void Maths::subFloatToDecimalInt(int &decimalInt, const float normalFloat)
{
    decimalInt -= floatToIntConservingDecimals(normalFloat);
}
