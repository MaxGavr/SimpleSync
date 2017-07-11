#pragma once

#include <list>

struct FileComparisonParameters;

class FileProperties
{
public:

    enum class COMPARISON_RESULT {
        EQUAL,
        PREFERABLE,
        NON_PREFERABLE,
        UNDEFINED
    };
    using ComparisonResults = std::list <COMPARISON_RESULT>;

    FileProperties(const CString& fileName, ULONGLONG sizeInBytes,
                   const CTime& creationTime, const CTime& accessTime, const CTime& writeTime,
                   BOOL isDir = FALSE);
    FileProperties(const CString& fileName, BOOL isDir = FALSE);
    ~FileProperties();

    enum class TIMES {
        CREATION_TIME,
        LAST_WRITE_TIME,
        LAST_ACCESS_TIME
    };

    COMPARISON_RESULT compareTo(const FileProperties& file, const FileComparisonParameters& params) const;

    BOOL operator< (const FileProperties& file) const;
    BOOL operator== (const FileProperties& file) const;

    CString getFileName() const;
    CString getFullPath() const;
    CString getFileFolder() const;

    ULONGLONG getSize() const;

    CTime getCreationTime() const;
    CTime getLastAccessTime() const;
    CTime getLastWriteTime() const;

    BOOL isDirectory() const;

private:
    COMPARISON_RESULT makeChoice(ComparisonResults& results) const;

    template<class T>
    COMPARISON_RESULT compareProperty(const T& first, const T& second) const;

    CString m_fileName;

    ULONGLONG m_sizeInBytes;

    CTime m_creationTime;
    CTime m_lastAccessTime;
    CTime m_lastWriteTime;

    BOOL m_isDirectory;
};

struct FileComparisonParameters
{
    BOOL compareSize = FALSE;
    BOOL compareTime = FALSE;
    FileProperties::TIMES timeToCompare = FileProperties::TIMES::LAST_WRITE_TIME;
};

template<>
inline FileProperties::COMPARISON_RESULT FileProperties::compareProperty(const BOOL& first, const BOOL& second) const
{
    if (first || !second)
        return COMPARISON_RESULT::PREFERABLE;
    else
        return first == second ? COMPARISON_RESULT::EQUAL : COMPARISON_RESULT::NON_PREFERABLE;
}

template<class T>
FileProperties::COMPARISON_RESULT FileProperties::compareProperty(const T& first, const T& second) const
{
    if (first > second)
        return COMPARISON_RESULT::PREFERABLE;
    else
        return first == second ? COMPARISON_RESULT::EQUAL : COMPARISON_RESULT::NON_PREFERABLE;
}
