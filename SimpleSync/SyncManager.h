#pragma once

#include <set>
#include <deque>
#include "FileProperties.h"
#include "CopyOperation.h"
#include "RemoveOperation.h"



class SyncManager
{
public:
    using FileSet = std::set <FileProperties>;
    using OperationQueue = std::deque <SyncOperation *>;

    SyncManager();
    ~SyncManager();

    void setSourceFolder(const CString& source);
    void setDestinationFolder(const CString& destination);

    CString getSourceFolder() const;
    CString getDestinationFolder() const;

    OperationQueue scan();

private:
    FileSet getFilesFromFolder(const CString& folder) const;

private:
    CString m_sourceFolder;
    CString m_destinationFolder;

    OperationQueue m_syncActions;
};

