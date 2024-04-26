
// ImageMatchDlg.h: 头文件
//

#pragma once

#include <string>
#include "feature_extraction.h"
#include "correlation_matcher.h"
#include "lsq_matcher.h"
#include "match_point_pair.h"
#include "SaveFeatureDlg.h"

// CImageMatchDlg 对话框
class CImageMatchDlg : public CDialogEx
{
// 构造
public:
	CImageMatchDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMAGEMATCH_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 左影像文件路径
	CString left_image_path;
	// 右影像文件路径
	CString right_image_path;

	int feature_extraction_method;

	int moravec_window_size;
	int moravec_restrain_winSize;
	int moravec_threshold;
	int harris_threshold;
	int corr_window_size;
	float corr_threshold;

	int lsq_window_size;
	float lsq_threshold;
	// 在两张影像上分别提取角点
	cv::Mat left_image, right_image;
	std::vector<MatchPointPair>corr_match_points;

	CString m_status;
	CStatusBar progress_bar;
	CRect m_rect;

	afx_msg void OnClickedButtonLeftImage();
	afx_msg void OnClickedButtonRightImage();
	afx_msg void OnClickedButtonCoor();
	afx_msg void OnClickedButtonLsq();
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClickedRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int searchWinsize;
};
