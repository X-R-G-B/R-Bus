#include "ResourcesManager.hpp"
#include "boost/filesystem.hpp"

namespace ECS {
    // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
    std::string ResourcesManager::_resourcePath;
    // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

    void ResourcesManager::init(std::string execPath)
    {
        boost::filesystem::path path_tmp = execPath;
        path_tmp = path_tmp.remove_filename();
        path_tmp = boost::filesystem::absolute(path_tmp, boost::filesystem::initial_path());
#ifndef PACKAGED
        path_tmp.append("assets");
        if (!boost::filesystem::exists(path_tmp)) {
            Logger::fatal("RESOURCE_MANAGER: Path not found: " + path_tmp.string());
        }
        _resourcePath = path_tmp.string();
        Logger::info("RESOURCE_MANAGER: Path Assets: " + _resourcePath);
        return;
#endif
        path_tmp = path_tmp.parent_path();
        path_tmp.append("share");
        if (!boost::filesystem::exists(path_tmp)) {
            Logger::fatal("RESOURCE_MANAGER: Path not found: " + path_tmp.string());
        }
#ifdef CLIENT
        path_tmp.append("r-type_client");
        if (!boost::filesystem::exists(path_tmp)) {
            Logger::fatal("RESOURCE_MANAGER: Path not found: " + path_tmp.string());
        }
#else
        path_tmp.append("r-type_server");
        if (!boost::filesystem::exists(path_tmp)) {
            Logger::fatal("RESOURCE_MANAGER: Path not found: " + path_tmp.string());
        }
#endif
        _resourcePath = path_tmp.string();
        Logger::info("RESOURCE_MANAGER: Path Assets: " + _resourcePath);
    }

    std::string ResourcesManager::convertPath(const std::string &path_const)
    {
        if (_resourcePath.empty()) {
            Logger::fatal("RESOURCE_MANAGER: need to call init first");
            return "";
        }
        std::string path(path_const, std::string("assets").length(), std::string::npos);
        boost::filesystem::path path_tmp = _resourcePath;
        path_tmp.append(path);
        if (!boost::filesystem::exists(path_tmp)) {
            Logger::fatal("RESOURCE_MANAGER: Path not found: " + path_tmp.string());
        } else {
            Logger::debug("RESOURCE_MANAGER: Path found: " + path_tmp.string());
        }
        return path_tmp.string();
    }
}
