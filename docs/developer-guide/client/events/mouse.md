## Input-related functions: mouse

- `bool Raylib::isMouseButtonPressed(MouseButton button);`
  Check if a mouse button has been pressed once.

- `bool Raylib::isMouseButtonDown(MouseButton button);`
  Check if a mouse button is being pressed.

- `bool Raylib::isMouseButtonReleased(MouseButton button);`
  Check if a mouse button has been released once.

- `bool Raylib::isMouseButtonUp(MouseButton button);`
  Check if a mouse button is NOT being pressed.

- `int Raylib::getMouseX();`
  Get the X position of the mouse cursor.

- `int Raylib::getMouseY();`
  Get the Y position of the mouse cursor.

- `Vector2 Raylib::getMousePosition();`
  Get the current position of the mouse cursor.

- `Vector2 Raylib::getMouseDelta();`
  Get the mouse delta movement.

- `void Raylib::setMousePosition(int x, int y);`
  Set the position of the mouse cursor.

- `void Raylib::setMouseOffset(int offsetX, int offsetY);`
  Set an offset for the mouse position.

- `void Raylib::setMouseScale(float scaleX, float scaleY);`
  Set the scaling factor for the mouse position.

- `float Raylib::getMouseWheelMove();`
  Get the mouse wheel movement.

- `Vector2 Raylib::getMouseWheelMoveV();`
  Get the mouse wheel movement as a vector.

- `void Raylib::setMouseCursor(int cursor);`
  Set the mouse cursor style.

## Example

```cpp
if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_RIGHT))
{
    // Move right
}
else if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_LEFT))
{
    // Move left
}
```
