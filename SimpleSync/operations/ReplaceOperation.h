#pragma once

#include "SyncOperation.h"



class ReplaceOperation : public SyncOperation
{
public:
    ReplaceOperation(const FileProperties& file,
                     const FileProperties& fileToReplace,
                     BOOL isAmbiguous = FALSE);
    ~ReplaceOperation(); 

    BOOL affectsFile(const FileProperties& file) const override;
    BOOL dependsOn(const SyncOperation* operation) const override;

    FileProperties getFileToReplace() const;

    // Operation becomes ambiguous if current sync context
    // does not imply only preferable replacement
    // Ambiguous operations are NOT executed
    BOOL isAmbiguous() const;
    
    void removeAmbiguity();

private:
    BOOL execute() override;

    FileProperties m_fileToReplace;
    BOOL m_isAmbiguous;
};

