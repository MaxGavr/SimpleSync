#pragma once

#include <set>
#include <deque>
#include <algorithm>

#include "FileProperties.h"
#include "CopyOperation.h"
#include "RemoveOperation.h"
#include "EmptyOperation.h"



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

    BOOL isFileInSourceFolder(const FileProperties& file) const;
    BOOL isFileInDestinationFolder(const FileProperties& file) const;

    BOOL isFileInFiles(const FileProperties& file, const FileSet& files) const;

    OperationQueue scan();
    void sync();

private:
    FileSet getFilesFromFolder(const CString& folder) const;

private:
    CString m_sourceFolder;
    CString m_destinationFolder;

    OperationQueue m_syncActions;
};

