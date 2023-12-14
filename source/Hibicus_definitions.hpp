#ifndef HIBICUS_DEFINITIONS_HPP_
#define HIBICUS_DEFINITIONS_HPP_

#include <cstdint>
#include <filesystem>
#include <string>

enum class enumObserverStatus {
    initialization,
    active,
    running
}

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

#endif // !HIBICUS_DEFINITIONS_HPP_