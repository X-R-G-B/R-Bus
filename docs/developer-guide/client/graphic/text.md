## Text Class

### Constructors

```cpp
Raylib::Text(
    std::string text,
    Raylib::Vector2 position = {0, 0},
    float fontSize = 5.0f,
    Raylib::Color color = Raylib::BLACK);
```

Create a text object with the given text, position, font size, and color.

### Methods

```cpp
void Raylib::Textdraw();
void Raylib::TextdrawEx(float spacing);
void Raylib::TextdrawPro(Raylib::Vector2 origin, float rotation, float spacing);

float Raylib::Text::x() const;
float Raylib::Text::y() const;
float Raylib::Text::getFontSize() const;
void Raylib::Text::setFontSize(float fontSize);
Raylib::Vector2 Raylib::Text::getPosition() const;
void Raylib::Text::setPixelPosition(Raylib::Vector2 position);
void Raylib::Text::setCurrentFontSize(float fontSize);
std::string &Text::getText();
void Text::setText(const std::string &text)
```

getPosition is in percentage of the screen size. For each draw, we recommand to compute and set the position in pixel with setPixelPosition to have a responsive text.

### Example usage

```cpp
Raylib::Text myText("Hello, World!", {100, 200}, 20.0f, Raylib::BLUE);
myText.setPixelPosition({300, 400});
myText.draw();
```
