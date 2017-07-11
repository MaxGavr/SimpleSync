#pragma once

#include "stdafx.h"
#include "FileProperties.h"

class SyncOperation
{
public:
    enum class TYPE {
        COPY,
        REPLACE,
        REMOVE,
        EMPTY
    };
    
    SyncOperation(TYPE type, const FileProperties& file);
    virtual ~SyncOperation();

    virtual BOOL execute() = 0;
    
    TYPE getType() const;

    FileProperties getFile() const;
    void setFile(const FileProperties& file);

private:
    TYPE m_type;

    FileProperties m_file;
};
