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
     * @brief convert a float to an int conserving the x first decimals
     * @param normalFloat the float to convert
     * @return int
     * @example 99.99 -> 9999 for DECIMALS_TO_CONSERVE = 2
     *
     */
    int floatToIntConservingDecimals(const float normalFloat);

    /**
     * @brief convert an int to a float with x decimals
     * @param decimalInt the int to convert
     * @return float
     * @example 9999 -> 99.99 for DECIMALS_TO_CONSERVE = 2
     *
     */
    float intToFloatConservingDecimals(const int decimalInt);

    /**
     * @brief remove the decimals of an int
     * @param decimalInt the int to modify
     * @return int
     * @example 9999 -> 99 for DECIMALS_TO_CONSERVE = 2
     *
     */
    int removeIntDecimals(const int decimalInt);

    /**
     * @brief add the x decimals to an int
     * @param normalInt the int to modify
     * @return int with x decimals
     * @example 99 -> 9900 for DECIMALS_TO_CONSERVE = 2
     *
     */
    int addIntDecimals(const int normalInt);

    /**
     * @brief addition of two ints with x decimals
     * @param decimalInt decimalInt the first int
     * @param otherDecimalInt decimalInt the second int
     * @return the result of the addition
     * @example 9999 + 9999 -> 199.98 for DECIMALS_TO_CONSERVE = 2
     *
     */
    int additionWithTwoIntDecimals(const int decimalInt, const int otherDecimalInt);

    /**
     * @brief subtraction of two ints with x decimals
     * @param minuend int the substraction minuend
     * @param subtrahend int the substraction subtrahend
     * @return the result of the subtraction
     * @example 9999 - 9999 -> 0 for DECIMALS_TO_CONSERVE = 2
     *
     */
    int subtractionWithTwoIntDecimals(const int minuend, const int subtrahend);

    /**
     * @brief multiplication of two ints with x decimals
     * @param decimalInt decimalInt the first int
     * @param otherDecimalInt decimalInt the first int
     * @return the result of the multiplication
     *
     */
    int multiplicationWithTwoIntDecimals(const int decimalInt, const int otherDecimalInt);

    /**
     * @brief division of two int with x decimals
     * @param dividend decimalInt the first int
     * @param divisor decimalInt the first int
     * @return the result of the division
     *
     */
    int divisionWithTwoIntDecimals(const int dividend, const int divisor);

    /**
     * @brief addition with a decimal int and a normal int
     * @param decimalInt decimalInt the decimal int that will be modified
     * @param normalInt normalInt the normal int that will be added
     * @return void
     * @example 500 + 5 = 550 for DECIMALS_TO_CONSERVE = 2
     */
    void addNormalIntToDecimalInt(int &decimalInt, const int normalInt);

    /**
     * @brief subtraction with a decimal int and a normal int
     * @param decimalInt decimalInt the decimal int that will be modified
     * @param normalInt the normal int that will be subtracted
     * @return void
     * @example 550 - 5 = 500 for DECIMALS_TO_CONSERVE = 2
     */
    void subNormalIntToDecimalInt(int &decimalInt, const int normalInt);

    /**
     * @brief add a float to an int with decimals
     * @param decimalInt decimalInt the decimal int that will be modified
     * @param normalFloat the normal float that will be added
     * @return nothing
     * @example 500 + 5.5 = 555 for DECIMALS_TO_CONSERVE = 2
     */
    void addFloatToDecimalInt(int &decimalInt, const float normalFloat);

    /**
     * @brief sub a float to an int with decimals
     * @param decimalInt decimalInt the decimal int that will be modified
     * @param normalFloat the normal float that will be subtracted
     * @return nothing
     * @example 500 - 5.5 = 445 for DECIMALS_TO_CONSERVE = 2
     */
    void subFloatToDecimalInt(int &decimalInt, const float normalFloat);

    /**
     * @brief get the angle from a vector
     * @param x the x of the vector
     * @param y the y of the vector
     * @return float
     */
    float getAngleFromVector(const float x, const float y);

    /**
     * @brief convert degrees to radians
     * @param degrees the degrees to convert
     * @return float
     */
    float degreesToRadians(const float degrees);

} // namespace Maths
