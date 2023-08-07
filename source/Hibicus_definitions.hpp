#ifndef HIBICUS_DEFINITIONS_HPP_
#define HIBICUS_DEFINITIONS_HPP_

#include <filesystem>

struct FileChunk
{
    std::string file_name;
    std::filesystem::file_time_type last_write_time;
};

#endif // !HIBICUS_DEFINITIONS_HPP_