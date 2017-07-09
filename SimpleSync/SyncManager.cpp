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

    for (const auto& file : sourceFiles)
    {
        SyncOperation* operation;
        if (!isFileInFiles(file, destinationFiles))
        {
            operation = new CopyOperation(file, getDestinationFolder());
        }
        else
        {
            auto it = std::find_if(destinationFiles.begin(), destinationFiles.end(), [&](const FileProperties& f) {
                return f.getFileName() == file.getFileName();
            });
            operation = new EmptyOperation(file, *it);
        }
        m_syncActions.push_back(operation);
    }

    for (const auto& file : destinationFiles)
    {
        if (isFileInFiles(file, sourceFiles))
        {
            SyncOperation* operation = new RemoveOperation(file);
            m_syncActions.push_back(operation);
        }
    }

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
        
        if (!fileFinder.IsDots() && !fileFinder.IsDirectory())
        {
            CString filePath = fileFinder.GetFilePath();
            files.insert(FileProperties(filePath));
        }
    }

    fileFinder.Close();
    return files;
}
