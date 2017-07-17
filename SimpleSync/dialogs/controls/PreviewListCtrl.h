#pragma once

#include "sync\SyncManager.h"

#define WM_ADJUST_COLUMNS (WM_USER + 10)



struct PreviewListColors
{
    const COLORREF DEFAULT_TEXT_COLOR = RGB(0, 0, 0);
    const COLORREF SOURCE_TO_DESTINATION_TEXT_COLOR = RGB(17, 173, 98);
    const COLORREF DESTINATION_TO_SOURCE_TEXT_COLOR = RGB(57, 64, 196);
    const COLORREF REMOVE_TEXT_COLOR = RGB(255, 0, 0);
    const COLORREF AMBIGUOUS_TEXT_COLOR = RGB(142, 68, 173);

    const COLORREF DEFAULT_BACK_COLOR = RGB(255, 255, 255);
    const COLORREF FOLDER_BACK_COLOR = RGB(255, 231, 173);
    const COLORREF FORBIDDEN_BACK_COLOR = RGB(255, 213, 213);
    const COLORREF FORBIDDEN_FOLDER_BACK_COLOR = RGB(255, 194, 194);
};


// MFC list control designed to show scan results from SyncManager
class CPreviewListCtrl : public CMFCListCtrl
{
	DECLARE_DYNAMIC(CPreviewListCtrl)

public:
    enum LIST_COLUMN {
        INDEX = 0,
        SOURCE_FILE = 1,
        ACTION = 2,
        DESTINATION_FILE = 3
    };
    
    enum ICON {
        RIGHT_ARROW = 0,
        EQUAL = 1,
        LEFT_ARROW = 2,
        FOLDER = 3,
        REMOVE = 4,
        QUESTION = 5
    };

	CPreviewListCtrl(SyncManager* syncManager);
	virtual ~CPreviewListCtrl();

    // Must be called before showPreview()
    void setupPreviewList();

    void adjustColumnsWidth();

    // Shows sync preview based on current queue of operations in SyncManager
    void showPreview();
    void clearPreview();

protected:
	DECLARE_MESSAGE_MAP()

private:
    void printSyncOperation(SyncOperation* op, int index = -1);

    void printFile(const FileProperties& file, int index, LIST_COLUMN column);
    void printOperationIndex(const SyncOperation* operation, int& index);
    void printOperationIcon(ICON icon, int index);

    void printCopyOperation(CopyOperation* operation, int index);
    void printRemoveOperation(RemoveOperation* operation, int index);
    void printReplaceOperation(ReplaceOperation* operation, int index);
    void printEmptyOperation(EmptyOperation* operation, int index);
    void printCreateOperation(CreateFolderOperation* operation, int index);

    // Used to recursively forbid dependent operations
    int forbidOperation(int index);

    // Must be called before using m_sortedOperations
    void sortOperationsByFolders(SyncManager::OperationQueue& operations);

    SyncManager* m_syncManager;
    SyncManager::OperationQueue m_sortedOperations;

public:
    // Prevents column resizing
    afx_msg void OnColumnResizeDragEnd(NMHDR *pNMHDR, LRESULT *pResult);

    afx_msg void OnDoubleClick(NMHDR *pNMHDR, LRESULT *pResult);

    // Calls forbidOperation() on clicked operation
    afx_msg void OnRightClick(NMHDR *pNMHDR, LRESULT *pResult);

    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg LRESULT OnAdjustColumns(WPARAM wParam, LPARAM lParam);

public:
    // Called by framework
    virtual COLORREF OnGetCellTextColor(int nRow, int nColumn) override;
    virtual COLORREF OnGetCellBkColor(int nRow, int nColumn) override;

private:
    // Dialogs that are created on double click:
    // if operation involves one file - CFilePropertiesDialog
    // if operation involves two files - CCompareFilesDialog
    // if operation involves folders - nothing is shown
    BOOL showFilePropertiesDialog(const SyncOperation* singleFileOperation);
    BOOL showFilesComparisonDialog(const SyncOperation* doubleFileOperation);

    // Return operation foreground/background color depending on its type
    COLORREF chooseOperationTextColor(const SyncOperation* operation) const;
    COLORREF chooseOperationBkColor(const SyncOperation* operation) const;
    
    PreviewListColors m_colors;
    
    CImageList m_imageList;
    CPngImage m_leftArrowImageSmall;
    CPngImage m_rightArrowImageSmall;
    CPngImage m_equalImageSmall;
    CPngImage m_folderImageSmall;
    CPngImage m_removeImageSmall;
    CPngImage m_questionImageSmall;

    const int PREVIEW_LIST_IMAGE_SIZE = 20;
    
    // Should be equal to amount of m_*ImageSmall
    // and to number of elements in ICON enum
    const int PREVIEW_LIST_IMAGE_COUNT = 6;
};
