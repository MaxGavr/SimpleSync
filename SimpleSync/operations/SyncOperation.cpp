#include "stdafx.h"
#include "SyncOperation.h"



SyncOperation::SyncOperation(TYPE type, const FileProperties& file)
    : m_type(type), m_file(file)
{
    m_isForbidden = FALSE;
}

SyncOperation::~SyncOperation()
{
}



void SyncOperation::forbid(BOOL isForbidden)
{
    m_isForbidden = isForbidden;
}

BOOL SyncOperation::isForbidden() const
{
    return m_isForbidden;
}

SyncOperation::TYPE SyncOperation::getType() const
{
    return m_type;
}

FileProperties SyncOperation::getFile() const
{
    return m_file;
}

void SyncOperation::setFile(const FileProperties& file)
{
    m_file = file;
}
