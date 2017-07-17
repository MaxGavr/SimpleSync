#pragma once

#include "SyncOperation.h"



class ReplaceOperation : public SyncOperation
{
public:
    ReplaceOperation(const FileProperties& file, const FileProperties& fileToReplace, BOOL isAmbiguous = FALSE);
    ~ReplaceOperation();

    BOOL execute() override;

    BOOL affectsFile(const FileProperties& file) const override;
    BOOL dependsOn(const SyncOperation* operation) const override;

    FileProperties getFileToReplace() const;
    BOOL isAmbiguous() const;
    
    void removeAmbiguity();

private:
    FileProperties m_fileToReplace;

    BOOL m_isAmbiguous;
};

