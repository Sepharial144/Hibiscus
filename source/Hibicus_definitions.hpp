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

struct ProjectInfo {
    std::string path;
    size_t start_time;
    size_t last_time;
    size_t delay_time;
};

struct FileChunk {
    std::string file_name;
    size_t timestamp;
};

#endif // !_HIBICUS_DEFINITIONS_HPP_