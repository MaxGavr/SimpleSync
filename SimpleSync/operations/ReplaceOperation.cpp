#include "stdafx.h"
#include "ReplaceOperation.h"



ReplaceOperation::ReplaceOperation(const FileProperties& file,
                                   const FileProperties& fileToReplace,
                                   BOOL isAmbiguous)
    : SyncOperation(SyncOperation::TYPE::REPLACE, file),
      m_fileToReplace(fileToReplace),
      m_isAmbiguous(isAmbiguous)
{
}

ReplaceOperation::~ReplaceOperation()
{
}



// Tip: Win32 functions GetFileAttributes() and SetFileAttributes()
//      may be required to create identical copy of original file,
//      as CopyFile() does not preserve "creation" and "last access" time stamps
BOOL ReplaceOperation::execute()
{
    if (isAmbiguous())
        return FALSE;
    else
    {
        CString orignalFile = getFile().getFullPath();
        CString fileToReplace = getFileToReplace().getFullPath();
        return CopyFile(orignalFile, fileToReplace, FALSE);
    }
        
}

BOOL ReplaceOperation::affectsFile(const FileProperties& file) const
{
    BOOL equalToOriginalFile = file == getFile();
    BOOL equalToReplacedFile = file == getFileToReplace();

    return equalToOriginalFile || equalToReplacedFile;
}

BOOL ReplaceOperation::dependsOn(const SyncOperation* operation) const
{
    BOOL affectsOriginalFile = operation->affectsFile(getFile());
    BOOL affectsReplacedFile = operation->affectsFile(getFileToReplace());

    return  affectsOriginalFile || affectsReplacedFile;
}

FileProperties ReplaceOperation::getFileToReplace() const
{
    return m_fileToReplace;
}

BOOL ReplaceOperation::isAmbiguous() const
{
    return m_isAmbiguous;
}

void ReplaceOperation::removeAmbiguity()
{
    m_isAmbiguous = FALSE;
}
