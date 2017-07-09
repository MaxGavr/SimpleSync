#include "stdafx.h"
#include "FileProperties.h"


FileProperties::FileProperties(const CString& fileName, BOOL isDir)
    : m_fileName(fileName), m_isDirectory(isDir)
{
}


FileProperties::~FileProperties()
{
}

BOOL FileProperties::operator<(const FileProperties& file) const
{
    return (this->getFileName() < file.getFileName());
}

BOOL FileProperties::operator==(const FileProperties& file) const
{
    return (this->getFileName() == file.getFileName());
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

BOOL FileProperties::isDirectory() const
{
    return m_isDirectory;
}
