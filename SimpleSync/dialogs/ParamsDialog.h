#pragma once

#include "afxwin.h"
#include "sync\SyncManager.h"



class CCompParametersDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCompParametersDialog)

public:
	CCompParametersDialog(const SyncManager* syncManager,
                          CWnd* pParent = NULL);
	virtual ~CCompParametersDialog();

    FileComparisonParameters getParameters() const;

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMPARE_PARAMETERS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

public:
    virtual BOOL OnInitDialog();

    afx_msg void OnTimeRadioBoxClicked(UINT id);
    afx_msg void OnSizeCheckBoxClicked();
    afx_msg void OnTimeCheckBoxClicked();

private:
    void enableTimeRadioBoxes(BOOL enable);

    FileComparisonParameters m_parameters;
    
    BOOL m_compareSize;
    BOOL m_compareTime;
    int m_timeParameterRadio;
};
