/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** AudioSystems implementation
*/

#include "AudioSystems.hpp"
#include "GraphicsCustomTypes.hpp"
#include "Registry.hpp"
#include "SystemManagersDirector.hpp"

namespace Systems {
    void GraphicsSystems::soundEffectPlayer(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        Registry::components<Raylib::Sound> arrSoundEffect =
            Registry::getInstance().getComponents<Raylib::Sound>();

        for (auto &soundEffect : arrSoundEffect) {
            if (soundEffect.NeedToPlay()) {
                soundEffect.play();
                soundEffect.setNeedToPlay(false);
            }
        }
    }

    void GraphicsSystems::musicPlayer(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
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

    const std::string musicPath      = "assets/Audio/Musics/Title.mp3";
    const std::string soundPathShoot = "assets/Audio/Sounds/laser.ogg";

    void GraphicsSystems::playSoundWithKey(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        Registry &registry                            = Registry::getInstance();
        Registry::components<Raylib::Music> arrMusics = registry.getComponents<Raylib::Music>();

        for (auto &music : arrMusics) {
            if (music.getPath() == musicPath && Raylib::isKeyPressed(Raylib::KeyboardKey::KB_M)) {
                music.setNeedToPlay(true);
            }
        }
    }

    void GraphicsSystems::initAudio(std::size_t managerId, std::size_t systemId)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        constexpr float musicVolume = 0.60F;
        constexpr float soundVolume = 0.63F;

        Raylib::Music music(musicPath, musicVolume);
        Raylib::Sound soundEffectShoot(soundPathShoot, soundVolume);

        Registry::getInstance().addEntity();
        Registry::getInstance().getComponents<Raylib::Music>().insertBack(music);
        Registry::getInstance().getComponents<Raylib::Sound>().insertBack(soundEffectShoot);
        SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> GraphicsSystems::getAudioSystems()
    {
        return {soundEffectPlayer, musicPlayer, playSoundWithKey, initAudio};
    }
} // namespace Systems
