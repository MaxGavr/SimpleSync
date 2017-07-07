#pragma once

#include "SyncManager.h"

class CPreviewListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CPreviewListCtrl)

public:
    enum COLUMNS {
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
    void printSyncAction(const SyncAction& action);

    SyncManager* m_syncManager;
};

