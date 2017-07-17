#include "stdafx.h"
#include "CreateOperation.h"



CreateFolderOperation::CreateFolderOperation(const FileProperties& originalFolder,
                                             const CString& folderToCreate)
    : SyncOperation(SyncOperation::TYPE::CREATE, originalFolder),
      m_folderToCreate(FileProperties(folderToCreate, TRUE))
{
}

CreateFolderOperation::~CreateFolderOperation()
{
}



BOOL CreateFolderOperation::execute()
{
    CString fullPathToFolder = getFolderToCreate().getFullPath();
    return CreateDirectory(fullPathToFolder, NULL);
}

BOOL CreateFolderOperation::affectsFile(const FileProperties& file) const
{
    FileProperties originalFolder = getFile();
    FileProperties folderToCreate = getFolderToCreate();
    
    if (originalFolder == file)
        return TRUE;

    BOOL originalFolderIsParent = file.isParentFolder(originalFolder);
    BOOL createdFolderIsParent = file.isParentFolder(folderToCreate);

    return originalFolderIsParent || createdFolderIsParent;
}

BOOL CreateFolderOperation::dependsOn(const SyncOperation* operation) const
{
    BOOL affectsOriginalFolder = operation->affectsFile(getFile());
    BOOL affectsCreatedFolder = operation->affectsFile(getFolderToCreate());

    return affectsOriginalFolder || affectsCreatedFolder;
}

FileProperties CreateFolderOperation::getFolderToCreate() const
{
    return m_folderToCreate;
}
