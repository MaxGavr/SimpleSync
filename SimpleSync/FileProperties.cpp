#include "stdafx.h"
#include "FileProperties.h"
#include <algorithm>




FileProperties::FileProperties(const CString& fileName, ULONGLONG sizeInBytes,
                               const CTime& creationTime, const CTime& accessTime, const CTime& writeTime,
                               BOOL isDir)
    : m_fileName(fileName),
      m_creationTime(creationTime),
      m_lastAccessTime(accessTime),
      m_lastWriteTime(writeTime),
      m_isDirectory(isDir),
      m_sizeInBytes(sizeInBytes)
{
}

FileProperties::FileProperties(const CString& fileName, BOOL isDir)
    : m_fileName(fileName), m_isDirectory(isDir)
{
}


FileProperties::~FileProperties()
{
}

FileProperties::COMPARISON_RESULT FileProperties::compareTo(const FileProperties& file,
                                                            const FileComparisonParameters& params) const
{
    if (this->getFileName() != file.getFileName())
    {
        return COMPARISON_RESULT::UNDEFINED;
    }

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

ULONGLONG FileProperties::getSize() const
{
    return m_sizeInBytes;
}

CTime FileProperties::getCreationTime() const
{
    return m_creationTime;
}

CTime FileProperties::getLastAccessTime() const
{
    return m_lastAccessTime;
}

CTime FileProperties::getLastWriteTime() const
{
    return m_lastWriteTime;
}

BOOL FileProperties::isDirectory() const
{
    return m_isDirectory;
}

FileProperties::COMPARISON_RESULT FileProperties::makeChoice(ComparisonResults& results) const
{
    results.remove(COMPARISON_RESULT::EQUAL);

    if (results.empty())
    {
        return COMPARISON_RESULT::EQUAL;
    }

    BOOL hasPreferable = std::find(results.begin(), results.end(), COMPARISON_RESULT::PREFERABLE) != results.end();
    BOOL hasNonPreferable = std::find(results.begin(), results.end(), COMPARISON_RESULT::NON_PREFERABLE) != results.end();
    
    if (hasPreferable)
    {
        if (hasNonPreferable)
        {
            return COMPARISON_RESULT::UNDEFINED;
        }
        else
        {
            return COMPARISON_RESULT::PREFERABLE;
        }
    }
    else
    {
        return COMPARISON_RESULT::NON_PREFERABLE;
    }
}
