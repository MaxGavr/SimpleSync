#include "stdafx.h"
#include "FileProperties.h"
#include <algorithm>




FileProperties::FileProperties(const CFileStatus& properties)
    : m_properties(properties)
{
}

FileProperties::FileProperties(const CString& fileName, BOOL isFolder)
{
    _tcscpy(m_properties.m_szFullName, fileName);
    if (isFolder)
        m_properties.m_attribute |= CFile::Attribute::directory;
}

FileProperties::~FileProperties()
{
}


FileProperties::COMPARISON_RESULT FileProperties::compareTo(const FileProperties& file,
    const FileComparisonParameters& params) const
{
    if (this->getFileName() != file.getFileName())
        return COMPARISON_RESULT::UNDEFINED;

    ComparisonResults results;

    if (params.compareSize)
    {
        COMPARISON_RESULT sizeCompare = compareProperty(this->getSize(), file.getSize());
        results.push_back(sizeCompare);
    }

    if (params.compareTime)
    {
        COMPARISON_RESULT timeCompare;

        switch (params.timeToCompare)
        {
        case TIMES::CREATION_TIME:
            timeCompare = compareProperty(this->getCreationTime(), file.getCreationTime());
            break;
        case TIMES::LAST_ACCESS_TIME:
            timeCompare = compareProperty(this->getLastAccessTime(), file.getLastAccessTime());
            break;
        case TIMES::LAST_WRITE_TIME:
            timeCompare = compareProperty(this->getLastWriteTime(), file.getLastWriteTime());
            break;
        }

        results.push_back(timeCompare);
    }

    return makeChoice(results);

}

FileProperties::COMPARISON_RESULT FileProperties::makeChoice(ComparisonResults& results) const
{
    results.remove(COMPARISON_RESULT::EQUAL);

    if (results.empty())
        return COMPARISON_RESULT::EQUAL;

    BOOL hasPreferable = std::find(results.begin(), results.end(), COMPARISON_RESULT::PREFERABLE)!= results.end();
    BOOL hasNonPreferable = std::find(results.begin(), results.end(), COMPARISON_RESULT::NON_PREFERABLE) != results.end();

    if (hasPreferable)
    {
        if (hasNonPreferable)
            return COMPARISON_RESULT::UNDEFINED;
        else
            return COMPARISON_RESULT::PREFERABLE;
    }
    else
        return COMPARISON_RESULT::NON_PREFERABLE;
}

FileProperties FileProperties::operator=(const FileProperties& file)
{
    _tcscpy(m_properties.m_szFullName, file.m_properties.m_szFullName);
    
    m_properties.m_size = file.m_properties.m_size;

    m_properties.m_ctime = file.m_properties.m_ctime;
    m_properties.m_mtime = file.m_properties.m_mtime;
    m_properties.m_atime = file.m_properties.m_atime;

    m_properties.m_attribute = file.m_properties.m_attribute;

    return *this;
}

BOOL FileProperties::operator<(const FileProperties& file) const
{
    bool isFolder1 = this->isFolder();
    bool isFolder2 = file.isFolder();

    if (isFolder1 != isFolder2)
        return isFolder1 ? FALSE : TRUE;
    else
        return (this->getFileName() < file.getFileName());
}

// TODO: test
BOOL FileProperties::operator==(const FileProperties& file) const
{
    BOOL name = _tcscmp(m_properties.m_szFullName,
                        file.m_properties.m_szFullName);
    BOOL size = m_properties.m_size == file.m_properties.m_size;
    BOOL times = (m_properties.m_ctime == file.m_properties.m_ctime) &&
                 (m_properties.m_mtime == file.m_properties.m_mtime) &&
                 (m_properties.m_atime == file.m_properties.m_atime);
    BOOL attribs = m_properties.m_attribute == file.m_properties.m_attribute;

    return name && size && times && attribs;
}


CString FileProperties::getFileName() const
{
    int slashPosition = getFullPath().ReverseFind('\\');
    return getFullPath().Right(getFullPath().GetLength() - slashPosition - 1);
}

CString FileProperties::getFullPath() const
{
    return m_properties.m_szFullName;
}

CString FileProperties::getParentFolder() const
{
    return getFullPath().Left(getFullPath().ReverseFind('\\'));
}

// won't work with foo/bar and foo
CString FileProperties::getRelativePath(const CString& rootFolder, BOOL withName) const
{
    int rootPosition = getFullPath().Find(rootFolder);
    if (rootPosition != 0)
        return CString();

    CString relativePathWithName = getFullPath().Right(getFullPath().GetLength() - rootFolder.GetLength() - 1);

    if (withName)
        return relativePathWithName;

    int slashPosition = relativePathWithName.ReverseFind('\\');
    if (slashPosition != -1)
        return relativePathWithName.Left(slashPosition);
    else
        return CString();
}

ULONGLONG FileProperties::getSize() const
{
    return m_properties.m_size;
}

CTime FileProperties::getCreationTime() const
{
    return m_properties.m_ctime;
}

CTime FileProperties::getLastAccessTime() const
{
    return m_properties.m_atime;
}

CTime FileProperties::getLastWriteTime() const
{
    return m_properties.m_mtime;
}

BOOL FileProperties::isFolder() const
{
    return (m_properties.m_attribute & CFile::Attribute::directory) ==
        CFile::Attribute::directory;
}

BOOL FileProperties::isParentFolder(const FileProperties& folder) const
{
    return folder.isFolder() && (folder.getFullPath() == getParentFolder());
}

BOOL FileProperties::isArchived() const
{
    return (m_properties.m_attribute & CFile::Attribute::archive) ==
        CFile::Attribute::archive;
}

BOOL FileProperties::isSystem() const
{
    return (m_properties.m_attribute & CFile::Attribute::system) ==
        CFile::Attribute::system;
}

BOOL FileProperties::isHidden() const
{
    return (m_properties.m_attribute & CFile::Attribute::hidden) ==
        CFile::Attribute::hidden;
}

BOOL FileProperties::isReadOnly() const
{
    return (m_properties.m_attribute & CFile::Attribute::readOnly) ==
        CFile::Attribute::readOnly;
}
