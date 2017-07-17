#pragma once

#include "SyncOperation.h"



class RemoveOperation : public SyncOperation
{
public:
    RemoveOperation(const FileProperties& fileToDelete);
    ~RemoveOperation();

    BOOL affectsFile(const FileProperties& file) const override;
    BOOL dependsOn(const SyncOperation* operation) const override;

private:
    // Both files and folders can be removed
    // Only empty folders will be removed successfully
    BOOL execute() override;
};

