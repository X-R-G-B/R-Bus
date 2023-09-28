/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Audio
*/

#include "Audio.hpp"

namespace Raylib {

    // Audio device management functions

    void initAudioDevice()
    {
        InitAudioDevice();
    }

    void closeAudioDevice()
    {
        CloseAudioDevice();
    }

    bool isAudioDeviceReady()
    {
        return IsAudioDeviceReady();
    }

    void setMasterVolume(float volume)
    {
        SetMasterVolume(volume);
    }

    // Sounds
    Sound::Sound(const std::string& fileName, float volume)
        : sound(LoadSound(fileName.c_str())),
          _path(fileName)
    {
        SetSoundVolume(sound, volume);
    }

    void Sound::unload()
    {
        UnloadSound(sound);
    }

    void Sound::play() const
    {
        PlaySound(sound);
    }

    void Sound::stop() const
    {
        StopSound(sound);
    }

    void Sound::pause() const
    {
        PauseSound(sound);
    }

    void Sound::resume() const
    {
        ResumeSound(sound);
    }

    bool Sound::isPlaying() const
    {
        return IsSoundPlaying(sound);
    }

    void Sound::setVolume(float volume) const
    {
        SetSoundVolume(sound, volume);
    }

    void Sound::setPitch(float pitch) const
    {
        SetSoundPitch(sound, pitch);
    }

    void Sound::setPan(float pan) const
    {
        SetSoundPitch(sound, pan);
    }

    bool Sound::NeedToPlay() const
    {
        return _needToPlay;
    }

    void Sound::setNeedToPlay(bool needToPlay)
    {
        _needToPlay = needToPlay;
    }

    std::string Sound::getPath() const
    {
        return _path;
    }

    // Music

    Music::Music(const std::string& fileName, float volume)
        : music(LoadMusicStream(fileName.c_str())),
          _path(fileName)
    {
        SetMusicVolume(music, volume);
    }

    void Music::unload()
    {
        UnloadMusicStream(music);
    }

    bool Music::isReady() const
    {
        return IsMusicStreamPlaying(music);
    }

    void Music::play() const
    {
        PlayMusicStream(music);
    }

    bool Music::isPlaying() const
    {
        return IsMusicStreamPlaying(music);
    }

    void Music::update() const
    {
        UpdateMusicStream(music);
    }

    void Music::stop() const
    {
        StopMusicStream(music);
    }

    void Music::pause() const
    {
        PauseMusicStream(music);
    }

    void Music::resume() const
    {
        ResumeMusicStream(music);
    }

    void Music::setVolume(float volume) const
    {
        SetMusicVolume(music, volume);
    }

    void Music::setPitch(float pitch) const
    {
        SetMusicPitch(music, pitch);
    }

    void Music::setPan(float pan) const
    {
        SetMusicPitch(music, pan);
    }

    float Music::getTimeLength() const
    {
        return GetMusicTimeLength(music);
    }

    float Music::getTimePlayed() const
    {
        return GetMusicTimePlayed(music);
    }

    bool Music::NeedToPlay() const
    {
        return _needToPlay;
    }

    void Music::setNeedToPlay(bool needToPlay)
    {
        _needToPlay = needToPlay;
    }

    std::string Music::getPath() const
    {
        return _path;
    }
} // namespace Raylib
