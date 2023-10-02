## Music Class

### Constructors

```cpp
Raylib::Music(const std::string& fileName, float volume = 0.5f);
```

Create a music object from the specified audio file.

### Methods

```cpp
void Raylib::Music::unload();
bool Raylib::Music::isReady() const;
void Raylib::Music::play() const;
bool Raylib::Music::isPlaying() const;
void Raylib::Music::update() const;
void Raylib::Music::stop() const;
void Raylib::Music::pause() const;
void Raylib::Music::resume() const;
void Raylib::Music::setVolume(float volume) const;
void Raylib::Music::setPitch(float pitch) const;
void Raylib::Music::setPan(float pan) const;
float Raylib::Music::getTimeLength() const;
float Raylib::Music::getTimePlayed() const;
bool Raylib::Music::NeedToPlay() const;
void Raylib::Music::setNeedToPlay(bool needToPlay);
std::string Raylib::Music::getPath() const;
```

### Example usage

```cpp
Raylib::Music myMusic("path/to/musicfile.ogg");
myMusic.play();
myMusic.setVolume(0.6f);
```
