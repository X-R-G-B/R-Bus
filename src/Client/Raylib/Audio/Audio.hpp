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

        private:
            ::Sound sound;
            bool _needToPlay {false};
            std::string _path;
    };

    class Music {
        public:
            Music(const std::string& fileName);
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

        private:
            ::Music music;
            bool _needToPlay {false};
            std::string _path;
    };
} // namespace Raylib
