#pragma once

#include "SyncOperation.h"



class EmptyOperation : public SyncOperation
{
public:
    EmptyOperation(const FileProperties& file, const FileProperties& equalFile);
    ~EmptyOperation();

    BOOL execute() override;

    BOOL affectsFile(const FileProperties& file) const override;
    BOOL dependsOn(const SyncOperation* operation) const override;

    FileProperties getEqualFile() const;

private:
    FileProperties m_equalFile;
};

