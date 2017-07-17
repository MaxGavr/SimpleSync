#pragma once

#include "SyncOperation.h"



class CreateFolderOperation : public SyncOperation
{
public:
    CreateFolderOperation(const FileProperties& originalFolder,
                          const CString& folderToCreate);
    ~CreateFolderOperation();

    BOOL affectsFile(const FileProperties& file) const override;
    BOOL dependsOn(const SyncOperation* operation) const override;

    FileProperties getFolderToCreate() const;

private:
    BOOL execute() override;

    FileProperties m_folderToCreate;
};

