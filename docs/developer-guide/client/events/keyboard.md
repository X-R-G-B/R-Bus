## Input-related functions: keyboard

- `bool Raylib::isKeyPressed(KeyboardKey key);`
  Check if a key has been pressed once.

- `bool Raylib::isKeyDown(KeyboardKey key);`
  Check if a key is being pressed.

- `bool Raylib::isKeyReleased(KeyboardKey key);`
  Check if a key has been released once.

- `bool Raylib::isKeyUp(KeyboardKey key);`
  Check if a key is NOT being pressed.

- `void Raylib::setExitKey(KeyboardKey key);`
  Set a key to exit the application.

- `int Raylib::getKeyPressed();`
  Get the key pressed (keycode).

- `int Raylib::getCharPressed();`
  Get the last character pressed (unicode).
