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

BOOL FileProperties::operator==(const FileProperties& file) const
{
    return (this->m_fileName == file.m_fileName);
}

CString FileProperties::getFileName() const
{
    int slashPosition = m_fileName.ReverseFind('\\');
    return m_fileName.Right(m_fileName.GetLength() - slashPosition - 1);
}

CString FileProperties::getFullPath() const
{
    return m_fileName;
}

CString FileProperties::getFileFolder() const
{
    return m_fileName.Left(m_fileName.ReverseFind('\\'));
}
