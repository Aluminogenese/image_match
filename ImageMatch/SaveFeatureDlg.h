#pragma once
#include "afxdialogex.h"
#include <opencv2/opencv.hpp>
#include "SelectFileDlg.h"

// SaveFeatureDlg 对话框

class SaveFeatureDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SaveFeatureDlg)

public:
	SaveFeatureDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~SaveFeatureDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SAVE_FEATURE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_jpgFileName;
	CString m_txtFileName;
	afx_msg void OnBnClickedOk();
};
