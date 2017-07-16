#pragma once

#include "SyncManager.h"

#define WM_ADJUST_COLUMNS (WM_USER + 10)



struct PreviewListColors
{
    const COLORREF DEFAULT_COLOR = RGB(0, 0, 0);
    const COLORREF SOURCE_TO_DESTINATION_COLOR = RGB(0, 255, 0);
    const COLORREF DESTINATION_TO_SOURCE_COLOR = RGB(0, 0, 255);
    const COLORREF DELETION_COLOR = RGB(255, 0, 0);
    const COLORREF AMBIGUOUS_COLOR = RGB(75, 0, 130);

    const COLORREF FOLDER_BACK_COLOR = RGB(252, 217, 134);
    const COLORREF DEFAULT_BACK_COLOR = RGB(255, 255, 255);
    const COLORREF FORBIDDEN_BACK_COLOR = RGB(255, 115, 115);
    const COLORREF FORBIDDEN_FOLDER_BACK_COLOR = RGB(252, 166, 134);
};


class CPreviewListCtrl : public CMFCListCtrl
{
	DECLARE_DYNAMIC(CPreviewListCtrl)

public:
    enum LIST_COLUMNS {
        INDEX = 0,
        SOURCE_FILE = 1,
        ACTION = 2,
        DESTINATION_FILE = 3
    };
    
    enum ICONS {
        RIGHT_ARROW = 0,
        EQUAL = 1,
        LEFT_ARROW = 2,
        FOLDER = 3,
        REMOVE = 4,
        QUESTION = 5
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

    void forbidOperation(int index);

    void sortOperationsByFolders(SyncManager::OperationQueue& operations);
    void adjustColumnsWidth();

    SyncManager* m_syncManager;
    SyncManager::OperationQueue m_sortedOperations;

public:
    afx_msg void OnColumnDragEnd(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDoubleClick(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnRightClick(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg LRESULT OnAdjustColumns(WPARAM wParam, LPARAM lParam);

public:
    virtual COLORREF OnGetCellTextColor(int nRow, int nColumn) override;
    virtual COLORREF OnGetCellBkColor(int nRow, int nColumn) override;

private:
    BOOL showFilePropertiesDialog(const SyncOperation* singleFileOperation);
    BOOL showFilesComparisonDialog(const SyncOperation* doubleFileOperation);

    void printOperationIndex(const SyncOperation* operation, int& index);
    void printOperationIcon(ICONS icon, int index);

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
    const int PREVIEW_LIST_IMAGE_COUNT = 6;
};
