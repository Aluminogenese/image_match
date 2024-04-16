
// ImageMatchDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "ImageMatch.h"
#include "ImageMatchDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CImageMatchDlg 对话框



CImageMatchDlg::CImageMatchDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMAGEMATCH_DIALOG, pParent)
	, image_1_path(_T(""))
	, image_2_path(_T(""))
	, image_1_corner_path(_T(""))
	, image_2_corner_path(_T(""))
	, corr_window_size(0)
	, corr_threshold(0)
	, result_corr_path(_T(""))
	, lsq_window_size(0)
	, lsq_threshold(0)
	, result_lsq_path(_T(""))
	, image_match_path(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImageMatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, image_1_path);
	DDX_Text(pDX, IDC_EDIT4, image_2_path);
	DDX_Text(pDX, IDC_EDIT2, image_1_corner_path);
	DDX_Text(pDX, IDC_EDIT3, image_2_corner_path);
	DDX_Text(pDX, IDC_EDIT5, corr_window_size);
	DDX_Text(pDX, IDC_EDIT6, corr_threshold);
	//  DDX_Text(pDX, IDC_EDIT7, result_corr_path);
	DDX_Text(pDX, IDC_EDIT8, lsq_window_size);
	DDX_Text(pDX, IDC_EDIT9, lsq_threshold);
	DDX_Text(pDX, IDC_EDIT10, result_lsq_path);
	//  DDX_Text(pDX, IDC_EDIT11, img_match_path);
	DDX_Text(pDX, IDC_EDIT7, image_match_path);
	DDX_Text(pDX, IDC_EDIT11, result_corr_path);
}

BEGIN_MESSAGE_MAP(CImageMatchDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_BN_CLICKED(IDC_BUTTON_FILE_PATH, &CImageMatchDlg::OnClickedButtonFilePath)
ON_BN_CLICKED(IDC_BUTTON_LEFT_IMAGE, &CImageMatchDlg::OnClickedButtonLeftImage)
ON_BN_CLICKED(IDC_BUTTON_RIGHT_IMAGE, &CImageMatchDlg::OnClickedButtonRightImage)
ON_BN_CLICKED(IDC_BUTTON_LEFT_DETECTED, &CImageMatchDlg::OnClickedButtonLeftDetected)
ON_BN_CLICKED(IDC_BUTTON_RIGHT_DETECTED, &CImageMatchDlg::OnClickedButtonRightDetected)
ON_BN_CLICKED(IDC_BUTTON_COOR_RESULT, &CImageMatchDlg::OnClickedButtonCoorResult)
ON_BN_CLICKED(IDC_BUTTON_COOR, &CImageMatchDlg::OnClickedButtonCoor)
ON_BN_CLICKED(IDC_BUTTON_FEATURE_EXTRACTION, &CImageMatchDlg::OnClickedButtonFeatureExtraction)
ON_BN_CLICKED(IDC_BUTTON_LSQ_RESULT, &CImageMatchDlg::OnClickedButtonLsqResult)
ON_BN_CLICKED(IDC_BUTTON_LSQ, &CImageMatchDlg::OnClickedButtonLsq)
ON_BN_CLICKED(IDC_BUTTON_COOR_RESULT2, &CImageMatchDlg::OnClickedButtonCoorResult2)
END_MESSAGE_MAP()


// CImageMatchDlg 消息处理程序

BOOL CImageMatchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CImageMatchDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CImageMatchDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CImageMatchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//void CImageMatchDlg::OnClickedButtonFilePath()
//{
	// TODO: 在此添加控件通知处理程序代码
			//文件类型说明字符串
//	static char BASED_CODE file[] = "所有格式(*.*)|*.*";
	//文件对话框初始化
//	CFileDialog SelectFile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, file, NULL);
	//弹出文件打开对话框
//	SelectFile.DoModal();
	//得到所选文件路径
//	CString FileName;
//	FileName = SelectFile.GetPathName();
	//将得到的文件路径名赋给对话框成员变量m_sRawIn
//	image_1_path = FileName;
//	UpdateData(FALSE);
//}


void CImageMatchDlg::OnClickedButtonLeftImage()
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
	image_1_path = FileName;
	UpdateData(FALSE);

}


void CImageMatchDlg::OnClickedButtonRightImage()
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
	image_2_path = FileName;
	UpdateData(FALSE);
}


void CImageMatchDlg::OnClickedButtonLeftDetected()
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
	image_1_corner_path = FileName;
	UpdateData(FALSE);
}


void CImageMatchDlg::OnClickedButtonRightDetected()
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
	image_2_corner_path = FileName;
	UpdateData(FALSE);

}


void CImageMatchDlg::OnClickedButtonCoorResult()
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
	image_match_path = FileName;
	UpdateData(FALSE);

}
void CImageMatchDlg::OnClickedButtonCoorResult2()
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
	result_corr_path = FileName;
	UpdateData(FALSE);

}


void CImageMatchDlg::OnClickedButtonFeatureExtraction()
{
	// TODO: 在此添加控件通知处理程序代码
	img_1 = cv::imread(image_1_path.GetBuffer(0), cv::IMREAD_COLOR);
	img_2 = cv::imread(image_2_path.GetBuffer(0), cv::IMREAD_COLOR);
	if (img_1.data == nullptr || img_2.data == nullptr)
	{
		std::cout << "图像打开失败！" << std::endl;
	}

	int corner_threshold = 700; // Moravec角点检测阈值

	 //featurextraction::HarrisCornerDetect(img_1,corners_1,120);
	 //featurextraction::HarrisCornerDetect(img_2, corners_2,120);
	double t = (double)cv::getTickCount();
	featurextraction::MoravecCornerDetect(img_1, corners_1);
	featurextraction::MoravecCornerDetect(img_2, corners_2);

	t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
	std::cout << " Sequential implementation: " << t << "s" << std::endl;

	// corners_1 = photogrammetry::SIFTCornerDetect(img_1);
	// corners_2 = photogrammetry::SIFTCornerDetect(img_2);"
	MessageBox("特征提取完成,");
		// 显示角点
	cv::Mat img_1_corner, img_2_corner;
	featurextraction::DrawCorners(img_1, img_1_corner, corners_1);
	featurextraction::DrawCorners(img_2, img_2_corner, corners_2);
	cv::imwrite(image_1_corner_path.GetBuffer(0), img_1_corner);
	cv::imwrite(image_2_corner_path.GetBuffer(0), img_2_corner);
	cv::imshow("img_1 角点检测后图像", img_1_corner);
	cv::imshow("img_2 角点检测后图像", img_2_corner);
	cv::waitKey(0);

}

void CImageMatchDlg::OnClickedButtonCoor()
{
	// TODO: 在此添加控件通知处理程序代码
		// 进行相关系数匹配
	std::cout << "开始相关系数匹配：" << std::endl;
	CorrelationMatcher corrMatcher;
	corrMatcher.setWindowSize(corr_window_size);
	corrMatcher.setThreshold(corr_threshold);


	corrMatcher.match(img_1, img_2, corners_1, corrMatches);
	// corrMatcher.matchImproved(img_1, img_2, corners_1, corners_2, corrMatches);

	std::cout << "相关系数匹配窗口大小：" << corr_window_size << "\t阈值：" << corr_threshold << std::endl;
	std::cout << "相关系数匹配用时：" << time << "秒" << std::endl;
	std::cout << "相关系数匹配到同名点：" << corrMatches.size() << std::endl;
	std::cout << std::endl;

	std::ofstream ofs;
	ofs.open(result_corr_path);
	for (auto& match : corrMatches)
	{
		ofs << "srcX: " << match.srcPt.x
			<< "\tsrcY: " << match.srcPt.y
			<< "\tdstX: " << match.dstPt.x
			<< "\tdstY: " << match.dstPt.y
			<< "\tidx: " << match.dist << std::endl;
	}
	ofs.close();

	// 显示匹配结果
	cv::Mat img_match;
	corrMatcher.drawMatches(img_1, img_2, img_match, corrMatches);
	cv::imwrite(image_match_path.GetBuffer(0), img_match);
	cv::imshow("匹配结果：", img_match);
	cv::waitKey(0);

}

void CImageMatchDlg::OnClickedButtonLsqResult()
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
	result_lsq_path = FileName;
	UpdateData(FALSE);

}


void CImageMatchDlg::OnClickedButtonLsq()
{
	// TODO: 在此添加控件通知处理程序代码
		// 在相关系数匹配的基础上进行最小二乘匹配
	LsqMatcher lsqMatcher;
	lsqMatcher.setWindowSize(lsq_window_size);
	lsqMatcher.setThreshold(lsq_threshold);
	std::vector<MatchPointPair> lsqMatches;

	for (auto match : corrMatches)
	{
		if (lsqMatcher.subPixelMatch(img_1, img_2, match))
			lsqMatches.push_back(match);
	}

	std::cout << "最小二乘匹配窗口大小：" << lsq_window_size << "\t阈值：" << lsq_threshold << std::endl;
	std::cout << "最小二乘匹配用时：" << time << "秒" << std::endl;
	std::cout << "最小二乘匹配到同名点：" << lsqMatches.size() << std::endl;
	std::cout << std::endl;

	std::ofstream ofs;
	ofs.open(result_lsq_path);
	for (auto match : lsqMatches)
	{
		ofs << "srcX: " << match.srcPt.x
			<< "\tsrcY: " << match.srcPt.y
			<< "\tdstX: " << match.dstPt.x
			<< "\tdstY: " << match.dstPt.y
			<< "\tidx: " << match.dist << std::endl;
	}
	ofs.close();
}


