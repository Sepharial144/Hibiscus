#include "FileObserver.hpp"
#include "Algorithm.hpp"

#include <chrono>
#include <iostream>

FileObserver::FileObserver() { }
FileObserver::FileObserver(std::filesystem::path config_path, uint64_t wait_millisec)
    : m_status { enumObserverStatus::initialization }
    , m_configPath { config_path }
    , m_milliseconds { wait_millisec }
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

void FileObserver::stop()
{
    isRunning = false;
    if (m_mainLoop.joinable())
        m_mainLoop.join();
}

// TODO: add isRunning variable into all condition statement to check loop status
void FileObserver::observeDirectory()
{
    isRunning = true;
    while (isRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(m_milliseconds));
        m_mtx.lock();

        for (const auto& projectPath : std::filesystem::directory_iterator(m_configPath)) {

            // list of projects is empty, need initialization
            if (m_status == enumObserverStatus::initialization) {
                for (const auto& projectFile : std::filesystem::directory_iterator(projectPath.path())) {
                    std::cout << projectFile.path() << std::endl;
                }
                m_status = enumObserverStatus::running;
            }
            if (m_status == enumObserverStatus::running) {
            }
            /*
            for (const auto& projectFile : std::filesystem::directory_iterator(projectPath.path())) {
                std::cout << projectFile.path() << std::endl;
                
            const std::string_view filePath { projectPath.path() };

            int64_t position = fileFilter(filePath);
            if (position == enumFileFilter::INVALID_FILE) {
                std::cout << "Invalid file: " << entry.path() << std::endl;
                continue;
            }

            if (position == COMMENTED_FILE) {
                std::cout << "Commented file: " << entry.path() << std::endl;
                continue;
            }

            if (position) {
                auto ftime = std::filesystem::last_write_time(entry.path());
                size_t timestamp = std::chrono::duration_cast<std::chrono::seconds>(
                    ftime.time_since_epoch())
                                       .count();
                m_projectList.emplace_back(entry.path(), timestamp);
                FileChunk fileChunk = FileChunk { entry.path(), timestamp };
                hibiscus::algo::updateList(m_fileList, fileChunk);
                continue;
            }
            if (position > 0)
                hibiscus::algo::removeFromList(m_fileList, file);
            */
        }
    }
    m_mtx.unlock();
}

// TODO: make it faster, not critical, rewrite the function
int64_t FileObserver::fileFilter(const std::string_view& file_path)
{
    if (file_path.rfind(".toml") == std::string_view::npos)
        return enumFileFilter::INVALID_FILE;

    const size_t position = file_path.rfind('/');
    const char letter = file_path[position + 1];
    if (letter == '#' || letter == '!') // is it really necessary to use '!'?
        return enumFileFilter::COMMENTED_FILE;

    return position;
}