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

SyncManager::ActionQueue SyncManager::scan()
{
    FileSet sourceFiles = getFilesFromFolder(getSourceFolder());
    FileSet destinationFiles = getFilesFromFolder(getDestinationFolder());

    ActionQueue syncActions;

    for (const auto& file : sourceFiles)
    {
        auto it = destinationFiles.find(file);
        if (it == destinationFiles.end())
        {
            syncActions.push(SyncAction(SyncAction::TYPE::COPY, file));
        }
    }

    for (const auto& file : destinationFiles)
    {
        auto it = sourceFiles.find(file);
        if (it == sourceFiles.end())
        {
            syncActions.push(SyncAction(SyncAction::TYPE::REMOVE, file));
        }
    }

    return syncActions;
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

SyncAction::SyncAction(TYPE type, FileProperties file)
    : m_type(type), m_file(file)//, m_destinationFile(destinationFile)
{
}
