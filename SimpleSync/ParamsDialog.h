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
    virtual BOOL OnInitDialog();

    afx_msg void OnParameterClicked(UINT id);
    afx_msg void OnSizeButtonClicked();
    afx_msg void OnTimeButtonClicked();

private:
    void enableTimeRadioBoxes(BOOL enable);

    FileComparisonParameters m_parameters;
    
    BOOL m_compareSize;
    BOOL m_compareTime;
    int m_timeParameterRadio;
};
