## Maths

The Maths namespace provides a collection of utility functions for precise mathematical operations involving decimal numbers. The namespace includes functions for converting between float and integer representations, performing arithmetic operations with preserved decimal precision, and modifying decimal integers using normal integers and floats.

You can use it including the following header file:

```cpp
#include "Maths.hpp"
```

### Constants

```cpp
static constexpr int Maths::DECIMALS_TO_CONSERVE = 2;
```

This constant specifies the number of decimal places to preserve during calculations, ensuring precision in decimal arithmetic operations.

For example, if `DECIMALS_TO_CONSERVE` is 2, we keep 2 decimal precision points. This means that (float)99.99 is converted to (int)9999, and (float) 0.09 is converted to (int)9.

### Functions

- Calculates and returns a multiplier based on the `DECIMALS_TO_CONSERVE` constant.
```cpp
static constexpr int Maths::getMultiplier()
// For example, `DECIMALS_TO_CONSERVE = 2` results in `100`
```

- Converts a floating-point number to an integer while conserving the specified number of decimals.
```cpp
int Maths::floatToIntConservingDecimals(const float normalFloat)
// For example, `99.99` becomes `9999` for `DECIMALS_TO_CONSERVE = 2`
```

- Converts an integer to a floating-point number with the specified number of decimals.
```cpp
float Maths::intToFloatConservingDecimals(const int decimalInt)
// For example, `9999` becomes `99.99` for `DECIMALS_TO_CONSERVE = 2`
```

- Removes decimals from an integer, producing a truncated integer result.
```cpp
int Maths::removeIntDecimals(const int decimalInt)
// For example, `9999` becomes `99` for `DECIMALS_TO_CONSERVE = 2`
```

- Adds the specified number of decimals to an integer.
```cpp
int Maths::addIntDecimals(const int normalInt)
// For instance, `99` becomes `9900` for `DECIMALS_TO_CONSERVE = 2`
```

- Performs addition on two integers with preserved decimals.
```cpp
int Maths::additionWithTwoIntDecimals(const int decimalInt, const int otherDecimalInt)
// For example, `9999 + 9999` results in `199.98` for `DECIMALS_TO_CONSERVE = 2`
```

- Performs subtraction on two integers with preserved decimals.
```cpp
int Maths::subtractionWithTwoIntDecimals(const int minuend, const int subtrahend)
// For instance, `9999 - 9999` results in `0` for `DECIMALS_TO_CONSERVE = 2`
```

- Performs multiplication on two integers with preserved decimals.
```cpp
int Maths::multiplicationWithTwoIntDecimals(const int decimalInt, const int otherDecimalInt)
```

- Performs division on two integers with preserved decimals.
```cpp
int Maths::divisionWithTwoIntDecimals(const int dividend, const int divisor)
```

- Adds a normal integer to a decimal integer, modifying the decimal integer in place.
```cpp
void Maths::addNormalIntToDecimalInt(int &decimalInt, const int normalInt)
// For example, `500 + 5` results in `550` for `DECIMALS_TO_CONSERVE = 2`
```

- Subtracts a normal integer from a decimal integer, modifying the decimal integer in place.
```cpp
void Maths::subNormalIntToDecimalInt(int &decimalInt, const int normalInt)
// For instance, `550 - 5` results in `500` for `DECIMALS_TO_CONSERVE = 2`
```

- Adds a floating-point number to a decimal integer, modifying the decimal integer in place.
```cpp
void Maths::addFloatToDecimalInt(int &decimalInt, const float normalFloat)
// For example, `500 + 5.5` results in `555` for `DECIMALS_TO_CONSERVE = 2`
```
