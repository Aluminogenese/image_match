#pragma once
#include "afxdialogex.h"
#include<opencv2/opencv.hpp>
#include "resource.h"

// SelectFileDlg 对话框

class SelectFileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SelectFileDlg)

public:
	SelectFileDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~SelectFileDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SELECT_FILENAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 保存路径
	CString m_jpgFileName;
	afx_msg void OnBnClickedButtonSelectFilename();
	afx_msg void OnBnClickedOk();
	CString m_txtFileName;
	afx_msg void OnBnClickedButtonSelectFilename2();
};
