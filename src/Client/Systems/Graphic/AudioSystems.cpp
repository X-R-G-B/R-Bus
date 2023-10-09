/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** AudioSystems implementation
*/

#include "AudioSystems.hpp"
#include "CustomTypes.hpp"
#include "Raylib.hpp"
#include "SystemManagersDirector.hpp"

namespace Systems {
    void GraphicSystems::soundEffectPlayer(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry::components<Raylib::Sound> arrSoundEffect =
            Registry::getInstance().getComponents<Raylib::Sound>();

        for (auto &soundEffect : arrSoundEffect) {
            if (soundEffect.NeedToPlay()) {
                soundEffect.play();
                soundEffect.setNeedToPlay(false);
            }
        }
    }

    void GraphicSystems::musicPlayer(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry::components<Raylib::Music> arrMusics =
            Registry::getInstance().getComponents<Raylib::Music>();

        for (auto &music : arrMusics) {
            if (music.NeedToPlay()) {
                music.play();
                music.setNeedToPlay(false);
            }
            if (music.isPlaying()) {
                music.update();
            }
        }
    }

    const std::string musicPath = "assets/Audio/Musics/Title.mp3";
    const std::string soundPath = "assets/Audio/Sounds/fire.ogg";

    void GraphicSystems::playSoundWithKey(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry &registry                            = Registry::getInstance();
        Registry::components<Raylib::Music> arrMusics = registry.getComponents<Raylib::Music>();
        Registry::components<Raylib::Sound> arrSounds = registry.getComponents<Raylib::Sound>();

        for (auto &music : arrMusics) {
            if (music.getPath() == musicPath && Raylib::isKeyPressed(Raylib::KeyboardKey::KB_SPACE)) {
                music.setNeedToPlay(true);
            }
        }
        for (auto &sound : arrSounds) {
            if (sound.getPath() == soundPath && Raylib::isKeyPressed(Raylib::KeyboardKey::KB_ENTER)) {
                sound.setNeedToPlay(true);
            }
        }
    }

    void GraphicSystems::initAudio(std::size_t managerId, std::size_t systemId)
    {
        constexpr float musicVolume = 0.60F;
        constexpr float soundVolume = 0.63F;

        Raylib::Music music(musicPath, musicVolume);
        Raylib::Sound sound(soundPath, soundVolume);

        Registry::getInstance().addEntity();
        Registry::getInstance().getComponents<Raylib::Music>().insertBack(music);
        Registry::getInstance().getComponents<Raylib::Sound>().insertBack(sound);
        SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> GraphicSystems::getAudioSystems()
    {
        return {soundEffectPlayer, musicPlayer, playSoundWithKey, initAudio};
    }
} // namespace Systems
