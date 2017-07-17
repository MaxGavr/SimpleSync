#pragma once

#include "SyncOperation.h"



class CopyOperation : public SyncOperation
{
public:
    CopyOperation(const FileProperties& fileToCopy,
                  CString destinationFolder);
    ~CopyOperation();

    BOOL affectsFile(const FileProperties& file) const override;
    BOOL dependsOn(const SyncOperation* operation) const override;

    CString getDestinationFolder() const;

private:
    BOOL execute() override;

    CString m_destinationFolder;
};

