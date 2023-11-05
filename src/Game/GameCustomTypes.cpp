/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Custom Types
*/

#include "GameCustomTypes.hpp"
#ifdef CLIENT
    #include "B-luga-graphics/Raylib/Raylib.hpp"
#endif

unsigned int Types::Enemy::_enemyNb = 0;
std::mutex Types::Enemy::_mutex;
unsigned int Types::Missiles::_missileNb = 0;
std::mutex Types::Missiles::_mutex;

namespace Types {

    void Types::WaveInfos::prepareNextWave()
    {
        setFirstEnemyCreated(false);
        _remainingEnemies.clear();
        setWaitingForNextWave(true);
    }

    void Types::WaveInfos::setWaitingForNextWave(bool value)
    {
#ifdef CLIENT
        Registry &registry            = Registry::getInstance();
        const std::string textKeyword = "WaveText";
        std::vector<std::size_t> ids  = registry.getEntitiesByComponents({typeid(Raylib::TextShared)});
        Registry::components<Raylib::TextShared> arrCol = registry.getComponents<Raylib::TextShared>();
        static constexpr float fontSize                 = 4.0F;

        if (value == false) {
            for (auto &id : ids) {
                if (arrCol[id]->getKeyword() == textKeyword) {
                    registry.removeEntity(id);
                    break;
                }
            }
        } else {
            unsigned int previousWave = getWaveId();
            const std::string text    = "Wave " + std::to_string(previousWave) + " survived";

            for (auto &id : ids) {
                if (arrCol[id]->getKeyword() == textKeyword) {
                    arrCol[id]->setCurrentText(text);
                    break;
                }
            }
            Registry::getInstance().addEntity();
            Raylib::TextShared endWaveText = Raylib::Text::fromText(
                text,
                {20, 10},
                fontSize,
                Raylib::Color(Raylib::ColorDef::White),
                textKeyword);
            Registry::getInstance().getComponents<Raylib::TextShared>().insertBack(endWaveText);
        }
#endif
        _waitingForNextWave = value;
    }

} // namespace Types
