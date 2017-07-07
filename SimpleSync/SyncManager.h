#pragma once

#include <set>
#include <queue>
#include "FileProperties.h"

struct SyncAction
{
    enum class TYPE {
        REMOVE,
        COPY,
        REPLACE
    };

    //enum class DIRECTION {
    //    SOURCE_TO_DESTINATION,
    //    DESTINATION_TO_SOURCE
    //};

    //SyncAction(FileProperties fileToDelete);
    //SyncAction(FileProperties originalFile, DIRECTION copyDirection);
    //SyncAction(FileProperties originalFile, FileProperties fileToReplace, DIRECTION replaceDirection);

    SyncAction(TYPE type, FileProperties file);

    TYPE m_type;
    //DIRECTION m_copyDirection;

    FileProperties m_file;
};

class SyncManager
{
public:
    using FileSet = std::set <FileProperties>;
    using ActionQueue = std::queue <SyncAction>;

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

