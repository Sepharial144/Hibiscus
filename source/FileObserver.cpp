#include "FileObserver.hpp"
#include "Algorithm.hpp"
#include "FolderInformation.hpp"
#include "logger/logger.hpp"

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
    logger::info("File observer observe directory ... initialization");
    for (const auto& projectPath : std::filesystem::directory_iterator(m_configPath)) {

        size_t countFiles = countFilesInFolder(projectPath);
        std::list<FileInfo> listFiles(countFiles);

        for (const auto& projectFile : std::filesystem::directory_iterator(projectPath.path())) {
            int64_t filter = fileFilter(projectFile.path().c_str());

            if (filter) {
                auto ftime = std::filesystem::last_write_time(projectFile);
                logger::debug(" -- {} {} {}",
                    projectFile.path().c_str(),
                    ftime,
                    " valid");
                listFiles.emplace_back(FileInfo { projectFile, ftime});
                continue;
            }

            if (filter == enumFileFilter::INVALID_FILE)
                logger::debug(" -- {} {} {}",
                    projectFile.path().c_str(),
                    std::filesystem::last_write_time(projectFile),
                    " invalid");

            if (filter == enumFileFilter::COMMENTED_FILE)
                logger::debug(" -- {} {} {}",
                    projectFile.path().c_str(),
                    std::filesystem::last_write_time(projectFile),
                    " commented");
        }
        logger::info("File observer write project information ... {}", projectPath.path().c_str());
        const std::lock_guard<std::mutex> lock(m_mtx);
        m_projectList.emplace_back(FolderInformation { projectPath, listFiles });
        logger::info("File observer write project information ... complete");
    }
    logger::debug("File observer count of projects ... {}", m_projectList.size());
    m_status = enumObserverStatus::running;
    logger::info("File observer observe directory ... initialization complete");
}

void FileObserver::runningDirectory()
{
    logger::info("File observer observe directory ... running");

    logger::info("File observer observe directory count of folders ...");
    auto isFoldersListsDifferent = (m_projectList.size() != countFilesInFolder(m_configPath)) ? true : false;
    if (isFoldersListsDifferent) {
        logger::info("File observer observe directory count of folders ... different");
        // todo differenciation of folders
        return;
    }

    logger::info("File observer observe directory count of folders ... same");

    for (const auto& projectPath : std::filesystem::directory_iterator(m_configPath)) {
        // list of projects is empty, need initialization
        size_t countFiles = countFilesInFolder(projectPath);
        std::list<FileInfo> listFiles(countFiles);

        for (const auto& projectFile : std::filesystem::directory_iterator(projectPath.path())) {
            std::cout << projectFile.path() << " " << std::filesystem::last_write_time(projectFile) << std::endl;

            logger::warning("File observer check file ...");
            int64_t filter = fileFilter(projectFile.path().c_str());

            if (filter == enumFileFilter::INVALID_FILE)
                logger::warning("File observer check file ... invalid file");

            if (filter == enumFileFilter::COMMENTED_FILE)
                logger::warning("File observer check file ... commented file");

            if (filter) {
                listFiles.emplace_back(FileInfo { projectFile, std::filesystem::last_write_time(projectFile) });
                logger::info("File observer check file ... complete");
            }
        }

        logger::debug("Count of files ... {}", listFiles.size());

        m_status = enumObserverStatus::running;
        logger::info("File observer observe directory ... initialization complete");
    }
}

// TODO: make it faster, not critical, rewrite the function
int64_t FileObserver::fileFilter(const std::string_view& file_path)
{
    logger::info("File observer file filter ...");
    if (file_path.rfind(".toml") == std::string_view::npos)
        return enumFileFilter::INVALID_FILE;

    logger::info("File observer file filter valid file ...");
    const size_t position = file_path.rfind('/');
    const char letter = file_path[position + 1];
    if (letter == '#' || letter == '!') // is it really necessary to use '!'?
        return enumFileFilter::COMMENTED_FILE;

    logger::info("File observer file filter valid file ... complete");
    return position;
}

size_t FileObserver::countFilesInFolder(std::filesystem::path path)
{
    logger::info("File observer count files in folder", path.c_str(), " ...");
    using std::filesystem::directory_iterator;
    auto distance = std::distance(directory_iterator(path), directory_iterator {});
    logger::info("File observer count files in folder ... complete: {}", distance);
    return distance;
}

FolderInformation* FileObserver::findFolderInformation(std::filesystem::path path)
{
    logger::info("File observer find folder information ...");
    auto folderIsFound = [&](const FolderInformation& folderInfo) {
        if (folderInfo.getRootFolder().compare(path) == 0)
            return true;
        return false;
    };

    auto end = m_projectList.end();
    if (auto it = std::find_if(m_projectList.begin(), end, folderIsFound); it != end) {
        logger::info("File observer find folder information ... found");
        return &(*it);
    }
    logger::info("File observer find folder information ... not found");
    return nullptr;
}

void FileObserver::updateProjectInformation(const std::filesystem::path path, std::list<FileInfo>& file_list)
{
    logger::info("File observer update project information ...");
    auto pFolderInformation = findFolderInformation(path);
    if (pFolderInformation != nullptr) {
        pFolderInformation->setFolderInformation(file_list);
        logger::info("File observer update project information ... complete");
        return;
    }

    m_projectList.emplace_back(FolderInformation { path, file_list });
    logger::info("File observer update project information ... complete");
}