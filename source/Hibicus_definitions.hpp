#ifndef _HIBICUS_DEFINITIONS_HPP_
#define _HIBICUS_DEFINITIONS_HPP_

#include <cstdint>
#include <filesystem>
#include <string>

enum enumFileFilter : int64_t {
    INVALID_FILE = -1,
    COMMENTED_FILE = 0
};

enum class enumObserverStatus {
    initialization,
    active,
    running
};

struct FileInfo {
    std::filesystem::path path;
    std::filesystem::file_time_type last_time;
};

struct FileChunk {
    std::string file_name;
    size_t timestamp;
};

#endif // !_HIBICUS_DEFINITIONS_HPP_