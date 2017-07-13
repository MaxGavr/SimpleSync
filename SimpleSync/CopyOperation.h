#pragma once

#include "SyncOperation.h"



class CopyOperation : public SyncOperation
{
public:
    CopyOperation(const FileProperties& fileToCopy, CString destinationFolder);
    ~CopyOperation();

    BOOL execute() override;
    BOOL isOperationDependent(const SyncOperation* dependentOp) const override;

    CString getDestinationFolder() const;

private:
    CString m_destinationFolder;
};

