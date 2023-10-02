## Music Class

### Constructors

```cpp
Raylib::Music(const std::string& fileName, float volume = 0.5f);
```

Create a music object from the specified audio file.

### Methods

```cpp
void unload();
bool isReady() const;
void play() const;
bool isPlaying() const;
void update() const;
void stop() const;
void pause() const;
void resume() const;
void setVolume(float volume) const;
void setPitch(float pitch) const;
void setPan(float pan) const;
float getTimeLength() const;
float getTimePlayed() const;
bool NeedToPlay() const;
void setNeedToPlay(bool needToPlay);
std::string getPath() const;
```

### Example usage

```cpp
Raylib::Music myMusic("path/to/musicfile.ogg");
myMusic.play();
myMusic.setVolume(0.6f);
```
