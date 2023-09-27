#pragma once

#include <string>
#include "raylib.h"

namespace Raylib {
    // Audio device management functions
    void initAudioDevice();
    void closeAudioDevice();
    bool isAudioDeviceReady();
    void setMasterVolume(float volume);

    // Sounds
    class Sound {
        public:
            Sound(const std::string& fileName);
            ~Sound();

            void play() const;
            void stop() const;
            void pause() const;
            void resume() const;
            bool isPlaying() const;
            void setVolume(float volume) const;
            void setPitch(float pitch) const;
            void setPan(float pan) const;

        private:
            ::Sound sound;
    };

    class Music {
        public:
            Music(const std::string& fileName);
            ~Music();
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

        private:
            ::Music music;
    };
} // namespace Raylib
