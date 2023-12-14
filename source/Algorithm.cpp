#include "Algorithm.hpp"
#include <cstring>

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

} // namespace hibiscus::algo