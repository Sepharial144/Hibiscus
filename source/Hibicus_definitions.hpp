#ifndef HIBICUS_DEFINITIONS_HPP_
#define HIBICUS_DEFINITIONS_HPP_

#include <filesystem>

struct FileChunk {
    std::string file_name;
    size_t timestamp;
};

#endif // !HIBICUS_DEFINITIONS_HPP_