#include "stdafx.h"
#include "CopyOperation.h"



CopyOperation::CopyOperation(const FileProperties& fileToCopy, CString destinationFolder)
    : SyncOperation(SyncOperation::TYPE::COPY, fileToCopy), m_destinationFolder(destinationFolder)
{
}


CopyOperation::~CopyOperation()
{
}

BOOL CopyOperation::execute()
{
    CString destinationFile = m_destinationFolder + CString("\\") + getFile().getFileName();
    return CopyFile(getFile().getFullPath(), destinationFile, TRUE);
}

CString CopyOperation::getDestinationFolder() const
{
    return m_destinationFolder;
}
