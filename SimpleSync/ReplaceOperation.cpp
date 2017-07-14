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

// Tip: Win32 functions GetFileAttributes() and SetFileAttributes() may be required
//      to create identical copy of original file, as CopyFile() does not
//      preserve "creation" and "last access" time stamps
BOOL ReplaceOperation::execute()
{
    if (isAmbiguous())
        return FALSE;
    else
        return CopyFile(getFile().getFullPath(), getFileToReplace().getFullPath(), FALSE);
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
