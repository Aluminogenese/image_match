
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
	, left_image_path(_T(""))
	, right_image_path(_T(""))
	, moravec_window_size(0)
	, moravec_threshold(0)
	, lsq_window_size(0)
	, moravec_restrain_winSize(0)
	, feature_extraction_method(0)
	, harris_threshold(0)
	, corr_window_size(0)
	, corr_threshold(0)
	, lsq_threshold(0)
	, searchWinsize(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImageMatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, left_image_path);
	DDX_Text(pDX, IDC_EDIT4, right_image_path);
	DDX_Text(pDX, IDC_EDIT5, moravec_window_size);
	DDX_Text(pDX, IDC_EDIT6, moravec_threshold);
	DDX_Text(pDX, IDC_EDIT8, lsq_window_size);
	//  DDX_Text(pDX, IDC_EDIT9, lsq_threshold);
	DDX_Text(pDX, IDC_EDIT14, moravec_restrain_winSize);
	DDX_Radio(pDX, IDC_RADIO1, feature_extraction_method);
	DDX_Text(pDX, IDC_EDIT3, harris_threshold);
	DDX_Text(pDX, IDC_EDIT15, corr_window_size);
	DDX_Text(pDX, IDC_EDIT16, corr_threshold);
	DDX_Text(pDX, IDC_EDIT17, lsq_threshold);
	DDX_Text(pDX, IDC_EDIT18, searchWinsize);
}

BEGIN_MESSAGE_MAP(CImageMatchDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_BUTTON_LEFT_IMAGE, &CImageMatchDlg::OnClickedButtonLeftImage)
ON_BN_CLICKED(IDC_BUTTON_RIGHT_IMAGE, &CImageMatchDlg::OnClickedButtonRightImage)
ON_BN_CLICKED(IDC_BUTTON_COOR, &CImageMatchDlg::OnClickedButtonCoor)
ON_BN_CLICKED(IDC_BUTTON_LSQ, &CImageMatchDlg::OnClickedButtonLsq)
ON_WM_TIMER()
ON_BN_CLICKED(IDC_RADIO1, &CImageMatchDlg::OnClickedRadio1)
ON_COMMAND(IDC_RADIO2, &CImageMatchDlg::OnRadio2)
ON_COMMAND(IDC_RADIO3, &CImageMatchDlg::OnRadio3)
ON_WM_TIMER()
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
	static UINT indicators[] =
	{
		IDS_STRING_MESSAGE,
		IDS_STRING_PROGRESS
	};
	progress_bar.Create(this);
	progress_bar.SetIndicators(indicators, 2);
	GetClientRect(&m_rect);

	progress_bar.SetPaneInfo(0, IDS_STRING_MESSAGE, SBPS_POPOUT, m_rect.Width() / 9);
	progress_bar.SetPaneInfo(1, IDS_STRING_PROGRESS, SBPS_STRETCH, 0);

	progress_bar.GetStatusBarCtrl().SetBkColor(RGB(255, 255, 255));
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, AFX_IDW_CONTROLBAR_FIRST);

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

void CImageMatchDlg::OnClickedButtonLeftImage()
{
	// TODO: 在此添加控件通知处理程序代码
	//文件类型说明字符串
	static char BASED_CODE file[] = "JPEG 文件交换格式(*.jpg)|*.jpg|Windows 位图(*.bmp)|*.bmp|便捷式网络图形(*.png)|*.png|图形交换格式(*.gif)|*.gif|TIFF 格式(*.tif)|*.tif||";
	//文件对话框初始化
	CFileDialog SelectFile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, file, NULL);
	//弹出文件打开对话框
	SelectFile.DoModal();
	//得到所选文件路径
	CString FileName;
	FileName = SelectFile.GetPathName();
	//将得到的文件路径名赋给对话框成员变量m_sRawIn
	left_image_path = FileName;
	UpdateData(FALSE);
}


void CImageMatchDlg::OnClickedButtonRightImage()
{
	// TODO: 在此添加控件通知处理程序代码
	//文件类型说明字符串
	static char BASED_CODE file[] = "JPEG 文件交换格式(*.jpg)|*.jpg|Windows 位图(*.bmp)|*.bmp|便捷式网络图形(*.png)|*.png|图形交换格式(*.gif)|*.gif|TIFF 格式(*.tif)|*.tif||";
	//文件对话框初始化
	CFileDialog SelectFile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, file, NULL);
	//弹出文件打开对话框
	SelectFile.DoModal();
	//得到所选文件路径
	CString FileName;
	FileName = SelectFile.GetPathName();
	//将得到的文件路径名赋给对话框成员变量m_sRawIn
	right_image_path = FileName;
	UpdateData(FALSE);
}


void CImageMatchDlg::OnClickedButtonCoor()
{
	// TODO: 在此添加控件通知处理程序代码
	std::ofstream ofs;
	ofs.open("logfile.log");

	left_image = cv::imread(left_image_path.GetBuffer(0), cv::IMREAD_COLOR);
	right_image = cv::imread(right_image_path.GetBuffer(0), cv::IMREAD_COLOR);
	if (left_image.data == nullptr || right_image.data == nullptr)
	{
		AfxMessageBox("图像打开失败！");
		return;
	}

	CString m_status = "开始进行特征提取...";
	ofs << m_status << std::endl;
	progress_bar.SetPaneText(1, m_status);

	UpdateData(TRUE);

	std::vector<cv::Point> corners;
	double t = (double)cv::getTickCount();
	if (feature_extraction_method == 0) {
		if (harris_threshold != 0) {
			featurextraction::HarrisCornerDetect(left_image, corners, harris_threshold);
		}
		else {
			featurextraction::HarrisCornerDetect(left_image, corners);
		}
		featurextraction::MoravecCornerDetect(left_image, corners);
	}
	else if (feature_extraction_method == 1) {
		if (moravec_window_size != 0 && moravec_restrain_winSize != 0 && moravec_threshold != 0) {
			if (moravec_window_size % 2 == 0) {
				moravec_window_size += 1;
			}
			if (moravec_restrain_winSize % 2 == 0) {
				moravec_restrain_winSize += 1;
			}
			featurextraction::MoravecCornerDetect(left_image, corners, moravec_window_size, moravec_restrain_winSize, moravec_threshold);
		}
		else {
			featurextraction::MoravecCornerDetect(left_image, corners);
		}
	}
	else if (feature_extraction_method == 2) {
		featurextraction::SIFTCornerDetect(left_image, corners);
	}
	t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
	m_status.Format("特征提取完成！左影像共提取特征点数：%d，用时：%lfs，开始进行相关系数匹配...", corners.size(), t);
	ofs << m_status << std::endl;
	progress_bar.SetPaneText(1, m_status);

	t = (double)cv::getTickCount();
	if (corr_window_size != 0 && searchWinsize != 0 && corr_threshold != 0) {
		if (corr_window_size % 2 == 0) {
			corr_window_size += 1;
		}
		correlationmatch::match(corr_match_points, left_image, right_image, corners, corr_window_size, searchWinsize, corr_threshold);
	}
	else {
		correlationmatch::match(corr_match_points, left_image, right_image, corners);
	}
	t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
	m_status.Format("相关系数匹配完成！匹配点个数：%d，用时：%lfs", corr_match_points.size(), t);
	ofs << m_status << std::endl;
	ofs.close();
	progress_bar.SetPaneText(1, m_status);

	SaveFeatureDlg dlg;
	CString jpg_file_name, txt_file_name;
	if (IDOK == dlg.DoModal())
	{
		jpg_file_name = dlg.m_jpgFileName;
		txt_file_name = dlg.m_txtFileName;
		cv::Mat corr_match_resultImg;
		basematcher::draw_result_view(corr_match_resultImg, left_image, right_image, corr_match_points);
		if (jpg_file_name.GetLength() != 0) {
			cv::imwrite(jpg_file_name.GetBuffer(0), corr_match_resultImg);
		}
		if (txt_file_name.GetLength() != 0) {
			basematcher::save_match_points(txt_file_name.GetBuffer(0), corr_match_points);
		}
	}
}


void CImageMatchDlg::OnClickedButtonLsq()
{
	// TODO: 在此添加控件通知处理程序代码
	std::ofstream ofs;
	ofs.open("logfile.log", std::ios::app);
	m_status.Format("开始进行最小二乘匹配...");
	ofs << m_status << std::endl;
	progress_bar.SetPaneText(1, m_status);

	UpdateData(TRUE);

	std::vector<MatchPointPair>lsq_match_points;
	double t = (double)cv::getTickCount();
	if (lsq_window_size != 0 && lsq_threshold != 0) {
		if (lsq_window_size % 2 == 0) {
			lsq_window_size += 1;
		}
		lsqmatch::match(lsq_match_points, corr_match_points, left_image, right_image, lsq_window_size, lsq_threshold);
	}
	else
	{
		lsqmatch::match(lsq_match_points, corr_match_points, left_image, right_image);
	}
	t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
	m_status.Format("最小二乘匹配完成！匹配点个数：%d，用时：%lfs", lsq_match_points.size(), t);
	ofs << m_status << std::endl;
	ofs.close();
	progress_bar.SetPaneText(1, m_status);

	SaveFeatureDlg dlg;
	CString jpg_file_name, txt_file_name;
	if (IDOK == dlg.DoModal())
	{
		jpg_file_name = dlg.m_jpgFileName;
		txt_file_name = dlg.m_txtFileName;
		cv::Mat lsq_match_resultImg;
		basematcher::draw_result_view(lsq_match_resultImg, left_image, right_image, lsq_match_points);
		if (jpg_file_name.GetLength() != 0) {
			cv::imwrite(jpg_file_name.GetBuffer(0), lsq_match_resultImg);
		}
		if (txt_file_name.GetLength() != 0) {
			basematcher::save_match_points(txt_file_name.GetBuffer(0), lsq_match_points);
		}
	}
	corr_match_points.clear();
	corr_match_points.shrink_to_fit();
}


void CImageMatchDlg::OnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_STATIC4)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT3)->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT5)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT6)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT14)->ShowWindow(SW_HIDE);
}


void CImageMatchDlg::OnRadio2()
{
	// TODO: 在此添加命令处理程序代码
	GetDlgItem(IDC_STATIC4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT3)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC3)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT5)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT6)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT14)->ShowWindow(SW_SHOW);
}


void CImageMatchDlg::OnRadio3()
{
	// TODO: 在此添加命令处理程序代码
	GetDlgItem(IDC_STATIC4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT3)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT5)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT6)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT14)->ShowWindow(SW_HIDE);
}
