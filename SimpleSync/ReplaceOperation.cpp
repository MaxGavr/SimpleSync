#include "stdafx.h"
#include "ReplaceOperation.h"


ReplaceOperation::ReplaceOperation(const FileProperties& file, const FileProperties& fileToReplace, BOOL isAmbiguous)
    : SyncOperation(SyncOperation::TYPE::REPLACE, file),
      m_fileToReplace(fileToReplace),
      m_isAmbiguous(isAmbiguous)
{

}

ReplaceOperation::~ReplaceOperation()
{
}

BOOL ReplaceOperation::execute()
{
    if (isAmbiguous())
        return FALSE;
    else
        return ReplaceFile(getFileToReplace().getFullPath(), getFile().getFullPath(), NULL, 0, NULL, NULL);
}

BOOL ReplaceOperation::isOperationDependent(const SyncOperation* dependentOp) const
{
    return FALSE;
}

FileProperties ReplaceOperation::getFileToReplace() const
{
    return m_fileToReplace;
}

BOOL ReplaceOperation::isAmbiguous() const
{
    return m_isAmbiguous;
}
