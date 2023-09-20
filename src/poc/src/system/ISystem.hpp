/*
** ISystem.hpp for R-Bus in /home/kitetsu/Epitech/R-Bus/src/system
**
** Made by brice
** Login   <brice.desousa@epitech.eu>
**
** Started on  Tue Sep 19 5:13:48 PM 2023 brice
** Last update Thu Sep 20 9:54:30 AM 2023 brice
*/

#pragma once

namespace System {
    class ISystem {
        public:
            virtual ~ISystem() = default;

            virtual void run() = 0;

        protected:
        private:
            virtual void clientRun() = 0;
    };
}
