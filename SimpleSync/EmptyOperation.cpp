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

FileProperties EmptyOperation::getEqualFile() const
{
    return m_equalFile;
}
