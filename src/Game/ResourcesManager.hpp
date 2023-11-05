#pragma once

#include <boost/asio.hpp>
#include <string>
#include "B-luga/Json.hpp"
#include "B-luga/Logger.hpp"
#include "B-luga/PathResolver.hpp"
#include "boost/filesystem.hpp"

enum class JsonType : std::size_t {
    DEFAULT_PLAYER = 0,
    DEFAULT_PARALLAX,
    WAVE,
    BULLETS,
    MENU,
    SELECT_LOBBY,
    CREATE_LOBBY,
    HUD,
    ENEMIES,
    MAXTYPE
};

namespace ResourcesManagerValues {
    const std::vector<std::string> paths = {
        "assets/Json/playerData.json",
        "assets/Json/parallaxData.json",
        "assets/Json/waves.json",
        "assets/Json/bullets.json",
        "assets/Json/menu.json",
        "assets/Json/selectLobby.json",
        "assets/Json/createLobby.json",
        "assets/Json/HUD.json",
        "assets/Json/enemies.json",
    };
}

class ResourcesManager {
    public:
        static std::string getPathByJsonType(JsonType type)
        {
            static bool init = false;

            ResourcesManager::getMutex().lock();
            if (!init) {
                ResourcesManager::getMutex().unlock();
                for (const auto &path : ResourcesManagerValues::paths) {
                    Json::getInstance().registerJsonFile(path);
                }
                ResourcesManager::getMutex().lock();
                init = true;
            }
            ResourcesManager::getMutex().unlock();
            if (ResourcesManagerValues::paths.size() <= static_cast<std::size_t>(type)) {
                Logger::fatal(
                    "RESOURCE_MANAGER: Invalid JsonType: "
                    + std::to_string(static_cast<std::size_t>(type)));
                return "";
            }
            return ResourcesManagerValues::paths[static_cast<std::size_t>(type)];
        }

        /**
         * @brief Init the resource manager
         * @param execPath The path of the executable
         */
        static void init(std::string execPath)
        {
            if (!boost::filesystem::exists(execPath)) {
                Logger::fatal("RESOURCE_MANAGER: Invalid execPath: " + execPath);
                return;
            }
            boost::filesystem::path path_tmp = execPath;
            path_tmp                         = path_tmp.remove_filename();
            path_tmp = boost::filesystem::absolute(path_tmp, boost::filesystem::initial_path());
#ifdef PACKAGED
            getRessourcePath() = ResourcesManager::getRessourcePathPackaged(path_tmp.string());
#else
            getRessourcePath() = ResourcesManager::getRessourcePathNormal(path_tmp.string());
#endif
            Logger::info("RESOURCE_MANAGER: Path Assets: " + getRessourcePath());
            PathResolver::setResolver(ResourcesManager::convertPath);
        }

        /**
         * @brief Get the resource path
         * @param path_const The path to convert
         * @return The converted path
         * @note You need to have 'assets' in your path at the beginning
         */
        static std::string convertPath(const std::string &path_const)
        {
            std::string found_path;

            ResourcesManager::getMutex().lock();
            std::string resource_path = ResourcesManager::getRessourcePath();
            if (resource_path.empty()) {
                ResourcesManager::getMutex().unlock();
                Logger::fatal("RESOURCE_MANAGER: need to call init first (" + path_const + ")");
                return "";
            }
            if (ResourcesManager::getMap().find(path_const) != ResourcesManager::getMap().end()) {
                ResourcesManager::getMutex().unlock();
                return ResourcesManager::getMap().at(path_const);
            }
            ResourcesManager::getMutex().unlock();
            if (found_path.empty() && path_const.starts_with("assets")) {
                if (ResourcesManager::isExists(resource_path, "share/r-type")) {
                    boost::filesystem::path path_tmp = resource_path;
                    path_tmp                         = path_tmp.append("share");
                    path_tmp                         = path_tmp.append("r-type");
                    std::string path(path_const, std::string("assets").length(), std::string::npos);
                    path_tmp = path_tmp.append(path);
                    path_tmp = path_tmp.make_preferred();
                    Logger::debug("RESOURCE_MANAGER: Path found: " + path_tmp.string());
                    found_path = path_tmp.string();
                }
                if (found_path.empty() && ResourcesManager::isExists(resource_path, "assets")) {
                    boost::filesystem::path path_tmp = resource_path;
                    path_tmp                         = path_tmp.append(path_const);
                    path_tmp                         = path_tmp.make_preferred();
                    Logger::debug("RESOURCE_MANAGER: Path found: " + path_tmp.string());
                    found_path = path_tmp.string();
                }
            }
            if (found_path.empty()) {
                if (ResourcesManager::isExists(resource_path, path_const)) {
                    boost::filesystem::path path_tmp = resource_path;
                    path_tmp                         = path_tmp.append(path_const);
                    path_tmp                         = path_tmp.make_preferred();
                    Logger::debug("RESOURCE_MANAGER: Path found: " + path_tmp.string());
                    found_path = path_tmp.string();
                }
                if (found_path.empty() && ResourcesManager::isExists(resource_path, "bin")) {
                    boost::filesystem::path path_tmp = ResourcesManager::getRessourcePath();
                    path_tmp                         = path_tmp.append("bin");
                    path_tmp                         = path_tmp.append(path_const);
                    path_tmp                         = path_tmp.make_preferred();
                    if (boost::filesystem::exists(path_tmp)) {
                        Logger::debug("RESOURCE_MANAGER: Path found: " + path_tmp.string());
                        found_path = path_tmp.string();
                    }
                }
            }
            ResourcesManager::getMutex().lock();
            ResourcesManager::getMap()[path_const] = found_path;
            ResourcesManager::getMutex().unlock();
            return found_path;
        }

    private:
        static std::string &getRessourcePath()
        {
            static std::string resourcePath;
            return resourcePath;
        }

        static std::unordered_map<std::string, std::string> &getMap()
        {
            static std::unordered_map<std::string, std::string> map;
            return map;
        }

        static std::mutex &getMutex()
        {
            static std::mutex mutex;
            return mutex;
        }

        static bool isExists(const std::string &path, const std::string &rest)
        {
            boost::filesystem::path path_tmp = path;
            path_tmp                         = path_tmp.append(rest);
            path_tmp                         = path_tmp.make_preferred();
            return boost::filesystem::exists(path_tmp);
        }

        static std::string getRessourcePathPackaged(const std::string &pathFolderInit)
        {
            boost::filesystem::path path_tmp = pathFolderInit;
            path_tmp                         = path_tmp.parent_path();

            if (!ResourcesManager::isExists(path_tmp.string(), "share")
                || !ResourcesManager::isExists(path_tmp.string(), "bin")) {
                Logger::fatal("RESOURCE_MANAGER: Path don't contain OK: " + path_tmp.string());
                return ResourcesManager::getRessourcePathNormal(pathFolderInit);
            }
            return path_tmp.string();
        }

        static std::string getRessourcePathNormal(const std::string &pathFolderInit)
        {
            boost::filesystem::path path_tmp = pathFolderInit;

            if (!ResourcesManager::isExists(path_tmp.string(), "assets")) {
                Logger::fatal("RESOURCE_MANAGER: Path don't contain OK: " + path_tmp.string());
                return "";
            }
            return path_tmp.string();
        }
};
