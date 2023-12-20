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

class FolderInformation;

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
    void initializationDirectory();
    void runningDirectory();

    int64_t fileFilter(const std::string_view& file_path);
    size_t countFilesInFolder(std::filesystem::path path);
    FolderInformation* findFolderInformation(std::filesystem::path root_folder);
    void updateProjectInformation(const std::filesystem::path path, std::list<FileInfo>& file_list);

private:
    bool isRunning = false;
    enumObserverStatus m_status;
    enumObserverStatus m_lastStatus;
    std::filesystem::path m_configPath;
    uint64_t m_milliseconds;
    std::list<FolderInformation> m_projectList;

    std::thread m_mainLoop;
    std::mutex m_mtx;
};

#endif // !_HIBISCUS_FILE_OBSERVER_HPP_