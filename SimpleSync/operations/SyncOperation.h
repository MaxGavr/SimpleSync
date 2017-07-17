#pragma once

#include "stdafx.h"
#include "../sync/FileProperties.h"
#include <memory>

class SyncOperation
{
public:
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

    virtual BOOL execute() = 0;

    virtual BOOL affectsFile(const FileProperties& file) const = 0;
    virtual BOOL dependsOn(const SyncOperation* operation) const = 0;

    void forbid(BOOL isForbidden);
    BOOL isForbidden() const;
    
    TYPE getType() const;

    FileProperties getFile() const;
    void setFile(const FileProperties& file);

private:
    TYPE m_type;
    BOOL m_isForbidden;

    FileProperties m_file;
};
