#pragma once

#include "stdafx.h"
#include "sync\FileProperties.h"
#include <memory>



class SyncOperation
{
public:
    // In conjunction with private execute(), guarantees,
    // that only SyncManager can execute operations with files
    friend class SyncManager;

    // shared_ptr is used in order to utilize information about
    // operations even after SyncManager deals with them
    using ptr = std::shared_ptr <SyncOperation>;
    using const_ptr = std::shared_ptr <const SyncOperation>;

    enum class TYPE {
        COPY,
        REPLACE,
        REMOVE,
        CREATE,
        EMPTY
    };
    
    SyncOperation(TYPE type, const FileProperties& file);
    virtual ~SyncOperation();

    // File is affected if operation is executed on this file
    // or its parent folder
    virtual BOOL affectsFile(const FileProperties& file) const = 0;

    // Utilizes affectsFile()
    virtual BOOL dependsOn(const SyncOperation* operation) const = 0;

    // Forbidden operations are ignored by SyncManager
    void forbid(BOOL isForbidden);
    BOOL isForbidden() const;
    
    TYPE getType() const;

    FileProperties getFile() const;
    void setFile(const FileProperties& file);

private:
    virtual BOOL execute() = 0;

    TYPE m_type;
    BOOL m_isForbidden;

    FileProperties m_file;
};
