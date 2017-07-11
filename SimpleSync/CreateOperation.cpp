#include "stdafx.h"
#include "CreateOperation.h"



CreateFolderOperation::CreateFolderOperation(const FileProperties& originalFolder, const CString& folderToCreate)
    : SyncOperation(SyncOperation::TYPE::CREATE, originalFolder),
      m_folderToCreate(FileProperties(folderToCreate, TRUE))
{
}

CreateFolderOperation::~CreateFolderOperation()
{
}

BOOL CreateFolderOperation::execute()
{
    return CreateDirectory(getFolder().getFullPath(), NULL);
}

FileProperties CreateFolderOperation::getFolder() const
{
    return m_folderToCreate;
}
