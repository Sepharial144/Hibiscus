#ifndef _HIBISCUS_FILE_OBSERVER_HPP_
#define _HIBISCUS_FILE_OBSERVER_HPP_

#include "Hibicus_definitions.hpp"

#include <thread>
#include <mutex>

#include <filesystem>
#include <list>
#include <string>
#include <functional>

class FileObserver
{
public:
    explicit FileObserver();
    explicit FileObserver(std::filesystem::path config_path);
    ~FileObserver();
    //void getListFiles(std::vector<std::string>& res_container);
    void start();

private:
    void observeDirectory();

private:
    bool isRunning = false;
    std::filesystem::path m_configPath;
    std::list<FileChunk> m_fileList;

    std::thread m_mainLoop;
    std::mutex m_mtx;
};

#endif // !_HIBISCUS_FILE_OBSERVER_HPP_