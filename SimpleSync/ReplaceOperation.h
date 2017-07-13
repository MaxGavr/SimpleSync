#pragma once

#include "SyncOperation.h"



class ReplaceOperation : public SyncOperation
{
public:
    ReplaceOperation(const FileProperties& file, const FileProperties& fileToReplace, BOOL isAmbiguous = FALSE);
    ~ReplaceOperation();

    BOOL execute() override;
    BOOL isOperationDependent(const SyncOperation* dependentOp) const override;

    FileProperties getFileToReplace() const;
    BOOL isAmbiguous() const;

private:
    FileProperties m_fileToReplace;

    BOOL m_isAmbiguous;
};

