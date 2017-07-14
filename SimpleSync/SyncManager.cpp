#include "stdafx.h"
#include "SyncManager.h"



SyncManager::SyncManager()
    : m_syncDirection(SYNC_DIRECTION::LEFT_TO_RIGHT)
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

void SyncManager::setComparisonParameters(const FileComparisonParameters& params)
{
    m_compareParameters = params;
}

FileComparisonParameters SyncManager::getComparisonParameters() const
{
    return m_compareParameters;
}

BOOL SyncManager::isFileInSourceFolder(const FileProperties& file) const
{
    return (file.getParentFolder().Find(getSourceFolder()) == 0);
}

BOOL SyncManager::isFileInDestinationFolder(const FileProperties& file) const
{
    return (file.getParentFolder().Find(getDestinationFolder()) == 0);
}

CString SyncManager::getFileRelativePath(const FileProperties& file, BOOL withName) const
{
    CString source = file.getRelativePath(getSourceFolder(), withName);
    CString dest = file.getRelativePath(getDestinationFolder(), withName);
    return source.IsEmpty() ? dest : source;
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
            CFileStatus fileProperties;
            CFile::GetStatus(fileFinder.GetFilePath(), fileProperties);

            FileProperties file(fileProperties);

            files.insert(file);
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
            manageCopyOperation(file, destination);
        else
        {
            // equal file ?? same name file
            auto equalFileIterator = destinationFiles.find(file);
            
            if (file.isFolder())
            {
                if (getOptions().recursive)
                {
                    m_syncActions.push_back(new EmptyOperation(file, *equalFileIterator));
                    scanFolders(file.getFullPath(), equalFileIterator->getFullPath());
                }
            }
            else
                manageReplaceOperation(file, *equalFileIterator);

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
                manageCopyOperation(file, source);
            else
                manageRemoveOperation(file);
        }

        fileIterator = destinationFiles.erase(fileIterator);
    }
}

void SyncManager::manageCopyOperation(const FileProperties& fileToCopy, CString destinationFolder)
{
    if (fileToCopy.isFolder())
    {
        CString folderToCreate = destinationFolder + "\\" + fileToCopy.getFileName();
        m_syncActions.push_back(new CreateFolderOperation(fileToCopy, folderToCreate));
        
        FileSet files = getFilesFromFolder(fileToCopy.getFullPath());

        for (const auto& file : files)
        {
            manageCopyOperation(file, folderToCreate);
        }
    }
    else
    {
        if (getOptions().copyMissingFiles)
            m_syncActions.push_back(new CopyOperation(fileToCopy, destinationFolder));
    }
}

void SyncManager::manageReplaceOperation(const FileProperties& originalFile, const FileProperties& fileToReplace)
{
    using RESULT = FileProperties::COMPARISON_RESULT;

    RESULT compareResult = originalFile.compareTo(fileToReplace, getComparisonParameters());
    switch (compareResult)
    {
    case RESULT::PREFERABLE:
        m_syncActions.push_back(new ReplaceOperation(originalFile, fileToReplace, FALSE));
        break;
    case RESULT::NON_PREFERABLE:
        if (getSyncDirection() == SYNC_DIRECTION::BOTH)
            m_syncActions.push_back(new ReplaceOperation(fileToReplace, originalFile, FALSE));
        else
            m_syncActions.push_back(new ReplaceOperation(originalFile, fileToReplace, TRUE));
        break;
    case RESULT::UNDEFINED:
        m_syncActions.push_back(new ReplaceOperation(originalFile, fileToReplace, TRUE));
        break;
    case RESULT::EQUAL:
        m_syncActions.push_back(new EmptyOperation(originalFile, fileToReplace));
        break;
    }
}

void SyncManager::manageRemoveOperation(const FileProperties& fileToRemove)
{
    if (fileToRemove.isFolder())
    {
        FileSet files = getFilesFromFolder(fileToRemove.getFullPath());

        for (const auto& file : files)
            manageRemoveOperation(file);
    }

    if (getOptions().deleteFiles)
        m_syncActions.push_back(new RemoveOperation(fileToRemove));
}
