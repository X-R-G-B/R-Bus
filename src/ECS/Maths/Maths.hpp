/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Maths
*/

#pragma once

namespace Maths {

    /*
     * @brief convert a float to an int conserving the two first decimals
     * @param num the float to convert
     * @return the int
     * @example 99.99 -> 9999
     *
     */
    int decimalToIntegrerWithTwoDecimals(float num);

    /*
     * @brief convert an int to a float with two decimals
     * @param num the int to convert
     * @return the float
     * @example 9999 -> 99.99
     *
     */
    float integrerToDecimalWithTwoDecimals(int num);

} // namespace Maths
