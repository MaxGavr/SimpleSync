#pragma once

#include <set>
#include "FileProperties.h"

struct SyncAction
{
    enum class TYPE {
        REMOVE,
        CREATE,
        COPY,
        REPLACE
    };

    enum class DIRECTION {
        SOURCE_TO_DESTINATION,
        DESTINATION_TO_SOURCE
    };

    SyncAction(TYPE type, FileProperties sourceFile, FileProperties destinationFile);

    TYPE m_type;
    DIRECTION m_copyDirection;

    FileProperties m_sourceFile;
    FileProperties m_destinationFile;
};

class SyncManager
{
public:
    using FileSet = std::set <FileProperties>;

    SyncManager();
    ~SyncManager();

    void setSourceFolder(const CString& source);
    void setDestinationFolder(const CString& destination);

    CString getSourceFolder() const;
    CString getDestinationFolder() const;

    FileSet scan();

private:
    FileSet getFilesFromFolder(const CString& folder) const;

private:
    CString m_sourceFolder;
    CString m_destinationFolder;
};

