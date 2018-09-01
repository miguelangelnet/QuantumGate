// This file is part of the QuantumGate project. For copyright and
// licensing information refer to the license file(s) in the project root.

#include "stdafx.h"
#include "CIPReputationsDlg.h"
#include "Common\Util.h"

using namespace QuantumGate::Implementation;

IMPLEMENT_DYNAMIC(CIPReputationsDlg, CDialogBase)

CIPReputationsDlg::CIPReputationsDlg(CWnd* pParent) : CDialogBase(IDD_IPREPUTATIONS_DIALOG, pParent)
{}

CIPReputationsDlg::~CIPReputationsDlg()
{}

void CIPReputationsDlg::SetAccessManager(QuantumGate::AccessManager * am) noexcept
{
	m_AccessManager = am;
}

void CIPReputationsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CIPReputationsDlg, CDialogBase)
END_MESSAGE_MAP()

BOOL CIPReputationsDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// Init reputation list
	auto flctrl = (CListCtrl*)GetDlgItem(IDC_IPREPUTATIONS_LIST);
	flctrl->SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	flctrl->InsertColumn(0, _T("IP"), LVCFMT_LEFT, 125);
	flctrl->InsertColumn(1, _T("Reputation"), LVCFMT_LEFT, 75);
	flctrl->InsertColumn(2, _T("Last Update Time"), LVCFMT_LEFT, 125);

	UpdateIPReputationList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CIPReputationsDlg::UpdateIPReputationList() noexcept
{
	auto flctrl = (CListCtrl*)GetDlgItem(IDC_IPREPUTATIONS_LIST);
	flctrl->DeleteAllItems();

	if (const auto result = m_AccessManager->GetAllIPReputations(); result.Succeeded())
	{
		for (const auto& rep : *result)
		{
			const auto pos = flctrl->InsertItem(0, rep.Address.GetCString());
			if (pos != -1)
			{
				flctrl->SetItemText(pos, 1, Util::FormatString(L"%d", rep.Reputation).c_str());

				tm time_tm{ 0 };
				WChar timestr[100]{ 0 };

				if (localtime_s(&time_tm, &rep.LastUpdateTime) == 0)
				{
					if (std::wcsftime(timestr, sizeof(timestr), L"%d/%m/%Y %H:%M:%S", &time_tm) != 0)
					{
						flctrl->SetItemText(pos, 2, timestr);
					}
				}
			}
		}
	}
}
