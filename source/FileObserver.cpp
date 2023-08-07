#include "FileObserver.hpp"

#include <chrono>
#include <iostream>

FileObserver::FileObserver() {}
FileObserver::FileObserver(std::filesystem::path config_path, std::function<bool(std::string&)> filter)
    : m_configPath{ config_path }
    , m_filter{ filter }
{
}


FileObserver::~FileObserver() 
{
    if(m_mainLoop.joinable())
        m_mainLoop.join();
}

/*
void FileObserver::getListFiles(std::vector<std::string>& res_container)
{
    const std::lock_guard<std::mutex> lock(m_mtx);
    res_container.reserve(m_fileList.size());
    std::copy(m_fileList.begin(), m_fileList.end(), std::back_inserter(res_container));
}
*/


void FileObserver::start() 
{
    m_mainLoop = std::thread(&FileObserver::observeDirectory, this);
}


void FileObserver::observeDirectory()
{
    std::vector<FileChunk> rawFilesList(100);
    isRunning = true;
    while(isRunning)
    {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        m_mtx.lock();
        rawFilesList.clear();
        for (const auto& entry : std::filesystem::directory_iterator(m_configPath))
        {
            std::string file{ entry.path() };

            if(fileFilter(file))
                rawFilesList.push_back({file, std::filesystem::last_write_time(entry.path())});
        }

        // TODO: make algorothm filter better
        m_fileList.clear();

        // TODO: create filter by timestamp
        m_mtx.unlock();
    }
}

 bool FileObserver::fileFilter(const std::string& line)
 {
    const size_t position = line.rfind('/');
    const std::string fileName = line.substr(position + 1, line.size());
        
    if (fileName.rfind(".yaml") == std::string::npos)
        return false;
    const char letter = fileName[0];
    if (letter == '#' || letter == '!')
        return false;   
    return true; 
 }