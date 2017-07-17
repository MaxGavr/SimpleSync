#pragma once

#include "SyncOperation.h"



class RemoveOperation : public SyncOperation
{
public:
    RemoveOperation(const FileProperties& fileToDelete);
    ~RemoveOperation();

    BOOL execute() override;

    BOOL affectsFile(const FileProperties& file) const override;
    BOOL dependsOn(const SyncOperation* operation) const override;
};

