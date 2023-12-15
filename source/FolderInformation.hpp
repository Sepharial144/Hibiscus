#ifndef _HIBISCUS_FOLDER_INFORMATION_HPP_
#define _HIBISCUS_FOLDER_INFORMATION_HPP_

#include "Hibicus_definitions.hpp"

#include <list>
#include <filesystem>

class FolderInformation
{
    private:
        struct FileInfo
        {
            std::filesystem::path;
            uint64_t create_timestamp;
            uint64_t change_timestamp;
        };

    public:
        explicit FolderInformation();
        ~FolderInformation();

        std::filesystem::path getRootFolder();
        void upateFileInformation(std::filesystem::path);

    private:
        std::filesystem::path m_rootFolder;
        std::list<FileInfo> m_listFileInfo;
}

#endif // !_HIBISCUS_FOLDER_INFORMATION_HPP_