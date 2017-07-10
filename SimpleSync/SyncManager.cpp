#include "stdafx.h"
#include "SyncManager.h"



SyncManager::SyncManager()
{
}


SyncManager::~SyncManager()
{
}



void SyncManager::setSourceFolder(const CString& source)
{
    m_sourceFolder = source;
}

void SyncManager::setDestinationFolder(const CString& destination)
{
    m_destinationFolder = destination;
}

CString SyncManager::getSourceFolder() const
{
    return m_sourceFolder;
}

CString SyncManager::getDestinationFolder() const
{
    return m_destinationFolder;
}

void SyncManager::setSyncDirection(SYNC_DIRECTION direction)
{
    m_syncDirection = direction;
}

SyncManager::SYNC_DIRECTION SyncManager::getSyncDirection() const
{
    return m_syncDirection;
}

void SyncManager::setOptions(const SyncManagerOptions& options)
{
    m_options = options;
}

SyncManagerOptions SyncManager::getOptions() const
{
    return m_options;
}

BOOL SyncManager::isFileInSourceFolder(const FileProperties& file) const
{
    return (file.getFileFolder().Find(getSourceFolder()) == 0);
}

BOOL SyncManager::isFileInDestinationFolder(const FileProperties& file) const
{
    return (file.getFileFolder().Find(getDestinationFolder()) == 0);
}

BOOL SyncManager::isFileInFiles(const FileProperties& file, const FileSet &files) const
{
    return std::any_of(files.begin(), files.end(), [&](const FileProperties& f) {
        return file.getFileName() == f.getFileName();
    });
}

void SyncManager::scan()
{
    m_syncActions.clear();

    if (getSyncDirection() == SYNC_DIRECTION::RIGHT_TO_LEFT)
    {
        scanFolders(getDestinationFolder(), getSourceFolder());
    }
    else
    {
        scanFolders(getSourceFolder(), getDestinationFolder());
    }
}

void SyncManager::sync()
{
    for (auto& action: m_syncActions)
    {
        action->execute();
    }
}

SyncManager::OperationQueue& SyncManager::getOperations()
{
    return m_syncActions;
}

SyncManager::FileSet SyncManager::getFilesFromFolder(const CString& folder) const
{
    FileSet files;

    CFileFind fileFinder;
    bool working = fileFinder.FindFile(folder + CString("\\*.*"));

    while (working)
    {
        working = fileFinder.FindNextFile();
        
        if (!fileFinder.IsDots())
        {
            CString filePath = fileFinder.GetFilePath();
            BOOL isDirectory = fileFinder.IsDirectory();

            files.insert(FileProperties(filePath, isDirectory));
        }
    }

    fileFinder.Close();
    return files;
}

void SyncManager::scanFolders(CString source, CString destination)
{
    FileSet sourceFiles = getFilesFromFolder(source);
    FileSet destinationFiles = getFilesFromFolder(destination);

    for (auto fileIterator = sourceFiles.cbegin(); fileIterator != sourceFiles.cend(); )
    {
        const FileProperties& file = *fileIterator;

        if (!isFileInFiles(file, destinationFiles))
        {
            if (getOptions().copyMissingFiles)
                m_syncActions.push_back(new CopyOperation(file, getDestinationFolder()));
        }
        else
        {
            auto equalFileIterator = destinationFiles.find(file);
            
            if (file.isDirectory() && getOptions().recursive)
            {
                scanFolders(file.getFullPath(), equalFileIterator->getFullPath());
            }
            else
            {
                m_syncActions.push_back(new EmptyOperation(file, *equalFileIterator));
            }

            destinationFiles.erase(equalFileIterator);
        }

        fileIterator = sourceFiles.erase(fileIterator);
    }

    for (auto fileIterator = destinationFiles.cbegin(); fileIterator != destinationFiles.cend();)
    {
        const FileProperties& file = *fileIterator;

        if (!isFileInFiles(file, sourceFiles))
        {
            if (getSyncDirection() == SYNC_DIRECTION::BOTH)
            {
                if (getOptions().copyMissingFiles)
                    m_syncActions.push_back(new CopyOperation(file, source));
            }
            else
            {
                if (getOptions().deleteFiles)
                    m_syncActions.push_back(new RemoveOperation(file));
            }
        }

        fileIterator = destinationFiles.erase(fileIterator);
    }
}
