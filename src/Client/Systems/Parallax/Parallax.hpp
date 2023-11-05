#pragma once

#include "ResourcesManager.hpp"

namespace Systems::Parallax {
    class ActualParallax {
        public:
            static ActualParallax &getInstance()
            {
                static auto instance = ActualParallax();
                return instance;
            }

            JsonType getActualParallaxType()
            {
                return _actualParallax;
            }

            void setActualParralaxType(JsonType data)
            {
                _actualParallax = data;
            }

        private:
            ActualParallax()
            {
                _actualParallax = JsonType::DEFAULT_PARALLAX;
            }
            ~ActualParallax() = default;

            JsonType _actualParallax;
    };

    void initParalax();
} // namespace Systems::Parallax