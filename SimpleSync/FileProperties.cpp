#include "stdafx.h"
#include "FileProperties.h"
#include <algorithm>



FileProperties::FileProperties(const CFileStatus& properties)
    : m_properties(properties)
{
}

FileProperties::FileProperties(const CString& fileName, BOOL isFolder)
{
    wcscpy_s(m_properties.m_szFullName, fileName);
    if (isFolder)
        m_properties.m_attribute |= CFile::Attribute::directory;
}

FileProperties::~FileProperties()
{
}


FileProperties::COMPARISON_RESULT FileProperties::compareTo(const FileProperties& file,
                                                            const FileComparisonParameters& params) const
{
    BOOL equalNames = this->getFileName() == file.getFileName();
    if (!equalNames)
        return COMPARISON_RESULT::UNDEFINED;

    ComparisonResults results;

    if (params.m_compareSize)
    {
        COMPARISON_RESULT sizeCompare = compareProperty(this->getSize(),
                                                        file.getSize());
        results.push_back(sizeCompare);
    }

    if (params.m_compareTime)
    {
        COMPARISON_RESULT timeCompare;

        switch (params.m_timeToCompare)
        {
        case TIME_STAMP::CREATION_TIME:
            timeCompare = compareProperty(this->getCreationTime(),
                                          file.getCreationTime());
            break;
        case TIME_STAMP::LAST_ACCESS_TIME:
            timeCompare = compareProperty(this->getLastAccessTime(),
                                          file.getLastAccessTime());
            break;
        case TIME_STAMP::LAST_WRITE_TIME:
            timeCompare = compareProperty(this->getLastWriteTime(),
                                          file.getLastWriteTime());
            break;
        }

        results.push_back(timeCompare);
    }

    return makeChoice(results);
}

FileProperties::COMPARISON_RESULT FileProperties::makeChoice(ComparisonResults& results) const
{
    using RESULT = COMPARISON_RESULT;

    results.remove(RESULT::EQUAL);

    if (results.empty())
        return RESULT::EQUAL;

    auto it = std::find(results.begin(), results.end(), RESULT::PREFERABLE);
    BOOL hasPreferable = it != results.end();

    it = std::find(results.begin(), results.end(), RESULT::NON_PREFERABLE);
    BOOL hasNonPreferable = it != results.end();

    if (hasPreferable)
    {
        if (hasNonPreferable)
            return RESULT::UNDEFINED;
        else
            return RESULT::PREFERABLE;
    }
    else
        return RESULT::NON_PREFERABLE;
}

FileProperties FileProperties::operator=(const FileProperties& file)
{
    wcscpy_s(m_properties.m_szFullName, file.m_properties.m_szFullName);
    
    m_properties.m_size = file.m_properties.m_size;

    m_properties.m_ctime = file.m_properties.m_ctime;
    m_properties.m_mtime = file.m_properties.m_mtime;
    m_properties.m_atime = file.m_properties.m_atime;

    m_properties.m_attribute = file.m_properties.m_attribute;

    return *this;
}

BOOL FileProperties::operator<(const FileProperties& file) const
{
    BOOL isThisFolder = this->isFolder();
    BOOL isFileFolder = file.isFolder();

    if (isThisFolder != isFileFolder)
        return isThisFolder ? FALSE : TRUE;
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
    CString fullPath = getFullPath();
    int slashPos = fullPath.ReverseFind('\\');
    return fullPath.Right(fullPath.GetLength() - slashPos - 1);
}

CString FileProperties::getFullPath() const
{
    return m_properties.m_szFullName;
}

CString FileProperties::getParentFolder() const
{
    CString fullPath = getFullPath();
    int slashPos = fullPath.ReverseFind('\\');
    return fullPath.Left(slashPos);
}

// won't work with foo/bar and foo ???
CString FileProperties::getRelativePath(const CString& rootFolder,
                                        BOOL withName) const
{
    CString fullPath = getFullPath();

    int rootPosition = fullPath.Find(rootFolder);
    // TODO: throw exception
    if (rootPosition != 0)
        return CString();

    int relativePathPos = fullPath.GetLength() - rootFolder.GetLength() - 1;
    CString relativePathWithName = fullPath.Right(relativePathPos);

    if (withName)
        return relativePathWithName;

    int slashPos = relativePathWithName.ReverseFind('\\');
    if (slashPos != -1)
        return relativePathWithName.Left(slashPos);
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
