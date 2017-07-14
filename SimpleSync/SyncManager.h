#pragma once

#include <set>
#include <deque>
#include <algorithm>

#include "FileProperties.h"
#include "CopyOperation.h"
#include "RemoveOperation.h"
#include "EmptyOperation.h"
#include "ReplaceOperation.h"
#include "CreateOperation.h"



struct SyncManagerOptions
{
    BOOL recursive = TRUE; // used
    BOOL deleteFiles = FALSE; // used
    BOOL createEmptyFolders = FALSE;
    BOOL syncHiddenFiles = FALSE;
    BOOL copyMissingFiles = TRUE; //used
};


class SyncManager
{
public:
    using FileSet = std::set <FileProperties>;
    using OperationQueue = std::deque <SyncOperation *>;

    enum class SYNC_DIRECTION {
        LEFT_TO_RIGHT,
        BOTH,
        RIGHT_TO_LEFT
    };

    SyncManager();
    ~SyncManager();

    void setSourceFolder(const CString& source);
    void setDestinationFolder(const CString& destination);

    CString getSourceFolder() const;
    CString getDestinationFolder() const;

    void setSyncDirection(SYNC_DIRECTION direction);
    SYNC_DIRECTION getSyncDirection() const;

    void setOptions(const SyncManagerOptions& options);
    SyncManagerOptions getOptions() const;

    void setComparisonParameters(const FileComparisonParameters& params);
    FileComparisonParameters getComparisonParameters() const;

    BOOL isFileInSourceFolder(const FileProperties& file) const;
    BOOL isFileInDestinationFolder(const FileProperties& file) const;

    CString getFileRelativePath(const FileProperties& file, BOOL withName) const;

    BOOL isFileInFiles(const FileProperties& file, const FileSet& files) const;

    BOOL scan();
    void sync();
    OperationQueue getOperations();

private:
    BOOL folderExists(const CString& folder) const;
    FileSet getFilesFromFolder(const CString& folder) const;
    void scanFolders(CString source, CString destination);
    
    void manageCopyOperation(const FileProperties& fileToCopy, CString destinationFolder);
    void manageReplaceOperation(const FileProperties& originalFile, const FileProperties& fileToReplace);
    void manageRemoveOperation(const FileProperties& fileToRemove);

private:
    CString m_sourceFolder;
    CString m_destinationFolder;

    SYNC_DIRECTION m_syncDirection;
    SyncManagerOptions m_options;
    FileComparisonParameters m_compareParameters;

    OperationQueue m_syncActions;
};

