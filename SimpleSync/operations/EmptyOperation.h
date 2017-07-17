#pragma once

#include "SyncOperation.h"



// Indicates that two files (folders) are equal within current sync context
// Does not do anything
class EmptyOperation : public SyncOperation
{
public:
    EmptyOperation(const FileProperties& file,
                   const FileProperties& equalFile);
    ~EmptyOperation();

    BOOL affectsFile(const FileProperties& file) const override;
    BOOL dependsOn(const SyncOperation* operation) const override;

    FileProperties getEqualFile() const;

private:
    BOOL execute() override;

    FileProperties m_equalFile;
};

