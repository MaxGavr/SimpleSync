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

SyncManager::FileSet SyncManager::scan()
{
    FileSet sourceFiles = getFilesFromFolder(getSourceFolder());
    FileSet destinationFiles = getFilesFromFolder(getDestinationFolder());

    return FileSet();
}

SyncManager::FileSet SyncManager::getFilesFromFolder(const CString& folder) const
{
    FileSet files;

    CFileFind fileFinder;
    bool working = fileFinder.FindFile(folder + CString("\\*.*"));

    while (working)
    {
        working = fileFinder.FindNextFile();
        CString fileUrl = fileFinder.GetFileURL();
        
        files.insert(FileProperties(fileUrl));
    }

    fileFinder.Close();
    return files;
}

SyncAction::SyncAction(TYPE type, FileProperties sourceFile, FileProperties destinationFile)
    : m_type(type), m_sourceFile(sourceFile), m_destinationFile(destinationFile)
{
}
