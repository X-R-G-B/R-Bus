## Sound Class

### Constructors

```cpp
Raylib::Sound(const std::string& fileName, float volume = 0.5f);
```
Create a sound object from the specified audio file.

### Methods

```cpp
void Raylib::Sound::unload();
void Raylib::Sound::play() const;
void Raylib::Sound::stop() const;
void Raylib::Sound::pause() const;
void Raylib::Sound::resume() const;
bool Raylib::Sound::isPlaying() const;
void Raylib::Sound::setVolume(float volume) const;
void Raylib::Sound::setPitch(float pitch) const;
void Raylib::Sound::setPan(float pan) const;
bool Raylib::Sound::NeedToPlay() const;
void Raylib::Sound::setNeedToPlay(bool needToPlay);
std::string Raylib::Sound::getPath() const;
```

### Example usage

```cpp
Raylib::Sound mySound("path/to/soundfile.wav");
mySound.play();
mySound.setVolume(0.8f);
```
