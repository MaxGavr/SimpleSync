#include "stdafx.h"
#include "RemoveOperation.h"



RemoveOperation::RemoveOperation(const FileProperties& fileToDelete)
    : SyncOperation(SyncOperation::TYPE::REMOVE, fileToDelete)
{
}

RemoveOperation::~RemoveOperation()
{
}



BOOL RemoveOperation::execute()
{
    CString fullFilePath = getFile().getFullPath();
    BOOL fileIsFolder = getFile().isFolder();

    if (fileIsFolder)
        return RemoveDirectory(fullFilePath);
    else
        return DeleteFile(fullFilePath);
}

BOOL RemoveOperation::affectsFile(const FileProperties& file) const
{
    FileProperties fileToRemove = getFile();

    if (fileToRemove == file)
        return TRUE;

    if (fileToRemove.isFolder())
        return file.isParentFolder(fileToRemove);
    else
        return FALSE;
}

BOOL RemoveOperation::dependsOn(const SyncOperation* operation) const
{
    FileProperties fileToRemove = getFile();
    return operation->affectsFile(fileToRemove);
}
