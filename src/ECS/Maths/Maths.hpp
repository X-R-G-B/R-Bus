/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Maths
*/

#pragma once

// The DECIMALS_TO_CONSERVE constant is used to specify the number of decimal places
// to preserve when converting floating-point numbers to integers and vice versa. By
// conserving a specific number of decimals, we can represent certain data types as
// integers rather than floats, reducing the overall data size and making the network
// communication more efficient.

namespace Maths {

    static constexpr int DECIMALS_TO_CONSERVE = 2;

    /**
     * @brief Calculates a multiplier based on the DECIMALS_TO_CONSERVE constant.
     * @return The calculated multiplier.
     * @example If DECIMALS_TO_CONSERVE is 2, the function returns 100.
     */
    constexpr int getMultiplier()
    {
        int result = 1;

        for (int i = 0; i < DECIMALS_TO_CONSERVE; i++) {
            result *= 10;
        }
        return result;
    }

    /**
     * @brief convert a float to an int conserving the two first decimals
     * @param normalFloat the float to convert
     * @return the int
     * @example 99.99 -> 9999
     *
     */
    int floatToIntConservingDecimals(float normalFloat);

    /**
     * @brief convert an int to a float with two decimals
     * @param decimalInt the int to convert
     * @return the float
     * @example 9999 -> 99.99
     *
     */
    float intToFloatConservingDecimals(int decimalInt);

    /**
     * @brief remove the decimals of an int
     * @param decimalInt the int to modify
     * @return the int
     * @example 9999 -> 99
     *
     */
    int removeIntDecimals(int decimalInt);

    /**
     * @brief add the decimals to an int
     * @param normalInt the int to modify
     * @return the int
     * @example 99 -> 9900
     *
     */
    int addIntDecimals(int normalInt);

    /**
     * @brief addition of two ints with two decimals
     * @param decimalInt the first int
     * @param otherDecimalInt the second int
     * @return the result of the addition
     * @example 9999 + 9999 -> 199.98
     *
     */
    int additionWithTwoIntDecimals(int decimalInt, int otherDecimalInt);

    /**
     * @brief subtraction of two ints with two decimals
     * @param minuend the substraction minuend
     * @param subtrahend the substraction subtrahend
     * @return the result of the subtraction
     * @example 9999 - 9999 -> 0
     *
     */
    int subtractionWithTwoIntDecimals(int minuend, int subtrahend);

    /**
     * @brief multiplication of two ints with two decimals
     * @param decimalInt the first int
     * @param otherDecimalInt the second int
     * @return the result of the multiplication
     *
     */
    int multiplicationWithTwoIntDecimals(int decimalInt, int otherDecimalInt);

    /**
     * @brief division of two int with two decimals
     * @param dividend the first int
     * @param divisor the second int
     * @return the result of the division
     *
     */
    int divisionWithTwoIntDecimals(int dividend, int divisor);

    /**
     * @brief multiplication of two ints with two decimals
     * @param decimalInt the first int
     * @param normalInt the second int
     * @return the result of the multiplication
     * @example 500 + 5 = 550
     */
    int addNormalIntToDecimalInt(int decimalInt, int normalInt);

    /**
     * @brief multiplication of two ints with two decimals
     * @param decimalInt the first int
     * @param normalInt the second int
     * @return the result of the multiplication
     * @example 550 - 5 = 500
     */
    int subNormalIntToDecimalInt(int decimalInt, int normalInt);

} // namespace Maths
