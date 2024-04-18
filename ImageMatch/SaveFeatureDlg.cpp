// SaveFeatureDlg.cpp: 实现文件
//

#include "pch.h"
#include "afxdialogex.h"
#include "SaveFeatureDlg.h"


// SaveFeatureDlg 对话框

IMPLEMENT_DYNAMIC(SaveFeatureDlg, CDialogEx)

SaveFeatureDlg::SaveFeatureDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SAVE_FEATURE, pParent)
{

}

SaveFeatureDlg::~SaveFeatureDlg()
{
}

void SaveFeatureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SaveFeatureDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &SaveFeatureDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// SaveFeatureDlg 消息处理程序


void SaveFeatureDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	SelectFileDlg dlg;
	if (IDOK == dlg.DoModal())
	{
		m_jpgFileName = dlg.m_jpgFileName;
		m_txtFileName = dlg.m_txtFileName;
	}
	CDialogEx::OnOK();
}
