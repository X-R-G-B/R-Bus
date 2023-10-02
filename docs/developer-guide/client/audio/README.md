# Using Audio in our game client

In your C++ client application, you can manage audio using the Raylib library. This guide explains how to work with sounds and music using our Raylib wrapper classes. Before looking at the other pages, we advise you to look at the rest of this page. Initialize the audio correctly is essential to use it in our client.

## Audio Device Management

### Functions

```cpp
void Raylib::initAudioDevice();
void Raylib::closeAudioDevice();
bool Raylib::isAudioDeviceReady();
void Raylib::setMasterVolume(float volume);
```

## Example usage

You need to initialize the audio device before using any audio functions. You can do this by calling the initAudioDevice() function. You can then use the other functions to manage the audio device. If you want to player music, every frame you need to call the UpdateMusicStream() function.

```cpp
int main() {
    Raylib::Music music("path/to/musicfile.ogg");

    Raylib::initAudioDevice();
    // game loop
    while (!Raylib::WindowShouldClose()) {
        // stuff
        //for every music
        Raylib::UpdateMusicStream(music);
    }
    Raylib::closeAudioDevice();
}
```
