#include "stdafx.h"
#include "SyncManager.h"



SyncManager::SyncManager()
    : m_syncDirection(SYNC_DIRECTION::LEFT_TO_RIGHT),
      m_sourceFolder(_T("")),
      m_destinationFolder(_T(""))
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

CString SyncManager::getFileRelativePath(const FileProperties& file,
                                         BOOL withName) const
{
    CString source = file.getRelativePath(getSourceFolder(), withName);
    CString dest = file.getRelativePath(getDestinationFolder(), withName);
    return source.IsEmpty() ? dest : source;
}

BOOL SyncManager::isFileInFiles(const FileProperties& file,
                                const FileSet &files) const
{
    return files.find(file) != files.end();
}

BOOL SyncManager::scan()
{
    for (auto it = m_syncActions.begin(); it != m_syncActions.end(); ++it)
        delete (*it);
    m_syncActions.clear();

    if (!folderExists(getSourceFolder()) ||
        !folderExists(getDestinationFolder()))
        return FALSE;

    if (getSyncDirection() == SYNC_DIRECTION::RIGHT_TO_LEFT)
        scanFolders(getDestinationFolder(), getSourceFolder());
    else
        scanFolders(getSourceFolder(), getDestinationFolder());

    return TRUE;
}

void SyncManager::sync(SyncCallback* callback)
{
    for (auto& operation: m_syncActions)
    {
        if (!operation->isForbidden())
        {
            (*callback)(operation);
            operation->execute();
        }
    }
}

SyncManager::OperationQueue SyncManager::getOperations()
{
    return m_syncActions;
}

BOOL SyncManager::folderExists(const CString& folder) const
{
    DWORD attributes = GetFileAttributes(folder);
    if (attributes == INVALID_FILE_ATTRIBUTES)
        return FALSE;
    return (attributes & FILE_ATTRIBUTE_DIRECTORY);
}

BOOL SyncManager::fileMeetsRequirements(const FileProperties& file) const
{
    const auto& options = getOptions();

    if (!options.syncHiddenFiles && file.isHidden())
        return FALSE;

    if (!options.recursive && file.isFolder())
        return FALSE;

    return TRUE;
}

SyncManager::FileSet SyncManager::getFilesFromFolder(const CString& folder) const
{
    FileSet files;

    CFileFind fileFinder;
    BOOL hasFiles = fileFinder.FindFile(folder + CString("\\*.*"));

    while (hasFiles)
    {
        hasFiles = fileFinder.FindNextFile();
        
        if (!fileFinder.IsDots())
        {
            CFileStatus fileProperties;
            CFile::GetStatus(fileFinder.GetFilePath(), fileProperties);

            FileProperties file(fileProperties);

            if (fileMeetsRequirements(file))
                files.insert(file);
        }
    }

    fileFinder.Close();
    return files;
}

void SyncManager::scanFolders(const CString& source,
                              const CString& destination)
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
            // TODO: come up with suitable name for iterator
            auto sameFileIterator = destinationFiles.find(file);
            
            if (file.isFolder())
            {
                enqueueOperation(new EmptyOperation(file, *sameFileIterator));
                scanFolders(file.getFullPath(), sameFileIterator->getFullPath());
            }
            else
                manageReplaceOperation(file, *sameFileIterator);

            destinationFiles.erase(sameFileIterator);
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

void SyncManager::enqueueOperation(SyncOperation* operation)
{
    if (operation)
        m_syncActions.push_back(operation);
}

void SyncManager::manageCopyOperation(const FileProperties& fileToCopy,
                                      const CString& destinationFolder)
{
    if (fileToCopy.isFolder())
    {
        BOOL isEmpty = PathIsDirectoryEmpty(fileToCopy.getFullPath());
        if (isEmpty && !getOptions().createEmptyFolders)
            return;

        CString folderToCreate = destinationFolder + "\\" + fileToCopy.getFileName();
        enqueueOperation(new CreateFolderOperation(fileToCopy, folderToCreate));

        FileSet files = getFilesFromFolder(fileToCopy.getFullPath());

        for (const auto& file : files)
            manageCopyOperation(file, folderToCreate);
    }
    else
    {
        if (getOptions().copyMissingFiles)
            enqueueOperation(new CopyOperation(fileToCopy, destinationFolder));
    }
}

void SyncManager::manageReplaceOperation(const FileProperties& originalFile,
                                         const FileProperties& fileToReplace)
{
    using RESULT = FileProperties::COMPARISON_RESULT;

    RESULT compareResult = originalFile.compareTo(fileToReplace, getComparisonParameters());
    SyncOperation* op;

    switch (compareResult)
    {
    case RESULT::PREFERABLE:
        op = new ReplaceOperation(originalFile, fileToReplace, FALSE);
        break;
    case RESULT::NON_PREFERABLE:
        if (getSyncDirection() == SYNC_DIRECTION::BOTH)
            op = new ReplaceOperation(fileToReplace, originalFile, FALSE);
        else
            op = new ReplaceOperation(originalFile, fileToReplace, TRUE);
        break;
    case RESULT::UNDEFINED:
        op = new ReplaceOperation(originalFile, fileToReplace, TRUE);
        break;
    case RESULT::EQUAL:
        op = new EmptyOperation(originalFile, fileToReplace);
        break;
    }
    enqueueOperation(op);
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
        enqueueOperation(new RemoveOperation(fileToRemove));
}
