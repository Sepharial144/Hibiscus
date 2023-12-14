#ifndef _HIBISCUS_ALGORITHM_HPP_
#define _HIBISCUS_ALGORITHM_HPP_

#include "Hibicus_definitions.hpp"

#include <cstdint>
#include <list>
#include <string>

namespace hibiscus::algo {

int64_t fileFilter(std::string& line);
void extractFileName(const std::string& source, std::string& dest);
void updateList(std::list<FileChunk> list, const FileChunk& file_chunk);
void removeFromList(std::list<FileChunk> list, const std::string& file_name);

} // namespace hibiscus::algo

#endif // !_HIBISCUS_ALGORITHM_HPP_