#pragma once

#include "SyncOperation.h"



class RemoveOperation : public SyncOperation
{
public:
    RemoveOperation(const FileProperties& fileToDelete);
    ~RemoveOperation();

    BOOL execute() override;
};
