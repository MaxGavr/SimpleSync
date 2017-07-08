#include "stdafx.h"
#include "SyncOperation.h"



SyncOperation::SyncOperation(TYPE type, const FileProperties& file)
    : m_type(type), m_file(file)
{
}


SyncOperation::~SyncOperation()
{
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
