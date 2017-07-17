#include "stdafx.h"
#include "CopyOperation.h"



CopyOperation::CopyOperation(const FileProperties& fileToCopy,
                             CString destinationFolder)
    : SyncOperation(SyncOperation::TYPE::COPY, fileToCopy),
      m_destinationFolder(destinationFolder)
{
}

CopyOperation::~CopyOperation()
{
}



// Tip: Win32 functions GetFileAttributes() and SetFileAttributes() may be required
//      to create identical copy of original file, as CopyFile() does not
//      preserve "creation" and "last access" time stamps
BOOL CopyOperation::execute()
{
    CString originalFileName = getFile().getFileName();
    CString slash("\\");

    CString newFilePath = getDestinationFolder() + slash + originalFileName;
    return CopyFile(getFile().getFullPath(), newFilePath, TRUE);
}

BOOL CopyOperation::affectsFile(const FileProperties& file) const
{
    return file == getFile();
}

BOOL CopyOperation::dependsOn(const SyncOperation * operation) const
{
    BOOL affectsFileToCopy = operation->affectsFile(getFile());
    return affectsFileToCopy;
}

CString CopyOperation::getDestinationFolder() const
{
    return m_destinationFolder;
}
