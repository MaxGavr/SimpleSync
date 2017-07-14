#pragma once

#include "SyncManager.h"



class CPreviewListCtrl : public CMFCListCtrl
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
    void printSyncAction(SyncOperation* action, int index = -1);

    void printCopyOperation(CopyOperation* operation, int index);
    void printRemoveOperation(RemoveOperation* operation, int index);
    void printReplaceOperation(ReplaceOperation* operation, int index);
    void printEmptyOperation(EmptyOperation* operation, int index);
    void printCreateOperation(CreateFolderOperation* operation, int index);

    void sortOperationsByFolders(SyncManager::OperationQueue& operations);
    void optimizeColumnsWidth();

    SyncManager* m_syncManager;
    SyncManager::OperationQueue m_sortedOperations;


public:
    afx_msg void OnDoubleClick(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnRightClick(NMHDR *pNMHDR, LRESULT *pResult);

private:
    BOOL showFilePropertiesDialog(const SyncOperation* singleFileOperation);
    BOOL showFilesComparisonDialog(const SyncOperation* doubleFileOperation);


public:
    virtual COLORREF OnGetCellTextColor(int nRow, int nColumn) override;

private:
    COLORREF chooseOperationTextColor(const SyncOperation* operation) const;
    
    const COLORREF DEFAULT_COLOR = RGB(0, 0, 0);
    const COLORREF SOURCE_TO_DESTINATION_COLOR = RGB(0, 255, 0);
    const COLORREF DESTINATION_TO_SOURCE_COLOR = RGB(0, 0, 255);
    const COLORREF DELETION_COLOR = RGB(255, 0, 0);
    const COLORREF AMBIGUOUS_COLOR = RGB(75, 0, 130);
};


