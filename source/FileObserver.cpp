#include "FileObserver.hpp"
#include "Algorithm.hpp"
#include "FolderInformation.hpp"
#include "logger/logger.hpp"

#include <cmath>
#include <vector>

FileObserver::FileObserver() { }
FileObserver::FileObserver(std::filesystem::path config_path, uint64_t wait_millisec)
    : DirectoryStorage { config_path }
    , m_status { enumObserverStatus::initialization }
    , m_lastStatus { enumObserverStatus::initialization }
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
        logger::info("File observer observe directory ...");
        if (m_status == enumObserverStatus::initialization && m_lastStatus == enumObserverStatus::initialization) {
            initializationDirectory();
        }

        if (m_status == enumObserverStatus::running && m_lastStatus == enumObserverStatus::initialization) {
            runningDirectory();
        }
    }
}

void FileObserver::initializationDirectory()
{
    using filesystem_iteraror = std::filesystem::directory_iterator;

    logger::info("File observer observe directory ... initialization");
    for (const auto& projectEntry : filesystem_iteraror(DirectoryStorage::configPath())) {

        size_t count = DirectoryStorage::countFilesInFolder(projectEntry);
        auto listFiles = createFolderList(projectEntry);

        logger::info("File observer write project information ... {}", projectEntry.path().c_str());
        DirectoryStorage::emplace_back(FolderInformation { projectEntry, listFiles });
        logger::info("File observer write project information ... complete");
    }
    logger::debug("File observer count of projects ... {}", DirectoryStorage::size());
    m_status = enumObserverStatus::running;
    logger::info("File observer observe directory ... initialization complete");
}

// TODO: need refactoring
void FileObserver::runningDirectory()
{
    using filesystem_iteraror = std::filesystem::directory_iterator;

    logger::info("File observer observe directory ... running");

    logger::info("File observer observe directory count of folders ...");
    const auto [size, count] = DirectoryStorage::sizeAndCount();
    if (size == count) {
        logger::info("File observer observe directory ... equal");
        // check inner files
        return;
    }

    if (size > count) {
        logger::info("File observer observe is found deleted directories ... less");
        for (const auto& folderInformation : DirectoryStorage::directoryList()) {
            int32_t rc = -1;
            auto rootFolder = folderInformation.getRootFolder();
            auto hash = std::filesystem::hash_value(rootFolder);
            for (const auto& projectEntry : filesystem_iteraror(DirectoryStorage::configPath())) {
                const int rc = projectEntry.path().compare(rootFolder);
                if (rc == 0)
                    break;
            }
            if (rc != 0) {
                DirectoryStorage::removeByHash(hash);
            }
            // TODO: create mechanizm to update folders
        }
    }
    if (size < count) {
        logger::info("File observer observe is found new directories ... greater");

        for (const auto& projectEntry : filesystem_iteraror(DirectoryStorage::configPath())) {
            auto pFolderInformation = findFolderInformation(projectEntry.path());
            if (pFolderInformation == nullptr) {
                auto listFiles = createFolderList(projectEntry);
                logger::info("File observer add new directory ... {}", projectEntry.path().c_str());
                DirectoryStorage::emplace_back(FolderInformation { projectEntry, listFiles });
                logger::info("File observer add new directory ... complete");

                // TODO: create mechanizm to update folders
            }
        }
    }
}

auto FileObserver::createFolderList(std::filesystem::path project_path) -> std::list<FileInfo>
{
    using filesystem_iteraror = std::filesystem::directory_iterator;

    logger::debug("File observer: inspecting ... {}", project_path.c_str());
    size_t count = DirectoryStorage::countFilesInFolder(project_path);
    std::list<FileInfo> listFiles;
    listFiles.resize(count);

    for (const auto& file : filesystem_iteraror(project_path)) {
        int64_t filter = fileFilter(file.path().c_str());

        if (filter == enumFileFilter::INVALID_FILE)
            logger::debug(" -- {} {} {}",
                file.path().c_str(),
                std::filesystem::last_write_time(file),
                " invalid");

        if (filter == enumFileFilter::COMMENTED_FILE)
            logger::debug(" -- {} {} {}",
                file.path().c_str(),
                std::filesystem::last_write_time(file),
                " commented");

        if (filter) {
            auto ftime = std::filesystem::last_write_time(file);
            logger::debug(" -- {} {} {}",
                file.path().c_str(),
                ftime,
                " valid");
            listFiles.emplace_back(FileInfo { file, ftime });
        }
    }
    logger::debug("File observer: inspecting ... done");

    return listFiles;
}

// TODO: make it faster, not critical, rewrite the function
int64_t FileObserver::fileFilter(const std::string_view& file_path)
{
    if (file_path.rfind(".toml") == std::string_view::npos || file_path.rfind(".oml") == std::string_view::npos)
        return enumFileFilter::INVALID_FILE;

    const size_t position = file_path.rfind('/');
    const char letter = file_path[position + 1];
    if (letter == '#' || letter == '!') // is it really necessary to use '!'?
        return enumFileFilter::COMMENTED_FILE;

    return position;
}