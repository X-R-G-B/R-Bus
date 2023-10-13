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

    /*
     * @brief remove the decimals of an int
     * @param num the int to return
     * @return the int
     * @example 9999 -> 99
     *
     */
    int removeIntegerDecimals(int num);

    /*
     * @brief add the decimals of an int
     * @param num the int to return
     * @return the int
     * @example 99 -> 9900
     *
     */
    int addIntegerDecimals(int num);

    /*
     * @brief addition of two int with two decimals
     * @param a the first int
     * @param b the second int
     * @return the result of the addition
     * @example 9999 + 9999 -> 199.98
     *
     */
    int additionWithDecimal(int a, int b);

    /*
     * @brief soustraction of two int with two decimals
     * @param a the first int
     * @param b the second int
     * @return the result of the soustraction
     * @example 9999 - 9999 -> 0
     *
     */
    int subtractionWithDecimal(int a, int b);

    /*
     * @brief multiplication of two int with two decimals
     * @param a the first int
     * @param b the second int
     * @return the result of the multiplication
     *
     */
    int multiplicationWithDecimal(int a, int b);

    /*
     * @brief division of two int with two decimals
     * @param a the first int
     * @param b the second int
     * @return the result of the division
     *
     */
    int divisionWithDecimal(int a, int b);

} // namespace Maths
