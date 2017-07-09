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

BOOL SyncManager::isFileInSourceFolder(const FileProperties& file) const
{
    return file.getFileFolder() == getSourceFolder();
}

BOOL SyncManager::isFileInDestinationFolder(const FileProperties& file) const
{
    return file.getFileFolder() == getDestinationFolder();
}

BOOL SyncManager::isFileInFiles(const FileProperties& file, const FileSet &files) const
{
    return std::any_of(files.begin(), files.end(), [&](const FileProperties& f) {
        return file.getFileName() == f.getFileName();
    });
}

SyncManager::OperationQueue SyncManager::scan()
{
    FileSet sourceFiles = getFilesFromFolder(getSourceFolder());
    FileSet destinationFiles = getFilesFromFolder(getDestinationFolder());

    m_syncActions.clear();

    for (auto fileIterator = sourceFiles.cbegin(); fileIterator != sourceFiles.cend(); )
    {
        const FileProperties& file = *fileIterator;
        SyncOperation* operation;

        if (!isFileInFiles(file, destinationFiles))
        {
            operation = new CopyOperation(file, getDestinationFolder());
        }
        else
        {
            auto equalFileIterator = destinationFiles.find(file);
            operation = new EmptyOperation(file, *equalFileIterator);

            destinationFiles.erase(equalFileIterator);
        }

        fileIterator = sourceFiles.erase(fileIterator);
        m_syncActions.push_back(operation);
    }

    for (auto fileIterator = destinationFiles.cbegin(); fileIterator != destinationFiles.cend();)
    {
        const FileProperties& file = *fileIterator;
        if (!isFileInFiles(file, sourceFiles))
        {
            SyncOperation* operation = new RemoveOperation(file);

            m_syncActions.push_back(operation);
        }
        fileIterator = destinationFiles.erase(fileIterator);
    }

    return m_syncActions;
}

void SyncManager::sync()
{
    for (auto& action: m_syncActions)
    {
        action->execute();
    }
}

SyncManager::FileSet SyncManager::getFilesFromFolder(const CString& folder) const
{
    FileSet files;

    CFileFind fileFinder;
    bool working = fileFinder.FindFile(folder + CString("\\*.*"));

    while (working)
    {
        working = fileFinder.FindNextFile();
        
        if (!fileFinder.IsDots() && !fileFinder.IsDirectory())
        {
            CString filePath = fileFinder.GetFilePath();
            files.insert(FileProperties(filePath));
        }
    }

    fileFinder.Close();
    return files;
}
