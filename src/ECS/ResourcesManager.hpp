#pragma once

#include <string>
#include "Logger.hpp"
#include "ResourcesManager.hpp"
#include "boost/filesystem.hpp"

namespace ECS {
    class ResourcesManager {
        public:
            static void init(std::string execPath)
            {
                if (!boost::filesystem::exists(execPath)) {
                    Logger::fatal("RESOURCE_MANAGER: Invalid execPath: " + execPath);
                    return;
                }
                boost::filesystem::path path_tmp = execPath;
                path_tmp                         = path_tmp.remove_filename();
                path_tmp = boost::filesystem::absolute(path_tmp, boost::filesystem::initial_path());
#ifndef PACKAGED
                path_tmp.append("assets");
                if (!boost::filesystem::exists(path_tmp)) {
                    Logger::fatal("RESOURCE_MANAGER: Path not found: " + path_tmp.string());
                }
                getRessourcePath() = path_tmp.make_preferred().string();
                Logger::info("RESOURCE_MANAGER: Path Assets: " + getRessourcePath());
                return;
#endif
                path_tmp = path_tmp.parent_path();
                path_tmp.append("share");
                if (!boost::filesystem::exists(path_tmp)) {
                    Logger::fatal("RESOURCE_MANAGER: Path not found: " + path_tmp.string());
                }
                path_tmp.append("r-type");
                if (!boost::filesystem::exists(path_tmp)) {
                    Logger::fatal("RESOURCE_MANAGER: Path not found: " + path_tmp.string());
                }
                getRessourcePath() = path_tmp.make_preferred().string();
                if (!boost::filesystem::exists(getRessourcePath())) {
                    Logger::fatal("RESOURCE_MANAGER: Path not found: " + getRessourcePath());
                    path_tmp.append("assets");
                    if (!boost::filesystem::exists(path_tmp)) {
                        Logger::fatal("RESOURCE_MANAGER: Path not found: " + path_tmp.string());
                    } else {
                        getRessourcePath() = path_tmp.make_preferred().string();
                    }
                }
                Logger::info("RESOURCE_MANAGER: Path Assets: " + getRessourcePath());
            }

            static std::string convertPath(const std::string &path_const)
            {
                if (getRessourcePath().empty()) {
                    Logger::fatal("RESOURCE_MANAGER: need to call init first (" + path_const + ")");
                    return "";
                }
                if (path_const.substr(0, std::string("assets").length()) != std::string("assets")) {
                    Logger::error(
                        "RESOURCE_MANAGER: Invalid path given (must start with 'assets': " + path_const + ")");
                    return "";
                }
                std::string path(path_const, std::string("assets").length(), std::string::npos);
                boost::filesystem::path path_tmp = getRessourcePath();
                path_tmp.append(path);
                path_tmp = path_tmp.make_preferred();
                if (!boost::filesystem::exists(path_tmp)) {
                    Logger::fatal("RESOURCE_MANAGER: Path not found: " + path_tmp.string());
                } else {
                    Logger::debug("RESOURCE_MANAGER: Path found: " + path_tmp.string());
                }
                return path_tmp.string();
            }

        private:
            static std::string &getRessourcePath()
            {
                static std::string resourcePath;
                return resourcePath;
            }

    };
} // namespace ECS
