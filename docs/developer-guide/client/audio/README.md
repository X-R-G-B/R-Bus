# Using Audio in our game client

In your C++ client application, you can manage audio using the Raylib library. This guide explains how to work with sounds and music using our Raylib wrapper classes.

## Sound Class

### Constructors

```cpp
Raylib::Sound(const std::string& fileName, float volume = 0.5f);
```
Create a sound object from the specified audio file.

### Methods

```cpp
void unload();
void play() const;
void stop() const;
void pause() const;
void resume() const;
bool isPlaying() const;
void setVolume(float volume) const;
void setPitch(float pitch) const;
void setPan(float pan) const;
bool NeedToPlay() const;
void setNeedToPlay(bool needToPlay);
std::string getPath() const;
```

### Example usage

```cpp
Raylib::Sound mySound("path/to/soundfile.wav");
mySound.play();
mySound.setVolume(0.8f);
```

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

## Audio Device Management

### Functions

```cpp
void initAudioDevice();
void closeAudioDevice();
bool isAudioDeviceReady();
void setMasterVolume(float volume);
```

## Example usage

You need to initialize the audio device before using any audio functions. You can do this by calling the initAudioDevice() function. You can then use the other functions to manage the audio device. If you want to player music, every frame you need to call the UpdateMusicStream() function.

```cpp
int main() {
    Raylib::Music music("path/to/musicfile.ogg");

    initAudioDevice();
    // game loop
    while (!WindowShouldClose()) {
        // stuff
        //for every music
        UpdateMusicStream(music);
    }
    closeAudioDevice();
}
```

## Audio File Formats

Raylib supports the following audio file formats:

- WAV
- OGG
- MP3
- XM
- QOA
- MOD
- FLAC

## Volume, pitch, and pan

- The pitch base level is 1.0f (normal pitch).
- The volume base level is 1.0f (maximum volume).
- The pan base level is 0.5f (center).
