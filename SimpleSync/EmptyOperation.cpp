#include "stdafx.h"
#include "EmptyOperation.h"


EmptyOperation::EmptyOperation(const FileProperties& file, const FileProperties& equalFile)
    : SyncOperation(SyncOperation::TYPE::EMPTY, file), m_equalFile(equalFile)
{
}


EmptyOperation::~EmptyOperation()
{
}

BOOL EmptyOperation::execute()
{
    return TRUE;
}

BOOL EmptyOperation::isOperationDependent(const SyncOperation* dependentOp) const
{
    const auto& file = dependentOp->getFile();
    if (getFile().isFolder())
        return file.isParentFolder(getFile()) || file.isParentFolder(getEqualFile());
    else
        return FALSE;
}

FileProperties EmptyOperation::getEqualFile() const
{
    return m_equalFile;
}
