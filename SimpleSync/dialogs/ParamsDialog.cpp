#include "stdafx.h"
#include "../SimpleSync.h"
#include "ParamsDialog.h"
#include "../sync/SyncManager.h"
#include "afxdialogex.h"



IMPLEMENT_DYNAMIC(CCompParametersDialog, CDialogEx)

CCompParametersDialog::CCompParametersDialog(const SyncManager* syncManager,
                                             CWnd* pParent)
	: CDialogEx(IDD_COMPARE_PARAMETERS_DIALOG, pParent)
{
    auto defaultParameters = syncManager->getComparisonParameters();
    m_parameters = defaultParameters;

    m_compareSize = defaultParameters.m_compareSize;
    m_compareTime = defaultParameters.m_compareTime;
    m_timeParameterRadio = (int)defaultParameters.m_timeToCompare;
}

CCompParametersDialog::~CCompParametersDialog()
{
}

FileComparisonParameters CCompParametersDialog::getParameters() const
{
    return m_parameters;
}

void CCompParametersDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Radio(pDX, IDC_CREATION_TIME_RADIO, m_timeParameterRadio);
    DDX_Check(pDX, IDC_SIZE_PARAMETER_CHECK, m_compareSize);
    DDX_Check(pDX, IDC_TIME_PARAMETER_CHECK, m_compareTime);
}

void CCompParametersDialog::OnTimeRadioBoxClicked(UINT id)
{
    UpdateData(TRUE);

    switch (id)
    {
    case IDC_CREATION_TIME_RADIO:
        m_parameters.m_timeToCompare = FileProperties::TIME_STAMP::CREATION_TIME;
        break;
    case IDC_WRITE_TIME_RADIO:
        m_parameters.m_timeToCompare = FileProperties::TIME_STAMP::LAST_WRITE_TIME;
        break;
    case IDC_ACCESS_TIME_RADIO:
        m_parameters.m_timeToCompare = FileProperties::TIME_STAMP::LAST_ACCESS_TIME;
        break;
    }
}


BEGIN_MESSAGE_MAP(CCompParametersDialog, CDialogEx)
    ON_CONTROL_RANGE(BN_CLICKED,
                     IDC_CREATION_TIME_RADIO,
                     IDC_ACCESS_TIME_RADIO,
                     &CCompParametersDialog::OnTimeRadioBoxClicked)
    ON_BN_CLICKED(IDC_SIZE_PARAMETER_CHECK,
                  &CCompParametersDialog::OnSizeCheckBoxClicked)
    ON_BN_CLICKED(IDC_TIME_PARAMETER_CHECK,
                  &CCompParametersDialog::OnTimeCheckBoxClicked)
END_MESSAGE_MAP()


void CCompParametersDialog::OnSizeCheckBoxClicked()
{
    UpdateData(TRUE);
    m_parameters.m_compareSize = m_compareSize;
}


void CCompParametersDialog::OnTimeCheckBoxClicked()
{
    UpdateData(TRUE);
    m_parameters.m_compareTime = m_compareTime;

    enableTimeRadioBoxes(m_compareTime);
}

void CCompParametersDialog::enableTimeRadioBoxes(BOOL enable)
{
    for (int i = IDC_CREATION_TIME_RADIO; i <= IDC_ACCESS_TIME_RADIO; ++i)
        GetDlgItem(i)->EnableWindow(enable);
}


BOOL CCompParametersDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    enableTimeRadioBoxes(m_compareTime);

    return TRUE;
}
