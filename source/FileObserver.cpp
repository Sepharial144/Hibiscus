#include "FileObserver.hpp"
#include "Algorithm.hpp"
#include "FolderInformation.hpp"
#include "logger/logger.hpp"

#include <chrono>
#include <iostream>

FileObserver::FileObserver() { }
FileObserver::FileObserver(std::filesystem::path config_path, uint64_t wait_millisec)
    : m_status { enumObserverStatus::initialization }
    , m_lastStatus { enumObserverStatus::initialization }
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
        // logger::info("File observer observe directory ...");
        if (m_status == enumObserverStatus::initialization && m_lastStatus == enumObserverStatus::initialization) {
            // logger::info("File observer observe directory ... initialization");
            for (const auto& projectPath : std::filesystem::directory_iterator(m_configPath)) {
                // list of projects is empty, need initialization
                size_t countFiles = countFilesInFolder(projectPath);
                std::list<FileInfo> listFiles(countFiles);

                for (const auto& projectFile : std::filesystem::directory_iterator(projectPath.path())) {
                    // logger::debug("   -- {} {}", projectFile.path(), hibiscus::algo::time_to_string(projectFile));
                    listFiles.emplace_back(FileInfo { projectFile, std::filesystem::last_write_time(projectFile) });
                }

                // logger::debug("Count of files ... ", listFiles.size());
                m_status = enumObserverStatus::running;
                // logger::info("File observer observe directory ... initialization complete");
                break;
            }
        }

        if (m_status == enumObserverStatus::running) {
            // logger::info("File observer observe directory ... running");
            for (const auto& projectPath : std::filesystem::directory_iterator(m_configPath)) {
                // list of projects is empty, need initialization
                for (const auto& projectFile : std::filesystem::directory_iterator(projectPath.path())) {
                    // logger::info("File observer observe directory ... check ", projectPath.path());
                }
                continue;
            }
        }
        /*
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
        }
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

        }
            */
    }
    m_mtx.unlock();
}

// TODO: make it faster, not critical, rewrite the function
int64_t FileObserver::fileFilter(const std::string_view& file_path)
{
    // logger::info("File observer file filter ...");
    if (file_path.rfind(".toml") == std::string_view::npos)
        return enumFileFilter::INVALID_FILE;

    // logger::info("File observer file filter valid file ...");
    const size_t position = file_path.rfind('/');
    const char letter = file_path[position + 1];
    if (letter == '#' || letter == '!') // is it really necessary to use '!'?
        return enumFileFilter::COMMENTED_FILE;

    // logger::info("File observer file filter valid file ... complete");
    return position;
}

size_t FileObserver::countFilesInFolder(std::filesystem::path path)
{
    // logger::info("File observer count files in folder", path.c_str(), " ...");
    using std::filesystem::directory_iterator;
    // logger::info("File observer count files in folder ... complete");
    return std::distance(directory_iterator(path), directory_iterator {});
}

FolderInformation* FileObserver::findFolderInformation(std::filesystem::path path)
{
    // logger::info("File observer find folder information ...");
    auto folderIsFound = [&](const FolderInformation& folderInfo) {
        if (folderInfo.getRootFolder().compare(path) == 0)
            return true;
        return false;
    };

    auto end = m_projectList.end();
    if (auto it = std::find_if(m_projectList.begin(), end, folderIsFound); it != end) {
        // logger::info("File observer find folder information ... found");
        return &(*it);
    }
    // logger::info("File observer find folder information ... not found");
    return nullptr;
}

void FileObserver::updateProjectInformation(const std::filesystem::path path, std::list<FileInfo>& file_list)
{
    // logger::info("File observer update project information ...");
    auto pFolderInformation = findFolderInformation(path);
    if (pFolderInformation != nullptr) {
        pFolderInformation->setFolderInformation(file_list);
        // logger::info("File observer update project information ... complete");
        return;
    }

    m_projectList.emplace_back(FolderInformation { path, file_list });
    // logger::info("File observer update project information ... complete");
}