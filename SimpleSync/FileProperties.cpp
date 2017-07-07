#include "stdafx.h"
#include "FileProperties.h"


FileProperties::FileProperties(const CString& fileName)
    : m_fileName(fileName)
{
}


FileProperties::~FileProperties()
{
}

BOOL FileProperties::operator<(const FileProperties& file) const
{
    return (this->m_fileName < file.m_fileName);
}

CString FileProperties::getFileName() const
{
    return m_fileName;
}
