## Input-related functions: keyboard

- `bool Raylib::isKeyPressed(Raylib::KeyboardKey key);`
  Check if a key has been pressed once.

- `bool Raylib::isKeyDown(Raylib::KeyboardKey key);`
  Check if a key is being pressed.

- `bool Raylib::isKeyReleased(Raylib::KeyboardKey key);`
  Check if a key has been released once.

- `bool Raylib::isKeyUp(Raylib::KeyboardKey key);`
  Check if a key is NOT being pressed.

- `void Raylib::setExitKey(Raylib::KeyboardKey key);`
  Set a key to exit the application.

- `int Raylib::getKeyPressed();`
  Get the key pressed (keycode).

- `int Raylib::getCharPressed();`
  Get the last character pressed (unicode).
