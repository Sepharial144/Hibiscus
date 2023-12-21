#ifndef _HIBISCUS_DIRECTORY_STORAGE_HPP_
#define _HIBISCUS_DIRECTORY_STORAGE_HPP_

#include "Hibicus_definitions.hpp"

#include <filesystem>
#include <list>
#include <mutex>
#include <string>
#include <tuple>

class FolderInformation;

class DirectoryStorage {
private:
    std::mutex mtx;

public:
    explicit DirectoryStorage();
    explicit DirectoryStorage(const std::filesystem::path config_path);
    virtual ~DirectoryStorage();

    std::filesystem::path configPath() const;
    auto directoryList() const -> const std::list<FolderInformation>&;
    size_t size() const;

    void emplace_back(const FolderInformation&& folder_info);
    void removeByHash(const size_t hash);
    size_t countFilesInFolder(std::filesystem::path path) const;
    auto sizeAndCount() const -> std::tuple<size_t, size_t>;
    FolderInformation* findFolderInformation(std::filesystem::path path);

    void updateProjectInformation(const std::filesystem::path path, std::list<FileInfo>& file_list);

private:
    std::filesystem::path m_configPath;
    std::list<FolderInformation> m_projectList;
};

#endif // !_HIBISCUS_DIRECTORY_STORAGE_HPP_