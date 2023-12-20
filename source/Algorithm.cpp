#include "Algorithm.hpp"
#include <chrono>
#include <cstring>
#include <filesystem>

namespace hibiscus::algo {

int64_t fileFilter(std::string& line)
{
    const size_t position = line.rfind('/');
    if (line.rfind(".yaml") == std::string::npos)
        return -1;
    const char letter = line[position + 1];
    if (letter == '#' || letter == '!')
        return position;
    return 0;
}

void extractFileName(const std::string& source, std::string& dest)
{
    const size_t position = source.rfind('/');
    dest = source.substr(position + 1, source.size() - position);
}

void updateList(std::list<FileChunk> list, const FileChunk& file_chunk)
{
    for (auto& el : list) {
        if (file_chunk.file_name == el.file_name) {
            el.timestamp = file_chunk.timestamp;
            return;
        }
    }
    list.push_back(file_chunk);
}

void removeFromList(std::list<FileChunk> list, const std::string& file_name)
{
    list.remove_if(
        [&](FileChunk& chunk) { return chunk.file_name == file_name; });
}

std::string to_string(const std::filesystem::file_time_type& ftime)
{
#if __cpp_lib_format
    return std::format("{:%c}", ftime);
#else
    std::time_t cftime = std::chrono::system_clock::to_time_t(
        std::chrono::file_clock::to_sys(ftime));
    std::string str = std::asctime(std::localtime(&cftime));
    str.pop_back(); // rm the trailing '\n' put by `asctime`
    return str;
#endif
}

} // namespace hibiscus::algo