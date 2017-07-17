#include "stdafx.h"
#include "CopyOperation.h"



CopyOperation::CopyOperation(const FileProperties& fileToCopy, CString destinationFolder)
    : SyncOperation(SyncOperation::TYPE::COPY, fileToCopy), m_destinationFolder(destinationFolder)
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
    CString destinationFile = m_destinationFolder + CString("\\") + getFile().getFileName();
    return CopyFile(getFile().getFullPath(), destinationFile, TRUE);
}

BOOL CopyOperation::affectsFile(const FileProperties& file) const
{
    return file == getFile();
}

BOOL CopyOperation::dependsOn(const SyncOperation * operation) const
{
    return operation->affectsFile(getFile());
}

CString CopyOperation::getDestinationFolder() const
{
    return m_destinationFolder;
}
