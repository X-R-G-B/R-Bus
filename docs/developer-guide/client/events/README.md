# Input Handling

In your game client, we use and enum to represent keyboard and mouse keys. The enum is defined in `inputs.hpp` and is called `KeyboardKey` and `MouseButton`. The enum values are the same as the ones defined in `raylib.h`.

## Input-related functions: keyboard

- `bool isKeyPressed(KeyboardKey key);`
  Check if a key has been pressed once.

- `bool isKeyDown(KeyboardKey key);`
  Check if a key is being pressed.

- `bool isKeyReleased(KeyboardKey key);`
  Check if a key has been released once.

- `bool isKeyUp(KeyboardKey key);`
  Check if a key is NOT being pressed.

- `void setExitKey(KeyboardKey key);`
  Set a key to exit the application.

- `int getKeyPressed();`
  Get the key pressed (keycode).

- `int getCharPressed();`
  Get the last character pressed (unicode).

## Input-related functions: mouse

- `bool isMouseButtonPressed(MouseButton button);`
  Check if a mouse button has been pressed once.

- `bool isMouseButtonDown(MouseButton button);`
  Check if a mouse button is being pressed.

- `bool isMouseButtonReleased(MouseButton button);`
  Check if a mouse button has been released once.

- `bool isMouseButtonUp(MouseButton button);`
  Check if a mouse button is NOT being pressed.

- `int getMouseX();`
  Get the X position of the mouse cursor.

- `int getMouseY();`
  Get the Y position of the mouse cursor.

- `Vector2 getMousePosition();`
  Get the current position of the mouse cursor.

- `Vector2 getMouseDelta();`
  Get the mouse delta movement.

- `void setMousePosition(int x, int y);`
  Set the position of the mouse cursor.

- `void setMouseOffset(int offsetX, int offsetY);`
  Set an offset for the mouse position.

- `void setMouseScale(float scaleX, float scaleY);`
  Set the scaling factor for the mouse position.

- `float getMouseWheelMove();`
  Get the mouse wheel movement.

- `Vector2 getMouseWheelMoveV();`
  Get the mouse wheel movement as a vector.

- `void setMouseCursor(int cursor);`
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
