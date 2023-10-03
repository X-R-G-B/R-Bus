## Image Class

### Constructors

```cpp
Raylib::Image(const std::string& fileName);
Raylib::Image(int width, int height, Raylib::Color color);
```
Create an image object from the specified file or create a blank image with the given width, height, and color.

### Methods

```cpp
bool Raylib::Image::isImageReady() const;
void Raylib::Image::unloadImage();
int Raylib::Image::getWidth() const;
int Raylib::Image::getHeight() const;
int Raylib::Image::getMipmaps() const;
int Raylib::Image::getFormat() const;
void* Raylib::Image::getData() const;
```

### Example usage

```cpp
Raylib::Image myImage("path/to/imagefile.png");
int width = myImage.getWidth();
std::cout << "Image width: " << width << std::endl;
myImage.unloadImage();
```
