#pragma once

#include "SyncOperation.h"



class CreateFolderOperation : public SyncOperation
{
public:
    CreateFolderOperation(const FileProperties& originalFolder, const CString& folderToCreate);
    ~CreateFolderOperation();

    BOOL execute() override;

    FileProperties getFolder() const;

private:
    FileProperties m_folderToCreate;
};

