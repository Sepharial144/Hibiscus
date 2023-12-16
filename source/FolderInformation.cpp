#include "FolderInformation.hpp"

FolderInformation::FolderInformation()
{
}

FolderInformation::~FolderInformation()
{
}

FolderInformation::FolderInformation(const std::filesystem::path path, const std::list<FileInfo>& listFiles)
    : m_rootFolder { path }
    , m_listFileInfo { listFiles }
{
}

void FolderInformation::setFolderInformation(std::list<FileInfo>& list_files)
{
    m_listFileInfo = std::move(list_files);
}

void FolderInformation::setRootFolder(const std::filesystem::path path)
{
    m_rootFolder = path;
}

const size_t FolderInformation::size() const
{
    return m_listFileInfo.size();
}

std::filesystem::path FolderInformation::getRootFolder() const
{
    return m_rootFolder;
}

const std::list<FileInfo>& FolderInformation::getFolderFileList() const
{
    return m_listFileInfo;
}