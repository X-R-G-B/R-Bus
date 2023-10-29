#pragma once

#include <string>
#include "Logger.hpp"

namespace ECS {
    class ResourcesManager {
        public:
            enum class FileType {
                ASSETS,
                BINARY,
            };
            static std::string convertPath(const std::string &path_const);
            static std::string convertPath(const std::string &path_const, FileType type);
            static void init(std::string execPath);

        private:
            // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
            static std::string _resourcePath;
            // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
    };
} // namespace ECS
