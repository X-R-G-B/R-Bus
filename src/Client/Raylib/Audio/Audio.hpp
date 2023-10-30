/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Audio
*/

#pragma once

#include <string>
extern "C"
{
#include "raylib.h"
}

namespace Raylib {
    // Audio device management functions
    void initAudioDevice();
    void closeAudioDevice();
    bool isAudioDeviceReady();
    void setMasterVolume(float volume);

    // Sounds
    class Sound {
        public:
            Sound(const std::string& fileName, float volume = 0.5f);
            [[nodiscard]] bool isReady() const;
            void unload();
            void play() const;
            void stop() const;
            void pause() const;
            void resume() const;
            [[nodiscard]] bool isPlaying() const;
            void setVolume(float volume) const;
            void setPitch(float pitch) const;
            void setPan(float pan) const;
            [[nodiscard]] bool NeedToPlay() const;
            void setNeedToPlay(bool needToPlay);
            [[nodiscard]] const std::string &getPath() const;
            [[nodiscard]] const std::string &getRealPath() const;

        private:
            std::string _path;
            std::string _realPath;
            ::Sound _sound;
            bool _needToPlay {false};
    };

    class Music {
        public:
            Music(const std::string& fileName, float volume = 0.5f);
            void unload();
            [[nodiscard]] bool isReady() const;
            void play() const;
            [[nodiscard]] bool isPlaying() const;
            void update() const;
            void stop() const;
            void pause() const;
            void resume() const;
            void setVolume(float volume) const;
            void setPitch(float pitch) const;
            void setPan(float pan) const;
            [[nodiscard]] float getTimeLength() const;
            [[nodiscard]] float getTimePlayed() const;
            [[nodiscard]] bool NeedToPlay() const;
            void setNeedToPlay(bool needToPlay);
            [[nodiscard]] const std::string &getPath() const;
            [[nodiscard]] const std::string &getRealPath() const;

        private:
            std::string _path;
            std::string _realPath;
            ::Music _music;
            bool _needToPlay {false};
    };
} // namespace Raylib
