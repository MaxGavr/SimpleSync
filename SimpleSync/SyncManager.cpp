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




SyncManager::OperationQueue SyncManager::scan()
{
    FileSet sourceFiles = getFilesFromFolder(getSourceFolder());
    FileSet destinationFiles = getFilesFromFolder(getDestinationFolder());

    m_syncActions.clear();

    for (const auto& file : sourceFiles)
    {
        auto it = destinationFiles.find(file);
        if (it == destinationFiles.end())
        {
            SyncOperation* operation = new CopyOperation(file, getDestinationFolder());
            m_syncActions.push_back(operation);
        }
    }

    for (const auto& file : destinationFiles)
    {
        auto it = sourceFiles.find(file);
        if (it == sourceFiles.end())
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
