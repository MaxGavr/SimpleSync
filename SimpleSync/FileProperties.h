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

    enum class TIMES {
        CREATION_TIME,
        LAST_WRITE_TIME,
        LAST_ACCESS_TIME
    };
    using ComparisonResults = std::list <COMPARISON_RESULT>;

    FileProperties(const CFileStatus& properties);
    FileProperties(const CString& fileName, BOOL isFolder = FALSE);
    ~FileProperties();

    COMPARISON_RESULT compareTo(const FileProperties& file, const FileComparisonParameters& params) const;

    BOOL operator< (const FileProperties& file) const;
    BOOL operator== (const FileProperties& file) const;

    CString getFileName() const;
    CString getFullPath() const;
    CString getParentFolder() const;
    CString getRelativePath(const CString& rootFolder, BOOL withName) const;

    ULONGLONG getSize() const;

    CTime getCreationTime() const;
    CTime getLastAccessTime() const;
    CTime getLastWriteTime() const;

    BOOL isFolder() const;
    BOOL isParentFolder(const FileProperties& folder) const;

private:
    COMPARISON_RESULT makeChoice(ComparisonResults& results) const;

    template<class T>
    COMPARISON_RESULT compareProperty(const T& first, const T& second) const;

    CFileStatus m_properties;
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
