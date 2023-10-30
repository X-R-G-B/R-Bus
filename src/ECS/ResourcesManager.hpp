#pragma once

#include <string>
#include "Logger.hpp"

namespace ECS {
    class ResourcesManager {
        public:
            /**
             * @brief The type of the file
             */
            enum class FileType {
                ASSETS,
                BINARY,
            };

            /**
             * @brief Get the resource path
             * @param path_const The path to convert
             * @return The converted path
             * @note You need to have 'assets' in your path at the beginning
             */
            static std::string convertPath(const std::string &path_const);

            /**
             * @brief Convert a path to the correct path
             * @param path_const The path to convert
             * @param type The type of the file
             * @return The converted path
             */
            static std::string convertPath(const std::string &path_const, FileType type);

            /**
             * @brief Init the resource manager
             * @param execPath The path of the executable
             */
            static void init(std::string execPath);

        private:
            // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
            static std::string _resourcePath;
            // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
    };
} // namespace ECS
