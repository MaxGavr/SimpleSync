#pragma once

#include "afxwin.h"
#include "FilePropertiesDialog.h"

class SyncManager;



class CCompareFilesDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCompareFilesDialog)

public:
	CCompareFilesDialog(const FileProperties& leftFile,
                        const FileProperties& rightFile,
                        const SyncManager* syncManager,
                        CWnd* pParent = NULL);
	virtual ~CCompareFilesDialog();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMPARE_FILES_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

public:
    virtual BOOL OnInitDialog();

private:
    CPngImage& chooseImage(FileProperties::COMPARISON_RESULT comparisonResult);

    const SyncManager* m_syncManager;

    FileProperties m_leftFile;
    FileProperties m_rightFile;

    CPngImage m_greaterImage;
    CPngImage m_lessImage;
    CPngImage m_equalImage;

    CStatic m_sizeBitmap;
    CStatic m_creationTimeBitmap;
    CStatic m_writeTimeBitmap;
    CStatic m_accessTimeBitmap;

    FilePropertiesControls m_leftControls;
    FilePropertiesControls m_rightControls;
};
