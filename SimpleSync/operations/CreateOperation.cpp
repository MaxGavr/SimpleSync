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

BOOL CreateFolderOperation::affectsFile(const FileProperties& file) const
{
    auto originalFolder = getFile();
    auto folderToCreate = getFolder();
    
    if (originalFolder == file)
        return TRUE;

    return file.isParentFolder(originalFolder) ||
           file.isParentFolder(folderToCreate);
}

BOOL CreateFolderOperation::dependsOn(const SyncOperation* operation) const
{
    return operation->affectsFile(getFile()) ||
           operation->affectsFile(getFolder());
}

FileProperties CreateFolderOperation::getFolder() const
{
    return m_folderToCreate;
}
