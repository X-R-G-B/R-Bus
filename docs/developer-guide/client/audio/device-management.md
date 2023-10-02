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
