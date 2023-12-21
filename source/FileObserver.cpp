#include "FileObserver.hpp"
#include "Algorithm.hpp"
#include "FolderInformation.hpp"
#include "logger/logger.hpp"

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
        std::list<FileInfo> listFiles(count);

        logger::debug("File observer: inspecting {}", projectEntry.path().c_str());
        for (const auto& projectFile : filesystem_iteraror(projectEntry.path())) {
            int64_t filter = fileFilter(projectFile.path().c_str());

            if (filter) {
                auto ftime = std::filesystem::last_write_time(projectFile);
                logger::debug(" -- {} {} {}",
                    projectFile.path().c_str(),
                    ftime,
                    " valid");
                listFiles.emplace_back(FileInfo { projectFile, ftime });
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
    if (size == count)
    {
        logger::info("File observer observe directory count of folders ... equal");
    }
    if (size > count)
    {
        logger::info("File observer observe directory count of folders ... less");

    }
    if (size < count)
    {
        logger::info("File observer observe directory count of folders ... greater");
    }
    /*
    auto isFoldersListsDifferent = (DirectoryStorage::size() != DirectoryStorage::countFilesInFolder(m_configPath)) ? true : false;
    if (isFoldersListsDifferent) {
        logger::info("File observer observe directory count of folders ... different");
        // todo differenciation of folders
        return;
    }

    logger::info("File observer observe directory count of folders ... same");

    for (const auto& projectEntry : filesystem_iteraror(DirectoryStorage::configPath())) {

        // list of projects is empty, need initialization
        size_t countFiles = DirectoryStorage::countFilesInFolder(projectEntry);
        std::list<FileInfo> listFiles(countFiles);

        for (const auto& projectFile : filesystem_iteraror(projectEntry.path())) {
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
    */
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