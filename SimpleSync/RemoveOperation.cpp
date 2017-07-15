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
    if (getFile().isFolder())
        return RemoveDirectory(getFile().getFullPath());
    else
        return DeleteFile(getFile().getFullPath());
}

BOOL RemoveOperation::affectsFile(const FileProperties& file) const
{
    auto thisFile = getFile();

    if (thisFile == file)
        return TRUE;

    if (thisFile.isFolder())
        return file.isParentFolder(thisFile);
    else
        return FALSE;
}

BOOL RemoveOperation::dependsOn(const SyncOperation* operation) const
{
    return operation->affectsFile(getFile());
}
