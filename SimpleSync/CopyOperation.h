#pragma once

#include "SyncOperation.h"



class CopyOperation : public SyncOperation
{
public:
    CopyOperation(const FileProperties& fileToCopy, CString destinationFolder);
    ~CopyOperation();

    BOOL execute() override;

    CString getDestinationFolder() const;

private:
    CString m_destinationFolder;
};

