#ifndef _HIBISCUS_FILE_OBSERVER_HPP_
#define _HIBISCUS_FILE_OBSERVER_HPP_

#include "DirectoryStorage.hpp"
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

class FileObserver : virtual public DirectoryStorage {
public:
    explicit FileObserver();
    explicit FileObserver(std::filesystem::path config_path, uint64_t wait_millisec);
    ~FileObserver();

    void start();
    void stop();

private:
    void observeDirectory();
    void initializationDirectory();
    void runningDirectory();

    std::list<FileInfo> createFolderList(std::filesystem::path project_path);
    int64_t fileFilter(const std::string_view& file_path);
    size_t countFilesInFolder(std::filesystem::path path);

private:
    bool isRunning = false;
    enumObserverStatus m_status;
    enumObserverStatus m_lastStatus;
    uint64_t m_milliseconds;
    std::thread m_mainLoop;
};

#endif // !_HIBISCUS_FILE_OBSERVER_HPP_