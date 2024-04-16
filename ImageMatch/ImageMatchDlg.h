
// ImageMatchDlg.h: 头文件
//

#pragma once

#include <string>
#include <fstream>
#include "feature_extraction.h"
#include "correlation_matcher.h"
#include "lsq_matcher.h"
#include "match_point_pair.h"
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
	CString image_1_path;
	// 右影像文件路径
	CString image_2_path;
	CString image_1_corner_path;
	CString image_2_corner_path;
	int corr_window_size;
	float corr_threshold;
//	CString result_corr_path;
	int lsq_window_size;
	float lsq_threshold;
	CString result_lsq_path;
	// 在两张影像上分别提取角点
	cv::Mat img_1, img_2;
	std::vector<cv::Point> corners_1, corners_2;
	std::vector<MatchPointPair> corrMatches;

//	afx_msg void OnClickedButtonFilePath();
	afx_msg void OnClickedButtonLeftImage();
	afx_msg void OnClickedButtonRightImage();
	afx_msg void OnClickedButtonLeftDetected();
	afx_msg void OnClickedButtonRightDetected();
	afx_msg void OnClickedButtonCoorResult();
	afx_msg void OnClickedButtonCoor();
	afx_msg void OnClickedButtonFeatureExtraction();
	afx_msg void OnClickedButtonLsqResult();
	afx_msg void OnClickedButtonLsq();
//	CString img_match_path;
	afx_msg void OnClickedButtonCoorResult2();
	CString image_match_path;
	CString result_corr_path;
};
