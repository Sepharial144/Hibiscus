#include "FileObserver.hpp"
#include "Algorithm.hpp"

#include <chrono>
#include <iostream>

FileObserver::FileObserver() { }
FileObserver::FileObserver(std::filesystem::path config_path)
    : m_configPath { config_path }
{
}

FileObserver::~FileObserver()
{
    if (m_mainLoop.joinable())
        m_mainLoop.join();
}

/*
void FileObserver::getListFiles(std::vector<std::string>& res_container)
{
    const std::lock_guard<std::mutex> lock(m_mtx);
    res_container.reserve(m_fileList.size());
    std::copy(m_fileList.begin(), m_fileList.end(),
std::back_inserter(res_container));
}
*/

void FileObserver::start()
{
    m_mainLoop = std::thread(&FileObserver::observeDirectory, this);
}

void FileObserver::observeDirectory()
{
    isRunning = true;
    while (isRunning) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        m_mtx.lock();

        for (const auto& entry :
            std::filesystem::directory_iterator(m_configPath)) {
            std::string file { entry.path() };

            int64_t position = hibiscus::algo::fileFilter(file);
            if (position == 0) {
                auto ftime = std::filesystem::last_write_time(entry.path());
                size_t timestamp = std::chrono::duration_cast<std::chrono::seconds>(
                    ftime.time_since_epoch())
                                       .count();
                FileChunk fileChunk = FileChunk { file, timestamp };
                hibiscus::algo::updateList(m_fileList, fileChunk);
                continue;
            }
            if (position > 0)
                hibiscus::algo::removeFromList(m_fileList, file);
        }
        m_mtx.unlock();
    }
}