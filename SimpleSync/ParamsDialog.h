#pragma once

#include "SyncManager.h"
#include "afxwin.h"


class CCompareParamsDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCompareParamsDialog)

public:
	CCompareParamsDialog(const SyncManager* syncManager, CWnd* pParent = NULL);
	virtual ~CCompareParamsDialog();

    FileComparisonParameters getParameters() const;

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMPARE_PARAMETERS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnParameterClicked(UINT id);

private:
    FileComparisonParameters m_parameters;
    
    BOOL m_compareSize;
    BOOL m_compareTime;
    int m_timeParameterRadio;

public:
    afx_msg void OnSizeButtonClicked();
    afx_msg void OnTimeButtonClicked();
};
