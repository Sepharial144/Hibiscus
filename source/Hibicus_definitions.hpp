#ifndef _HIBICUS_DEFINITIONS_HPP_
#define _HIBICUS_DEFINITIONS_HPP_

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

enum enumFileFilter : int64_t {
    INVALID_FILE = -1,
    COMMENTED_FILE = 0
};

enum class enumObserverStatus {
    initialization,
    active,
    running
};

enum class enumFolderStatus {
    none,
    addFolder,
    updateFolder,
    deleteFolder
};

struct FileInfo {
    std::filesystem::path path;
    std::filesystem::file_time_type last_time;
};

struct FileChunk {
    std::string file_name;
    size_t timestamp;
};

struct MarkedFolders {
    enumFolderStatus status;
    std::vector<std::filesystem::path> m_pathArray;
};

#endif // !_HIBICUS_DEFINITIONS_HPP_