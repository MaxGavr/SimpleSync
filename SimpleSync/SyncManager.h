#pragma once

#include <set>
#include <deque>
#include "FileProperties.h"

struct SyncAction
{
    enum class TYPE {
        REMOVE,
        COPY,
        REPLACE
    };

    /*enum class DIRECTION {
        SOURCE_TO_DESTINATION,
        DESTINATION_TO_SOURCE
    };*/

    SyncAction(TYPE type, FileProperties file);
    SyncAction(FileProperties originalFile, FileProperties fileToReplace);

    //SyncAction(TYPE type, FileProperties file);

    TYPE m_type;
    //DIRECTION m_direction;

    FileProperties m_file;
    FileProperties m_fileToReplace;
    //FileProperties m_secondFile;
};

class SyncManager
{
public:
    using FileSet = std::set <FileProperties>;
    using ActionQueue = std::deque <SyncAction>;

    SyncManager();
    ~SyncManager();

    void setSourceFolder(const CString& source);
    void setDestinationFolder(const CString& destination);

    CString getSourceFolder() const;
    CString getDestinationFolder() const;

    ActionQueue scan();

private:
    FileSet getFilesFromFolder(const CString& folder) const;

private:
    CString m_sourceFolder;
    CString m_destinationFolder;
};

