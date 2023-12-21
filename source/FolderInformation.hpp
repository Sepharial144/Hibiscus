#ifndef _HIBISCUS_FOLDER_INFORMATION_HPP_
#define _HIBISCUS_FOLDER_INFORMATION_HPP_

#include "Hibicus_definitions.hpp"

#include <filesystem>
#include <list>

class FolderInformation {
public:
    explicit FolderInformation();
    explicit FolderInformation(const std::filesystem::path path, const std::list<FileInfo>& listFiles);
    ~FolderInformation();

    void setFolderInformation(std::list<FileInfo>& list_files);
    void setRootFolder(const std::filesystem::path path);

    const size_t size() const;
    const size_t hash() const;
    std::filesystem::path getRootFolder() const;
    const std::list<FileInfo>& getFolderFileList() const;

private:
    std::filesystem::path m_rootFolder;
    std::list<FileInfo> m_listFileInfo;
};

#endif // !_HIBISCUS_FOLDER_INFORMATION_HPP_