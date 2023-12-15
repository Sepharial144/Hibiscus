#ifndef _HIBISCUS_FILE_OBSERVER_HPP_
#define _HIBISCUS_FILE_OBSERVER_HPP_

#include "Hibicus_definitions.hpp"

#include <mutex>
#include <thread>

#include <cstdint>
#include <filesystem>
#include <functional>
#include <list>
#include <string>
#include <string_view>

class FileObserver {
public:
    explicit FileObserver();
    explicit FileObserver(std::filesystem::path config_path, uint64_t wait_millisec);
    ~FileObserver();
    // void getListFiles(std::vector<std::string>& res_container);
    void start();
    void stop();

private:
    void observeDirectory();
    int64_t fileFilter(const std::string_view& file_path);

private:
    bool isRunning = false;
    enumObserverStatus m_status;
    std::filesystem::path m_configPath;
    uint64_t m_milliseconds;
    std::list<ProjectInfo> m_projectList;

    std::thread m_mainLoop;
    std::mutex m_mtx;
};

#endif // !_HIBISCUS_FILE_OBSERVER_HPP_