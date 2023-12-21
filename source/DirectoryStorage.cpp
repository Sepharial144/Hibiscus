#include "DirectoryStorage.hpp"
#include "FolderInformation.hpp"
#include "logger/logger.hpp"

#include <algorithm>
#include <utility>

DirectoryStorage::DirectoryStorage()
{
}

DirectoryStorage::DirectoryStorage(const std::filesystem::path config_path)
    : m_configPath { config_path }
{
}

DirectoryStorage::~DirectoryStorage()
{
}

std::filesystem::path DirectoryStorage::configPath() const
{
    return m_configPath;
}

size_t DirectoryStorage::size() const
{
    return m_projectList.size();
}

void DirectoryStorage::emplace_back(const FolderInformation&& folder_info)
{
    const std::lock_guard<std::mutex> lock(mtx);
    m_projectList.emplace_back(folder_info);
}

size_t DirectoryStorage::countFilesInFolder(std::filesystem::path path) const
{
    logger::info("Directory storage count files in folder", path.c_str(), " ...");
    using std::filesystem::directory_iterator;
    auto distance = std::distance(directory_iterator(path), directory_iterator {});
    logger::info("Directory storage count files in folder ... complete: {}", distance);
    return distance;
}

auto DirectoryStorage::sizeAndCount() const -> std::tuple<size_t, size_t>
{
    return {m_projectList.size(), countFilesInFolder(m_configPath)};
}

FolderInformation* DirectoryStorage::findFolderInformation(std::filesystem::path path)
{
    logger::info("Directory storage find folder information ...");
    auto folderIsFound = [&](const FolderInformation& folderInfo) {
        if (folderInfo.getRootFolder().compare(path) == 0)
            return true;
        return false;
    };

    auto end = m_projectList.end();
    if (auto it = std::find_if(m_projectList.begin(), end, folderIsFound); it != end) {
        logger::info("Directory storage find folder information ... found");
        return &(*it);
    }
    logger::info("Directory storage find folder information ... not found");
    return nullptr;
}

void DirectoryStorage::updateProjectInformation(const std::filesystem::path path, std::list<FileInfo>& file_list)
{
    logger::info("Directory storage update project information ...");
    auto pFolderInformation = findFolderInformation(path);
    if (pFolderInformation != nullptr) {
        pFolderInformation->setFolderInformation(file_list);
        logger::info("Directory storage update project information ... complete");
        return;
    }

    m_projectList.emplace_back(FolderInformation { path, file_list });
    logger::info("Directory storage update project information ... complete");
}