// SelectFileDlg.cpp: 实现文件
//

#include "pch.h"
#include "afxdialogex.h"
#include "SelectFileDlg.h"


// SelectFileDlg 对话框

IMPLEMENT_DYNAMIC(SelectFileDlg, CDialogEx)

SelectFileDlg::SelectFileDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SELECT_FILENAME, pParent)
	, m_jpgFileName(_T(""))
	, m_txtFileName(_T(""))
{

}

SelectFileDlg::~SelectFileDlg()
{
}

void SelectFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_jpgFileName);
	DDX_Text(pDX, IDC_EDIT2, m_txtFileName);
}


BEGIN_MESSAGE_MAP(SelectFileDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_FILENAME, &SelectFileDlg::OnBnClickedButtonSelectFilename)
	ON_BN_CLICKED(IDOK, &SelectFileDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_FILENAME2, &SelectFileDlg::OnBnClickedButtonSelectFilename2)
END_MESSAGE_MAP()


// SelectFileDlg 消息处理程序


void SelectFileDlg::OnBnClickedButtonSelectFilename()
{
	// TODO: 在此添加控件通知处理程序代码
		//文件类型说明字符串
	static char BASED_CODE file[] = "所有格式(*.*)|*.*";
	//文件对话框初始化
	CFileDialog SelectFile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, file, NULL);
	//弹出文件打开对话框
	SelectFile.DoModal();
	//得到所选文件路径
	CString FileName;
	FileName = SelectFile.GetPathName();
	//将得到的文件路径名赋给对话框成员变量m_sRawIn
	m_jpgFileName = FileName;
	UpdateData(FALSE);

}


void SelectFileDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void SelectFileDlg::OnBnClickedButtonSelectFilename2()
{
	// TODO: 在此添加控件通知处理程序代码
	static char BASED_CODE file[] = "所有格式(*.*)|*.*";
	//文件对话框初始化
	CFileDialog SelectFile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, file, NULL);
	//弹出文件打开对话框
	SelectFile.DoModal();
	//得到所选文件路径
	CString FileName;
	FileName = SelectFile.GetPathName();
	//将得到的文件路径名赋给对话框成员变量m_sRawIn
	m_txtFileName = FileName;
	UpdateData(FALSE);

}
