## Shapes-related functions

- `void Raylib::drawPixel(int posX, int posY, Raylib::Color color);`
  Draw a pixel at the specified position with the given color.

- `void Raylib::drawCircle(int centerX, int centerY, float radius, Raylib::Color color);`
  Draw a circle with the specified center, radius, and color.

- `void Raylib::drawRectangle(int posX, int posY, int width, int height, Raylib::Color color);`
  Draw a rectangle at the specified position with the given width, height, and color.

### Example usage

```cpp
Raylib::drawPixel(100, 100, Raylib::RED);
Raylib::drawCircle(200, 200, 50, Raylib::GREEN);
Raylib::drawRectangle(300, 300, 80, 120, Raylib::BLUE);
```
