/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Audio
*/

#pragma once

#include <string>
#include "ResourcesManager.hpp"
extern "C"
{
#include "raylib.h"
}

namespace Raylib {
    // Audio device management functions
    class Audio {
        public:
            static void initAudioDevice()
            {
                InitAudioDevice();
            }
            static void closeAudioDevice()
            {
                CloseAudioDevice();
            }
            static bool isAudioDeviceReady()
            {
                return IsAudioDeviceReady();
            }
            static void setMasterVolume(float volume)
            {
                SetMasterVolume(volume);
            }
    };

    // Sounds
    class Sound {
        public:
            Sound(const std::string& fileName, float volume)
                : _path(fileName),
                  _realPath(ECS::ResourcesManager::convertPath(fileName)),
                  _sound(LoadSound(_realPath.c_str()))
            {
                SetSoundVolume(_sound, volume);
            }

            bool isReady() const
            {
                return IsSoundReady(_sound);
            }

            void unload()
            {
                UnloadSound(_sound);
            }

            void play() const
            {
                PlaySound(_sound);
            }

            void stop() const
            {
                StopSound(_sound);
            }

            void pause() const
            {
                PauseSound(_sound);
            }

            void resume() const
            {
                ResumeSound(_sound);
            }

            bool isPlaying() const
            {
                return IsSoundPlaying(_sound);
            }

            void setVolume(float volume) const
            {
                SetSoundVolume(_sound, volume);
            }

            void setPitch(float pitch) const
            {
                SetSoundPitch(_sound, pitch);
            }

            void setPan(float pan) const
            {
                SetSoundPitch(_sound, pan);
            }

            bool NeedToPlay() const
            {
                return _needToPlay;
            }

            void setNeedToPlay(bool needToPlay)
            {
                _needToPlay = needToPlay;
            }

            std::string getPath() const
            {
                return _path;
            }

            std::string getRealPath() const
            {
                return _realPath;
            }

        private:
            std::string _path;
            std::string _realPath;
            ::Sound _sound;
            bool _needToPlay {false};
    };

    class Music {
        public:
            Music(const std::string& fileName, float volume)
                : _path(fileName),
                  _realPath(ECS::ResourcesManager::convertPath(fileName)),
                  _music(LoadMusicStream(_realPath.c_str()))
            {
                SetMusicVolume(_music, volume);
            }

            void unload()
            {
                UnloadMusicStream(_music);
            }

            bool isReady() const
            {
                return IsMusicReady(_music);
            }

            void play() const
            {
                PlayMusicStream(_music);
            }

            bool isPlaying() const
            {
                return IsMusicStreamPlaying(_music);
            }

            void update() const
            {
                UpdateMusicStream(_music);
            }

            void stop() const
            {
                StopMusicStream(_music);
            }

            void pause() const
            {
                PauseMusicStream(_music);
            }

            void resume() const
            {
                ResumeMusicStream(_music);
            }

            void setVolume(float volume) const
            {
                SetMusicVolume(_music, volume);
            }

            void setPitch(float pitch) const
            {
                SetMusicPitch(_music, pitch);
            }

            void setPan(float pan) const
            {
                SetMusicPitch(_music, pan);
            }

            float getTimeLength() const
            {
                return GetMusicTimeLength(_music);
            }

            float getTimePlayed() const
            {
                return GetMusicTimePlayed(_music);
            }

            bool NeedToPlay() const
            {
                return _needToPlay;
            }

            void setNeedToPlay(bool needToPlay)
            {
                _needToPlay = needToPlay;
            }

            std::string getPath() const
            {
                return _path;
            }

            std::string getRealPath() const
            {
                return _realPath;
            }

        private:
            std::string _path;
            std::string _realPath;
            ::Music _music;
            bool _needToPlay {false};
    };
} // namespace Raylib
