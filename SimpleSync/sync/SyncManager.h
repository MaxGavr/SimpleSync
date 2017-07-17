#pragma once

#include <set>
#include <deque>
#include <algorithm>
#include <functional>

#include "operations/CopyOperation.h"
#include "operations/ReplaceOperation.h"
#include "operations/RemoveOperation.h"
#include "operations/EmptyOperation.h"
#include "operations/CreateOperation.h"

#include "FileProperties.h"



struct SyncManagerOptions
{
    BOOL recursive = TRUE;
    BOOL deleteFiles = FALSE;
    BOOL createEmptyFolders = FALSE;
    BOOL syncHiddenFiles = FALSE;
    BOOL copyMissingFiles = TRUE;
};


// Primary class that handles most sync routine
class SyncManager
{
public:
    // Uses FileProperties::operator< to store files in certain order
    // see FileProperties::operator< declaration
    using FileSet = std::set <FileProperties>;

    using OperationQueue = std::deque <SyncOperation::ptr>;

    // Called right before execution of SyncOperation
    // argument - SyncOperation that is about to be executed
    using SyncCallback = std::function <void (SyncOperation::ptr&)>;

    // Called before scanning folder
    // argument - folder
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

    // Checks if file lies in subfolder of source/destination folder
    // or directly there
    BOOL isFileInSourceFolder(const FileProperties& file) const;
    BOOL isFileInDestinationFolder(const FileProperties& file) const;
    BOOL isFileInFileSet(const FileProperties& file,
                         const FileSet& files) const;

    // Get file path, relatively to source/destination folder
    CString getFileRelativePath(const FileProperties& file,
                                BOOL withName) const;

public:
    // Must be called before sync() to create queue of operations
    BOOL scan(ScanCallback* callback);

    void sync(SyncCallback* callback);
    OperationQueue getOperationQueue();

private:
    BOOL folderExists(const CString& folder) const;

    // Checks if certain SyncManagerOptions apply to file
    BOOL fileMeetsRequirements(const FileProperties& file) const;
    
    FileSet getFilesFromFolder(const CString& folder) const;
    
    // Called recursively while scanning
    void scanFolders(const CString& source,
                     const CString& destination,
                     ScanCallback* callback);

    void enqueueOperation(SyncOperation* operation);

    // Used in scanFolders(); each call enqueueOperation() if needed
    void manageCopyOperation(const FileProperties& fileToCopy,
                             const CString& destinationFolder);
    void manageReplaceOperation(const FileProperties& originalFile,
                                const FileProperties& fileToReplace);
    void manageRemoveOperation(const FileProperties& fileToRemove);

    void clearOperationQueue();

private:
    CString m_sourceFolder;
    CString m_destinationFolder;

    OperationQueue m_syncOperations;

    SYNC_DIRECTION m_syncDirection;
    SyncManagerOptions m_options;
    FileComparisonParameters m_compareParameters;
};

