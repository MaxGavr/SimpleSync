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
    return DeleteFile(getFile().getFullPath());
}
