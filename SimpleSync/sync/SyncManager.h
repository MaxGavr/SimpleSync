#pragma once

#include <set>
#include <deque>
#include <algorithm>
#include <functional>
#include "sync/FileProperties.h"


class SyncOperation;



struct SyncManagerOptions
{
    BOOL recursive = TRUE;
    BOOL deleteFiles = FALSE;
    BOOL createEmptyFolders = FALSE;
    BOOL syncHiddenFiles = FALSE;
    BOOL copyMissingFiles = TRUE;
};


class SyncManager
{
public:
    using FileSet = std::set <FileProperties>;
    using OperationQueue = std::deque <SyncOperation *>;
    using SyncCallback = std::function <void (const SyncOperation*)>;
    using ScanCallback = std::function <void (const CString&)>;

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

    BOOL scan(ScanCallback* callback);
    void sync(SyncCallback* callback);
    OperationQueue getOperations();

private:
    BOOL folderExists(const CString& folder) const;
    BOOL fileMeetsRequirements(const FileProperties& file) const;
    FileSet getFilesFromFolder(const CString& folder) const;
    
    void scanFolders(const CString& source, const CString& destination,
                     ScanCallback* callback);
    
    void clearOperationQueue();

    void enqueueOperation(SyncOperation* operation);
    void manageCopyOperation(const FileProperties& fileToCopy,
                             const CString& destinationFolder);
    void manageReplaceOperation(const FileProperties& originalFile,
                                const FileProperties& fileToReplace);
    void manageRemoveOperation(const FileProperties& fileToRemove);

private:
    CString m_sourceFolder;
    CString m_destinationFolder;

    SYNC_DIRECTION m_syncDirection;
    SyncManagerOptions m_options;
    FileComparisonParameters m_compareParameters;

    OperationQueue m_syncActions;
};

