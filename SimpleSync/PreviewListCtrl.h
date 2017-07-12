#pragma once

#include "SyncManager.h"

class CPreviewListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CPreviewListCtrl)

public:
    enum LIST_COLUMNS {
        INDEX,
        SOURCE_FILE,
        ACTION,
        DESTINATION_FILE
    };

	CPreviewListCtrl(SyncManager* syncManager);
	virtual ~CPreviewListCtrl();

    void setupColumns();

    void showPreview();

protected:
	DECLARE_MESSAGE_MAP()

private:
    void printSyncAction(SyncOperation* action);

    void printCopyOperation(CopyOperation* operation, int index);
    void printRemoveOperation(RemoveOperation* operation, int index);
    void printReplaceOperation(ReplaceOperation* operation, int index);
    void printEmptyOperation(EmptyOperation* operation, int index);
    void printCreateOperation(CreateFolderOperation* operation, int index);

    void sortOperationsByFolders(SyncManager::OperationQueue& operations);
    void optimizeColumnsWidth();

    SyncManager* m_syncManager;
    SyncManager::OperationQueue m_sortedOperations;
};


