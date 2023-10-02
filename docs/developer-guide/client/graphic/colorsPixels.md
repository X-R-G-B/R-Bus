## Colors/pixels related functions

- `Raylib::Color Raylib::fade(Raylib::Color color, float alpha);`
  Fade a color by a specified alpha value.

- `int Raylib::colorToInt(Raylib::Color color);`
  Convert a Color to a 32-bit integer.

- `Vector4 Raylib::colorNormalize(Raylib::Color color);`
  Normalize a Color struct to a Vector4.

- `Raylib::Color Raylib::colorFromNormalized(Raylib::Vector4 normalized);`
  Create a Color from a normalized Vector4.

- `Raylib::Color Raylib::getColor(unsigned int hexValue);`
  Create a Color from a hex value.
