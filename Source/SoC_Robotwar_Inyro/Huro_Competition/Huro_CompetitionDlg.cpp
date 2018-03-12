
// Huro_CompetitionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Huro_Competition.h"
#include "Huro_CompetitionDlg.h"
#include "afxdialogex.h"

#include "../Source/Common/Bitmap.h"
#include "../Source/Amazon/ImageConvert.h"

#include "../Source/Huro/AlignPosition.h"
#include "../Source/Huro/Barricade.h"
#include "../Source/Huro/RedStair.h"
#include "../Source/Huro/Hurdle.h"
#include "../Source/Huro/Stair.h"
#include "../Source/Huro/Golf.h"
#include "../Source/Huro/YellowHole.h"
#include "../Source/Huro/Mine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int imageSaveMode = 1;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CHuro_CompetitionDlg dialog



CHuro_CompetitionDlg::CHuro_CompetitionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HURO_COMPETITION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHuro_CompetitionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHuro_CompetitionDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ALIGN, &CHuro_CompetitionDlg::OnBnClickedButtonAlign)
	ON_BN_CLICKED(IDC_BUTTON_BARRICADE_UD, &CHuro_CompetitionDlg::OnBnClickedButtonBarricadeUd)
	ON_BN_CLICKED(IDC_BUTTON_RED_STAIR, &CHuro_CompetitionDlg::OnBnClickedButtonRedStair)
	ON_BN_CLICKED(IDC_BUTTON_RED_STAIR_ALIGN, &CHuro_CompetitionDlg::OnBnClickedButtonRedStairAlign)
	ON_BN_CLICKED(IDC_BUTTON_HURDLE, &CHuro_CompetitionDlg::OnBnClickedButtonHurdle)
	ON_BN_CLICKED(IDC_BUTTON_STAIR_UP, &CHuro_CompetitionDlg::OnBnClickedButtonStairUp)
	ON_BN_CLICKED(IDC_BUTTON_GREEN_BRIDGE, &CHuro_CompetitionDlg::OnBnClickedButtonGreenBridge)
	ON_BN_CLICKED(IDC_BUTTON_STAIR_DOWN, &CHuro_CompetitionDlg::OnBnClickedButtonStairDown)
	ON_BN_CLICKED(IDC_BUTTON_BALL_DETECT, &CHuro_CompetitionDlg::OnBnClickedButtonBallDetect)
	ON_BN_CLICKED(IDC_BUTTON_GOLF, &CHuro_CompetitionDlg::OnBnClickedButtonGolf)
	ON_BN_CLICKED(IDC_BUTTON_YELLOW_HOLE_UP, &CHuro_CompetitionDlg::OnBnClickedButtonYellowHoleUp)
	ON_BN_CLICKED(IDC_BUTTON_YELLOW_HOLE, &CHuro_CompetitionDlg::OnBnClickedButtonYellowHole)
	ON_BN_CLICKED(IDC_BUTTON_YELLOW_HOLE_DOWN, &CHuro_CompetitionDlg::OnBnClickedButtonYellowHoleDown)
	ON_BN_CLICKED(IDC_BUTTON_BARRICADE_SIDE, &CHuro_CompetitionDlg::OnBnClickedButtonBarricadeSide)
	ON_BN_CLICKED(IDC_BUTTON_MINE, &CHuro_CompetitionDlg::OnBnClickedButtonMine)
END_MESSAGE_MAP()


// CHuro_CompetitionDlg message handlers

BOOL CHuro_CompetitionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CommonMemoryAllocate();
	HuroMemoryAllocate();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CHuro_CompetitionDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CommonMemoryRelease();
	HuroMemoryRelease();

	return CDialogEx::DestroyWindow();
}

void CHuro_CompetitionDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHuro_CompetitionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHuro_CompetitionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CHuro_CompetitionDlg::OnBnClickedButtonAlign()
{
	// TODO: Add extra initialization here
	m_bWorking = m_ffFileFinder.FindFile("../Images/align/*.raw");
	fp_result = fopen("../Images/align.csv", "wt");
	fprintf(fp_result, "이미지 이름,기울기,y 절편,리턴 값");
	CreateDirectory("../Images/align/result", NULL);
	while (m_bWorking) {
		m_bWorking = m_ffFileFinder.FindNextFile();
		if (m_ffFileFinder.IsDirectory() || m_ffFileFinder.IsDots()) {
			continue;
		}

		m_strImagePath = m_ffFileFinder.GetFilePath();
		m_strImageNoExtension = m_ffFileFinder.GetRoot() + "result/" + m_ffFileFinder.GetFileTitle();
		fprintf(fp_result, "\n%s", m_ffFileFinder.GetFileTitle().GetString());
		ReleaseImageInfo(&original);
		original = OpenRawRGB565(m_strImagePath);
		AlignPosition(fp_result, m_strImageNoExtension.GetString());		
	}
	fclose(fp_result);
	AfxMessageBox("OK");
}


void CHuro_CompetitionDlg::OnBnClickedButtonBarricadeUd()
{
	// TODO: Add extra initialization here
	m_bWorking = m_ffFileFinder.FindFile("../Images/barrigate_ud/*.bmp");
	fp_result = fopen("../Images/barrigate_ud.csv", "wt");
	CreateDirectory("../Images/barrigate_ud/result", NULL);
	while (m_bWorking) {
		m_bWorking = m_ffFileFinder.FindNextFile();
		if (m_ffFileFinder.IsDirectory() || m_ffFileFinder.IsDots()) {
			continue;
		}

		m_strImagePath = m_ffFileFinder.GetFilePath();
		m_strImageNoExtension = m_ffFileFinder.GetRoot() + "result/" + m_ffFileFinder.GetFileTitle();
		m_imOpenedBitmap = OpenBitmap(m_strImagePath);
		RGB888toRGB565(m_imOpenedBitmap, original);

		fprintf(fp_result, "%s", m_ffFileFinder.GetFileTitle().GetString());
		BarricadeUp(fp_result, m_strImageNoExtension.GetString());
		ReleaseImageInfo(&m_imOpenedBitmap);
	}
	fclose(fp_result);
	AfxMessageBox("OK");
}


void CHuro_CompetitionDlg::OnBnClickedButtonRedStair()
{
	// TODO: Add extra initialization here
	m_bWorking = m_ffFileFinder.FindFile("../Images/red_stair/*.bmp");
	fp_result = fopen("../Images/red_stair.csv", "wt");
	fprintf(fp_result, "이미지 이름,theta,y절편");
	CreateDirectory("../Images/red_stair/result", NULL);
	while (m_bWorking) {
		m_bWorking = m_ffFileFinder.FindNextFile();
		if (m_ffFileFinder.IsDirectory() || m_ffFileFinder.IsDots()) {
			continue;
		}

		m_strImagePath = m_ffFileFinder.GetFilePath();
		m_strImageNoExtension = m_ffFileFinder.GetRoot() + "result/" + m_ffFileFinder.GetFileTitle();
		m_imOpenedBitmap = OpenBitmap(m_strImagePath);
		RGB888toRGB565(m_imOpenedBitmap, original);

		fprintf(fp_result, "\n%s", m_ffFileFinder.GetFileTitle().GetString());
		RedStair(fp_result, m_strImageNoExtension.GetString());
		ReleaseImageInfo(&m_imOpenedBitmap);
	}
	fclose(fp_result);
	AfxMessageBox("OK");
}


void CHuro_CompetitionDlg::OnBnClickedButtonRedStairAlign()
{
	// TODO: Add extra initialization here
	m_bWorking = m_ffFileFinder.FindFile("../Images/red_stair_align/*.bmp");
	fp_result = fopen("../Images/red_stair_align.csv", "wt");
	CreateDirectory("../Images/red_stair_align/result", NULL);
	while (m_bWorking) {
		m_bWorking = m_ffFileFinder.FindNextFile();
		if (m_ffFileFinder.IsDirectory() || m_ffFileFinder.IsDots()) {
			continue;
		}

		m_strImagePath = m_ffFileFinder.GetFilePath();
		m_strImageNoExtension = m_ffFileFinder.GetRoot() + "result/" + m_ffFileFinder.GetFileTitle();
		m_imOpenedBitmap = OpenBitmap(m_strImagePath);
		RGB888toRGB565(m_imOpenedBitmap, original);
		fprintf(fp_result, "%s", m_ffFileFinder.GetFileTitle().GetString());
		RedStairAlign(fp_result, m_strImageNoExtension.GetString());
		ReleaseImageInfo(&m_imOpenedBitmap);
	}
	fclose(fp_result);
	AfxMessageBox("OK");
}


void CHuro_CompetitionDlg::OnBnClickedButtonMine()
{
	// TODO: Add extra initialization here
	m_bWorking = m_ffFileFinder.FindFile("../Images/mine/*.bmp");
	fp_result = fopen("../Images/mine.csv", "wt");
	fprintf(fp_result, "이미지 이름,x좌표,y좌표,리턴 값\n");
	CreateDirectory("../Images/mine/result", NULL);
	while (m_bWorking) {
		m_bWorking = m_ffFileFinder.FindNextFile();
		if (m_ffFileFinder.IsDirectory() || m_ffFileFinder.IsDots()) {
			continue;
		}

		m_strImagePath = m_ffFileFinder.GetFilePath();
		m_strImageNoExtension = m_ffFileFinder.GetRoot() + "result/" + m_ffFileFinder.GetFileTitle();
		m_imOpenedBitmap = OpenBitmap(m_strImagePath);
		RGB888toRGB565(m_imOpenedBitmap, original);

		fprintf(fp_result, "%s", m_ffFileFinder.GetFileTitle().GetString());
		MineDetect(fp_result, m_strImageNoExtension.GetString());
		//MineAlign(fp_result, m_strImageNoExtension.GetString());
		ReleaseImageInfo(&m_imOpenedBitmap);
	}
	fclose(fp_result);
	AfxMessageBox("OK");
}


void CHuro_CompetitionDlg::OnBnClickedButtonHurdle()
{
	// TODO: Add extra initialization here
	m_bWorking = m_ffFileFinder.FindFile("../Images/hurdle/*.bmp");
	fp_result = fopen("../Images/hurdle.csv", "wt");
	CreateDirectory("../Images/hurdle/result", NULL);
	while (m_bWorking) {
		m_bWorking = m_ffFileFinder.FindNextFile();
		if (m_ffFileFinder.IsDirectory() || m_ffFileFinder.IsDots()) {
			continue;
		}

		m_strImagePath = m_ffFileFinder.GetFilePath();
		m_strImageNoExtension = m_ffFileFinder.GetRoot() + "result/" + m_ffFileFinder.GetFileTitle();
		m_imOpenedBitmap = OpenBitmap(m_strImagePath);
		RGB888toRGB565(m_imOpenedBitmap, original);

		fprintf(fp_result, "%s", m_ffFileFinder.GetFileTitle().GetString());
		Hurdle(fp_result, m_strImageNoExtension.GetString());
		ReleaseImageInfo(&m_imOpenedBitmap);
	}
	fclose(fp_result);
	AfxMessageBox("OK");
}


void CHuro_CompetitionDlg::OnBnClickedButtonStairUp()
{
	// TODO: Add extra initialization here
	m_bWorking = m_ffFileFinder.FindFile("../Images/stair_up/*.bmp");
	fp_result = fopen("../Images/stair_up.csv", "wt");
	fprintf(fp_result, "이미지 이름,bot_x,bot_y,반환값\n");
	CreateDirectory("../Images/stair_up/result", NULL);
	while (m_bWorking) {
		m_bWorking = m_ffFileFinder.FindNextFile();
		if (m_ffFileFinder.IsDirectory() || m_ffFileFinder.IsDots()) {
			continue;
		}

		m_strImagePath = m_ffFileFinder.GetFilePath();
		m_strImageNoExtension = m_ffFileFinder.GetRoot() + "result/" + m_ffFileFinder.GetFileTitle();
		m_imOpenedBitmap = OpenBitmap(m_strImagePath);
		RGB888toRGB565(m_imOpenedBitmap, original);
		fprintf(fp_result, "%s", m_ffFileFinder.GetFileTitle().GetString());
		StairUp(fp_result, m_strImageNoExtension.GetString());
		ReleaseImageInfo(&m_imOpenedBitmap);
	}
	fclose(fp_result);
	AfxMessageBox("OK");
}


void CHuro_CompetitionDlg::OnBnClickedButtonGreenBridge()
{
	// TODO: Add extra initialization here
	m_bWorking = m_ffFileFinder.FindFile("../Images/green_bridge/*.bmp");
	fp_result = fopen("../Images/green_bridge.csv", "wt");
	fprintf(fp_result, "이미지 이름,왼쪽 기울기,오른쪽 기울기,중심,반환값\n");
	CreateDirectory("../Images/green_bridge/result", NULL);
	while (m_bWorking) {
		m_bWorking = m_ffFileFinder.FindNextFile();
		if (m_ffFileFinder.IsDirectory() || m_ffFileFinder.IsDots()) {
			continue;
		}

		m_strImagePath = m_ffFileFinder.GetFilePath();
		m_strImageNoExtension = m_ffFileFinder.GetRoot() + "result/" + m_ffFileFinder.GetFileTitle();
		m_imOpenedBitmap = OpenBitmap(m_strImagePath);
		RGB888toRGB565(m_imOpenedBitmap, original);
		fprintf(fp_result, "%s", m_ffFileFinder.GetFileTitle().GetString());
		GreenBridge(fp_result, m_strImageNoExtension.GetString());
		ReleaseImageInfo(&m_imOpenedBitmap);
	}
	fclose(fp_result);
	AfxMessageBox("OK");
}


void CHuro_CompetitionDlg::OnBnClickedButtonStairDown()
{
	// TODO: Add extra initialization here
	m_bWorking = m_ffFileFinder.FindFile("../Images/stair_down/*.raw");
	fp_result = fopen("../Images/stair_down.csv", "wt");
	CreateDirectory("../Images/stair_down/result", NULL);
	while (m_bWorking) {
		m_bWorking = m_ffFileFinder.FindNextFile();
		if (m_ffFileFinder.IsDirectory() || m_ffFileFinder.IsDots()) {
			continue;
		}

		m_strImagePath = m_ffFileFinder.GetFilePath();
		m_strImageNoExtension = m_ffFileFinder.GetRoot() + "result/" + m_ffFileFinder.GetFileTitle();		
		ReleaseImageInfo(&original);
		original = OpenRawRGB565(m_strImagePath);		
		fprintf(fp_result, "\n%s", m_ffFileFinder.GetFileTitle().GetString());
		StairDown(fp_result, m_strImageNoExtension.GetString());
	}
	fclose(fp_result);
	AfxMessageBox("OK");
}


void CHuro_CompetitionDlg::OnBnClickedButtonBallDetect()
{
	// TODO: Add extra initialization here
	m_bWorking = m_ffFileFinder.FindFile("../Images/ball_detect/*.bmp");
	fp_result = fopen("../Images/ball_detect.csv", "wt");
	CreateDirectory("../Images/ball_detect/result", NULL);
	while (m_bWorking) {
		m_bWorking = m_ffFileFinder.FindNextFile();
		if (m_ffFileFinder.IsDirectory() || m_ffFileFinder.IsDots()) {
			continue;
		}

		m_strImagePath = m_ffFileFinder.GetFilePath();
		m_strImageNoExtension = m_ffFileFinder.GetRoot() + "result/" + m_ffFileFinder.GetFileTitle();
		m_imOpenedBitmap = OpenBitmap(m_strImagePath);
		RGB888toRGB565(m_imOpenedBitmap, original);
		fprintf(fp_result, "%s,x좌표,y좌표\n", m_ffFileFinder.GetFileTitle().GetString());
		BallDetect(fp_result, m_strImageNoExtension.GetString());
		ReleaseImageInfo(&m_imOpenedBitmap);
	}
	fclose(fp_result);
	AfxMessageBox("OK");
}


void CHuro_CompetitionDlg::OnBnClickedButtonGolf()
{
	// TODO: Add extra initialization here
	m_bWorking = m_ffFileFinder.FindFile("../Images/golf/*.bmp");
	fp_result = fopen("../Images/golf.csv", "wt");
	fprintf(fp_result, "이미지 이름,X 변화량,공 중심,리턴 값\n");
	CreateDirectory("../Images/golf/result", NULL);
	while (m_bWorking) {
		m_bWorking = m_ffFileFinder.FindNextFile();
		if (m_ffFileFinder.IsDirectory() || m_ffFileFinder.IsDots()) {
			continue;
		}

		m_strImagePath = m_ffFileFinder.GetFilePath();
		m_strImageNoExtension = m_ffFileFinder.GetRoot() + "result/" + m_ffFileFinder.GetFileTitle();
		m_imOpenedBitmap = OpenBitmap(m_strImagePath);
		RGB888toRGB565(m_imOpenedBitmap, original);
		
		fprintf(fp_result, "%s", m_ffFileFinder.GetFileTitle().GetString());
		Golf_B(fp_result, m_strImageNoExtension.GetString());
		Golf_H(fp_result, m_strImageNoExtension.GetString());
		ReleaseImageInfo(&m_imOpenedBitmap);
	}
	fclose(fp_result);
	AfxMessageBox("OK");
}


void CHuro_CompetitionDlg::OnBnClickedButtonYellowHoleUp()
{
	// TODO: Add extra initialization here
	m_bWorking = m_ffFileFinder.FindFile("../Images/yellow_hole_up/*.raw");
	fp_result = fopen("../Images/yellow_hole_up.csv", "wt");
	fprintf(fp_result, "이미지 이름,tan, Y절편, x중심\n");
	CreateDirectory("../Images/yellow_hole_up/result", NULL);
	while (m_bWorking) {
		m_bWorking = m_ffFileFinder.FindNextFile();
		if (m_ffFileFinder.IsDirectory() || m_ffFileFinder.IsDots()) {
			continue;
		}

		m_strImagePath = m_ffFileFinder.GetFilePath();
		m_strImageNoExtension = m_ffFileFinder.GetRoot() + "result/" + m_ffFileFinder.GetFileTitle();
		m_imOpenedBitmap = OpenBitmap(m_strImagePath);
		RGB888toRGB565(m_imOpenedBitmap, original);
		fprintf(fp_result, "%s", m_ffFileFinder.GetFileTitle().GetString());
		YellowHoleUp(fp_result, m_strImageNoExtension.GetString());
		ReleaseImageInfo(&m_imOpenedBitmap);
	}
	fclose(fp_result);
	AfxMessageBox("OK");
}


void CHuro_CompetitionDlg::OnBnClickedButtonYellowHole()
{
	// TODO: Add extra initialization here
	m_bWorking = m_ffFileFinder.FindFile("../Images/yellow_hole/*.bmp");
	fp_result = fopen("../Images/yellow_hole.csv", "wt");
	fprintf(fp_result, "이미지 이름,%end-start, tan, _y\n");
	CreateDirectory("../Images/yellow_hole/result", NULL);
	while (m_bWorking) {
		m_bWorking = m_ffFileFinder.FindNextFile();
		if (m_ffFileFinder.IsDirectory() || m_ffFileFinder.IsDots()) {
			continue;
		}

		m_strImagePath = m_ffFileFinder.GetFilePath();
		m_strImageNoExtension = m_ffFileFinder.GetRoot() + "result/" + m_ffFileFinder.GetFileTitle();
		m_imOpenedBitmap = OpenBitmap(m_strImagePath);
		RGB888toRGB565(m_imOpenedBitmap, original);
		fprintf(fp_result, "%s", m_ffFileFinder.GetFileTitle().GetString());
		YellowHoleAlign(fp_result, m_strImageNoExtension.GetString());
		ReleaseImageInfo(&m_imOpenedBitmap);
	}
	fclose(fp_result);
	AfxMessageBox("OK");
}


void CHuro_CompetitionDlg::OnBnClickedButtonYellowHoleDown()
{
	// TODO: Add extra initialization here
	m_bWorking = m_ffFileFinder.FindFile("../Images/yellow_hole_down/*.bmp");
	fp_result = fopen("../Images/yellow_hole_down.csv", "wt");
	fprintf(fp_result, "이미지 이름, theta, base_y\n");
	CreateDirectory("../Images/yellow_hole_down/result", NULL);
	while (m_bWorking) {
		m_bWorking = m_ffFileFinder.FindNextFile();
		if (m_ffFileFinder.IsDirectory() || m_ffFileFinder.IsDots()) {
			continue;
		}

		m_strImagePath = m_ffFileFinder.GetFilePath();
		m_strImageNoExtension = m_ffFileFinder.GetRoot() + "result/" + m_ffFileFinder.GetFileTitle();
		m_imOpenedBitmap = OpenBitmap(m_strImagePath);
		RGB888toRGB565(m_imOpenedBitmap, original);
		fprintf(fp_result, "%s", m_ffFileFinder.GetFileTitle().GetString());
		YellowHoleDown2(fp_result, m_strImageNoExtension.GetString());
		ReleaseImageInfo(&m_imOpenedBitmap);
	}
	fclose(fp_result);
	AfxMessageBox("OK");
}


void CHuro_CompetitionDlg::OnBnClickedButtonBarricadeSide()
{
	// TODO: Add extra initialization here
	m_bWorking = m_ffFileFinder.FindFile("../Images/barrigate_side/*.bmp");
	fp_result = fopen("../Images/barrigate_side.csv", "wt");
	CreateDirectory("../Images/barrigate_side/result", NULL);
	while (m_bWorking) {
		m_bWorking = m_ffFileFinder.FindNextFile();
		if (m_ffFileFinder.IsDirectory() || m_ffFileFinder.IsDots()) {
			continue;
		}

		m_strImagePath = m_ffFileFinder.GetFilePath();
		m_strImageNoExtension = m_ffFileFinder.GetRoot() + "result/" + m_ffFileFinder.GetFileTitle();
		m_imOpenedBitmap = OpenBitmap(m_strImagePath);
		RGB888toRGB565(m_imOpenedBitmap, original);
		fprintf(fp_result, "%s", m_ffFileFinder.GetFileTitle().GetString());
		BarricadeSide(fp_result, m_strImageNoExtension.GetString());
		ReleaseImageInfo(&m_imOpenedBitmap);
	}
	fclose(fp_result);
	AfxMessageBox("OK");
}
