/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** unloadRaylib
*/

#include "Raylib.hpp"
#include "Registry.hpp"

static void unloadSounds(std::size_t id)
{
    Registry::components<Raylib::Sound> arrSound = Registry::getInstance().getComponents<Raylib::Sound>();

    if (arrSound.exist(id)) {
        arrSound[id].unload();
    }
}

static void unloadMusic(std::size_t id)
{
    Registry::components<Raylib::Music> arrMusic = Registry::getInstance().getComponents<Raylib::Music>();

    if (arrMusic.exist(id)) {
        arrMusic[id].unload();
    }
}

static void unloadSprite(std::size_t id)
{
    Registry::components<Raylib::Sprite> arrSprite =
        Registry::getInstance().getComponents<Raylib::Sprite>();

    if (arrSprite.exist(id)) {
        arrSprite[id].unloadSprite();
    }
}

static void unloadImage(std::size_t id)
{
    Registry::components<Raylib::RayImage> arrImage = Registry::getInstance().getComponents<Raylib::RayImage>();

    if (arrImage.exist(id)) {
        arrImage[id].unloadImage();
    }
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static,-warnings-as-errors)
class UnloadRaylib {
    public:
        static void unloadRaylibComponents(std::size_t id)
        {
            unloadSounds(id);
            unloadMusic(id);
            unloadSprite(id);
            unloadImage(id);
        }
};
