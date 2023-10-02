## Sprite Class

### Constructors

```cpp
Raylib::Sprite(const std::string &fileName, float width, float height);
Raylib::Sprite(Image image, float width, float height);
```

Create a sprite object from the specified file and set its width and height or create a sprite from an image object with the given width and height. The width and height are in percentage of the screen.

### Methods

```cpp
unsigned int Raylib::Sprite::getId() const;
float Raylib::Sprite::getWidth() const;
float Raylib::Sprite::getHeight() const;
int Raylib::Sprite::getTextureWidth() const;
int Raylib::Sprite::getTextureHeight() const;
int Raylib::Sprite::getMipmaps() const;
int Raylib::Sprite::getFormat() const;
void Raylib::Sprite::unloadSprite();
```

### Example usage

```cpp
Raylib::Sprite mySprite("path/to/spritefile.png", 100.0f, 150.0f);
float spriteWidth = mySprite.getWidth();
std::cout << "Sprite width: " << spriteWidth << std::endl;
mySprite.unloadSprite();
```
