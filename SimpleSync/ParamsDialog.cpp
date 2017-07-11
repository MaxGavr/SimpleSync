#include "stdafx.h"
#include "SimpleSync.h"
#include "ParamsDialog.h"
#include "afxdialogex.h"



IMPLEMENT_DYNAMIC(CCompareParamsDialog, CDialogEx)

CCompareParamsDialog::CCompareParamsDialog(const SyncManager* syncManager, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMPARE_PARAMETERS_DIALOG, pParent)
{
    FileComparisonParameters defaultParameters = syncManager->getComparisonParameters();
    m_parameters = defaultParameters;

    m_compareSize = defaultParameters.compareSize;
    m_compareTime = defaultParameters.compareTime;
    m_timeParameterRadio = (int)defaultParameters.timeToCompare;
}

CCompareParamsDialog::~CCompareParamsDialog()
{
}

FileComparisonParameters CCompareParamsDialog::getParameters() const
{
    return m_parameters;
}

void CCompareParamsDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Radio(pDX, IDC_CREATION_TIME_RADIO, m_timeParameterRadio);
    DDX_Check(pDX, IDC_SIZE_PARAMETER_CHECK, m_compareSize);
    DDX_Check(pDX, IDC_TIME_PARAMETER_CHECK, m_compareTime);
}

void CCompareParamsDialog::OnParameterClicked(UINT id)
{
    UpdateData(TRUE);

    switch (id)
    {
    case IDC_CREATION_TIME_RADIO:
        m_parameters.timeToCompare = FileProperties::TIMES::CREATION_TIME;
        break;
    case IDC_WRITE_TIME_RADIO:
        m_parameters.timeToCompare = FileProperties::TIMES::LAST_WRITE_TIME;
        break;
    case IDC_ACCESS_TIME_RADIO:
        m_parameters.timeToCompare = FileProperties::TIMES::LAST_ACCESS_TIME;
        break;
    }
}


BEGIN_MESSAGE_MAP(CCompareParamsDialog, CDialogEx)
    ON_CONTROL_RANGE(BN_CLICKED, IDC_CREATION_TIME_RADIO, IDC_ACCESS_TIME_RADIO, &CCompareParamsDialog::OnParameterClicked)
    ON_BN_CLICKED(IDC_SIZE_PARAMETER_CHECK, &CCompareParamsDialog::OnSizeButtonClicked)
    ON_BN_CLICKED(IDC_TIME_PARAMETER_CHECK, &CCompareParamsDialog::OnTimeButtonClicked)
END_MESSAGE_MAP()


void CCompareParamsDialog::OnSizeButtonClicked()
{
    UpdateData(TRUE);
    m_parameters.compareSize = m_compareSize;
}


void CCompareParamsDialog::OnTimeButtonClicked()
{
    UpdateData(TRUE);
    m_parameters.compareTime = m_compareTime;

    for (int i = IDC_CREATION_TIME_RADIO; i <= IDC_ACCESS_TIME_RADIO; ++i)
        GetDlgItem(i)->EnableWindow(m_compareTime);
}
