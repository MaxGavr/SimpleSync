#pragma once

#include <list>

struct FileComparisonParameters;



// Contains file properties, such as:
// full path to file, size, time stamps and system attributes
class FileProperties
{
public:
    enum class COMPARISON_RESULT {
        EQUAL, 
        PREFERABLE,
        NON_PREFERABLE,
        UNDEFINED // preference cannot be determined
    };

    enum class TIME_STAMP {
        CREATION_TIME,
        LAST_WRITE_TIME,
        LAST_ACCESS_TIME
    };

    using ComparisonResults = std::list <COMPARISON_RESULT>;

    // TODO: rewrite comparison mechanism to support arbitrary amount
    //       of parameters (maybe using map of comparator functors)
    // TODO: check for correctness CFileStatus struct's members
    // TODO: exceptions
    // TODO: probably add temporary flag
    FileProperties(const CFileStatus& properties);
    FileProperties(const CString& fileName = _T(""), BOOL isFolder = FALSE);
    ~FileProperties();

    // Can be used to compare various properties, such as times and size
    // Greater value (>) is PREFERABLE
    template<class T>
    static COMPARISON_RESULT compareProperty(const T& first, const T& second);

    COMPARISON_RESULT compareTo(const FileProperties& file,
                                const FileComparisonParameters& params) const;

    FileProperties operator= (const FileProperties& file);

    // Required by SyncManager::FileSet to sort files by name
    // in lexicographical order
    // in addition, folders precede files;
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
    BOOL isParentFolder(const FileProperties& parentFolder) const;

    BOOL isArchived() const;
    BOOL isSystem() const;
    BOOL isHidden() const;
    BOOL isReadOnly() const;

private:
    COMPARISON_RESULT makeChoice(ComparisonResults& results) const;

    CFileStatus m_properties;
};


struct FileComparisonParameters
{
    BOOL m_compareSize = FALSE;
    BOOL m_compareTime = FALSE;
    FileProperties::TIME_STAMP m_timeToCompare =
        FileProperties::TIME_STAMP::LAST_WRITE_TIME;
};



template<class T>
FileProperties::COMPARISON_RESULT FileProperties::compareProperty(const T& prop1,
                                                                  const T& prop2)
{
    COMPARISON_RESULT result;

    if (prop1 > prop2)
        result = COMPARISON_RESULT::PREFERABLE;
    else
    {
        if (prop1 == prop2)
            result = COMPARISON_RESULT::EQUAL;
        else
            result = COMPARISON_RESULT::NON_PREFERABLE;
    }

    return result;
}
