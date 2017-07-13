#pragma once

#include "SyncOperation.h"



class EmptyOperation : public SyncOperation
{
public:
    EmptyOperation(const FileProperties& file, const FileProperties& equalFile);
    ~EmptyOperation();

    BOOL execute() override;
    BOOL isOperationDependent(const SyncOperation* dependentOp) const override;

    FileProperties getEqualFile() const;

private:
    FileProperties m_equalFile;
};

