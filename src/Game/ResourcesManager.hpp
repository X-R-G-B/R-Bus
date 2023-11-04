#pragma once

#include <string>
#include <boost/asio.hpp>
#include "B-luga/Json.hpp"
#include "B-luga/Logger.hpp"
#include "B-luga/PathResolver.hpp"
#include "ResourcesManager.hpp"
#include "boost/filesystem.hpp"

enum class JsonType : std::size_t {
    DEFAULT_PLAYER = 0,
    DEFAULT_ENEMY,
    TERMINATORBOSS,
    BULLETS,
    WAVE,
    MENU_DATA,
    SELECT_LOBBY,
    PARALLAX,
    MAXTYPE
};

const std::vector<std::string> paths = {
    "assets/Json/playerData.json",
    "assets/Json/enemyData.json",
    "assets/Json/terminatorData.json",
    "assets/Json/bullets.json",
    "assets/Json/wave.json",
    "assets/Json/menu.json",
    "assets/Json/selectLobby.json",
    "assets/Json/parallaxData.json",
};

class ResourcesManager {
    public:

        static std::string getPathByJsonType(JsonType type)
        {
            static bool init = false;

            if (!init) {
                for (const auto &path : paths) {
                    Json::getInstance().registerJsonFile(path);
                }
                init = true;
            }
            if (paths.size() <= static_cast<std::size_t>(type)) {
                return "";
            }
            return paths[static_cast<std::size_t>(type)];
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

            if (getRessourcePath().empty()) {
                Logger::fatal("RESOURCE_MANAGER: need to call init first (" + path_const + ")");
                return "";
            }
            if (ResourcesManager::getMap().find(path_const) != ResourcesManager::getMap().end()) {
                return ResourcesManager::getMap().at(path_const);
            }
            if (found_path.empty() && path_const.starts_with("assets")) {
                if (ResourcesManager::isExists(ResourcesManager::getRessourcePath(), "share/r-type")) {
                    boost::filesystem::path path_tmp = ResourcesManager::getRessourcePath();
                    path_tmp = path_tmp.append("share");
                    path_tmp = path_tmp.append("r-type");
                    std::string path(path_const, std::string("assets").length(), std::string::npos);
                    path_tmp = path_tmp.append(path);
                    path_tmp = path_tmp.make_preferred();
                    Logger::debug("RESOURCE_MANAGER: Path found: " + path_tmp.string());
                    found_path = path_tmp.string();
                }
                if (found_path.empty() && ResourcesManager::isExists(ResourcesManager::getRessourcePath(), "assets")) {
                    boost::filesystem::path path_tmp = ResourcesManager::getRessourcePath();
                    path_tmp = path_tmp.append(path_const);
                    path_tmp = path_tmp.make_preferred();
                    Logger::debug("RESOURCE_MANAGER: Path found: " + path_tmp.string());
                    found_path = path_tmp.string();
                }
            }
            if (found_path.empty()) {
                if (ResourcesManager::isExists(ResourcesManager::getRessourcePath(), path_const)) {
                    boost::filesystem::path path_tmp = ResourcesManager::getRessourcePath();
                    path_tmp = path_tmp.append(path_const);
                    path_tmp = path_tmp.make_preferred();
                    Logger::debug("RESOURCE_MANAGER: Path found: " + path_tmp.string());
                    found_path = path_tmp.string();
                }
                if (found_path.empty() && ResourcesManager::isExists(ResourcesManager::getRessourcePath(), "bin")) {
                    boost::filesystem::path path_tmp = ResourcesManager::getRessourcePath();
                    path_tmp = path_tmp.append("bin");
                    path_tmp = path_tmp.append(path_const);
                    path_tmp = path_tmp.make_preferred();
                    if (boost::filesystem::exists(path_tmp)) {
                        Logger::debug("RESOURCE_MANAGER: Path found: " + path_tmp.string());
                        found_path = path_tmp.string();
                    }
                }
            }
            ResourcesManager::getMap()[path_const] = found_path;
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

        static bool isExists(const std::string &path, const std::string &rest)
        {
            boost::filesystem::path path_tmp = path;
            path_tmp = path_tmp.append(rest);
            path_tmp = path_tmp.make_preferred();
            return boost::filesystem::exists(path_tmp);
        }

        static std::string getRessourcePathPackaged(const std::string &pathFolderInit)
        {
            boost::filesystem::path path_tmp = pathFolderInit;
            path_tmp = path_tmp.parent_path();

            if (!ResourcesManager::isExists(path_tmp.string(), "share") || !ResourcesManager::isExists(path_tmp.string(), "bin")) {
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
