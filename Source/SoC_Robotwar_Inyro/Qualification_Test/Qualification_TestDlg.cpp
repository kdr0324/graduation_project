
// Qualification_TestDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "Qualification_Test.h"
#include "Qualification_TestDlg.h"
#include "afxdialogex.h"

#include "../Source/PC/Color.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch(uMsg) {
	case BFFM_INITIALIZED:      // ���� ���� ��ȭ���ڸ� �ʱ�ȭ �� ��, �ʱ� ��� ����
	{
		::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, (LPARAM)lpData);
	}
	break;

	// BROWSEINFO ����ü�� ulFlags ���� BIF_STATUSTEXT �� ������ ��� ȣ��
	// ��, BIF_NEWDIALOGSTYLE �� �����Ǿ� ���� ��� ȣ����� ����
	case BFFM_SELCHANGED:       // ����ڰ� ������ ������ ��� ��ȭ���ڿ� ���õ� ��� ǥ��
	{
		TCHAR szPath[MAX_PATH] = {0,};
		::SHGetPathFromIDList((LPCITEMIDLIST)lParam, szPath);
		::SendMessage(hWnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)szPath);
	}
	break;
	// BROWSEINFO ����ü�� ulFlags ���� BIF_VALIDATE �� ������ ��� ȣ��
	// BIF_EDITBOX �� ���� ������ ��츸 ȣ���
	case BFFM_VALIDATEFAILED:   // ������ ��Ʈ�ѿ��� ���� �̸��� �߸� �Է��� ��� ȣ��
	{
		::MessageBox(hWnd, _T("�ش� ������ ã�� �� �����ϴ�."), _T("����"),
								 MB_ICONERROR | MB_OK);
	}
	break;
	}

	return 0;
}

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CQualification_TestDlg ��ȭ ����

CQualification_TestDlg::CQualification_TestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_QUALIFICATION_TEST_DIALOG, pParent)
	, is_init(FALSE)
	, m_n_qt_type(0)
	, m_n_tracking_index(0)
	, m_n_recognition_index(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_p_wnd_show = NULL;
	str_tab_name_tracking.RemoveAll();
	str_tab_name_recognition.RemoveAll();
}

void CQualification_TestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_QT_PIC_CAMERA, m_ctrl_camera);
	DDX_Control(pDX, IDC_QT_PIC_RESULT_1, m_ctrl_result[0]);
	DDX_Control(pDX, IDC_QT_PIC_RESULT_2, m_ctrl_result[1]);
	DDX_Control(pDX, IDC_QT_PIC_RESULT_3, m_ctrl_result[2]);
	DDX_Control(pDX, IDC_QT_PIC_RESULT_4, m_ctrl_result[3]);
	DDX_Radio(pDX, IDC_QR_RADIO_TRACKING, m_n_qt_type);
	DDX_Control(pDX, IDC_QT_OPTION_TAB, m_tab);
}

BEGIN_MESSAGE_MAP(CQualification_TestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_QT_BTN_SELECT, &CQualification_TestDlg::OnBnClickedQtBtnSelect)
	ON_NOTIFY(TCN_SELCHANGE, IDC_QT_OPTION_TAB, &CQualification_TestDlg::OnTcnSelchangeQtOptionTab)
END_MESSAGE_MAP()


// CQualification_TestDlg �޽��� ó����

BOOL CQualification_TestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	capture = new CaptureCAM_VFW;
	if(!capture->open(0)) {
		AfxMessageBox("ī�޶� ������ ���� ���߽��ϴ�.");
		return FALSE;
	}

	CRect rect;

	m_ctrl_camera.GetClientRect(&rect);
	m_pcd_camera.setStaticFrame(m_ctrl_camera.GetDC(), rect);

	m_st_tracking.m_n_result_img_cnt = RESULT_IMAGE_CNT;
	m_st_tracking.m_img_result = (IMG_INFO ***)malloc(sizeof(IMG_INFO **) * RESULT_IMAGE_CNT);
		
	m_st_recognition.m_n_result_img_cnt = RESULT_IMAGE_CNT;
	m_st_recognition.m_img_result = (IMG_INFO ***)malloc(sizeof(IMG_INFO **) * RESULT_IMAGE_CNT);

	for(int i = 0; i < RESULT_IMAGE_CNT; i++) {
		m_ctrl_result[i].GetClientRect(&rect);
		m_pcd_result[i].setStaticFrame(m_ctrl_result[i].GetDC(), rect);
		m_st_tracking.m_img_result[i] = &m_pcd_result[i].m_image;
		m_st_recognition.m_img_result[i] = &m_pcd_result[i].m_image;
	}	

	MallocColorLookUpTable();
	QualficationMain = NULL;
	// Tracking ���� ����
	// tab�̸� ����
	TrackingTabSetting();

	m_st_tracking.mutex.create(m_st_tracking.mutexName, 1, 1);
	
	// tab�̸� ����
	str_tab_name_recognition.Add("Object1");
	str_tab_name_recognition.Add("Object2");
	str_tab_name_recognition.Add("Object3");

	is_init = TRUE;
	SetTimer(1, 1000 / 30, NULL);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CQualification_TestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CQualification_TestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CQualification_TestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CQualification_TestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	// ĸ�ı� �޸� ����
	delete capture;

	delete[] m_class_binary_info;
	

	// �̹��� �޸� ����
	ReleaseImageInfo(&m_pcd_camera.m_image);
	for(int i = 0; i < RESULT_IMAGE_CNT; i++) {
		ReleaseImageInfo(&m_pcd_result[i].m_image);
	}

	free(m_st_tracking.m_img_result);
	free(m_st_recognition.m_img_result);
	free(m_st_tracking.m_st_roi);
	m_st_tracking.mutex.close();

	// LookUpTable �޸� ����
	ReleaseColorLookUpTable();
}

BOOL CQualification_TestDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if(pMsg->message == WM_KEYDOWN) {   // Ű���尡 �������� ó��
		switch(pMsg->wParam) {
		case VK_ESCAPE: // ESCŰ�� ��������..
		case VK_RETURN: // EnterŰ�� ��������..
		// ����Ű�� ��������
		case VK_LEFT:
		case VK_RIGHT:
		case VK_UP:
		case VK_DOWN:
		// TABŰ�� ��������
		case VK_TAB:
		// SPACEŰ�� ��������
		case VK_SPACE:
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CQualification_TestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if(!is_init)
		goto END;

	if(m_n_qt_type == TRACKING)
		TrackingGetValue();
	else
		RecognitionGetValue();

	// ������ �޸𸮰� �Ҵ�Ǿ� �ִ��� Ȯ���ϴ� �۾�
	// �̹��� �޸� ����
	ReleaseImageInfo(&m_pcd_camera.m_image);
#if defined(_OPENMP)
#pragma omp parallel for
#endif	
	for(int i = 0; i < RESULT_IMAGE_CNT; i++) {
		ReleaseImageInfo(&m_pcd_result[i].m_image);
	}

	// ���� ���� �� �޸� ������ ������ �߻��Ͽ� ���� ����� ����
	m_pcd_camera.m_image = CloneImageInfo(capture->queryFrame(), D_U8);

	// �� �׽�Ʈ ���� �Լ� ȣ��
	//QualficationMain(m_pcd_camera.m_image, (void *)struct);
	if(QualficationMain)
		QualficationMain(m_pcd_camera.m_image, p_buf);

	m_pcd_camera.DrawToHDC();
	for(int i = 0; i < RESULT_IMAGE_CNT; i++) {
		m_pcd_result[i].DrawToHDC();
	}

END:

	CDialogEx::OnTimer(nIDEvent);
}

void CQualification_TestDlg::OnBnClickedQtBtnSelect()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_tab.DeleteAllItems();

	switch(m_n_qt_type) {
	case TRACKING:
		SetTrackingTabInfo();
		QualficationMain = TrackingMain;
		p_buf = &m_st_tracking;
		break;
	case RECOGNITION:
		SetRecognitionTabInfo();
		QualficationMain = RecognitionMain;
		p_buf = &m_st_recognition;
		break;
	default:
		break;
	}
}

int CQualification_TestDlg::TrackingTabSetting()
{
	CString str_cur_dir, str_tracking_dir;
	GetCurrentDirectory(MAX_PATH, str_cur_dir.GetBufferSetLength(MAX_PATH));

	ITEMIDLIST *pildBrowse;

	char pathName[MAX_PATH];

	LPITEMIDLIST pidl = NULL;
	BROWSEINFO bInfo;
	memset(&bInfo, 0, sizeof(BROWSEINFO));

	bInfo.pidlRoot = pidl;
	bInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT | BIF_VALIDATE;
	bInfo.lpfn = BrowseCallbackProc;

	// �ʱ� ��� �����ϴ� ��
	bInfo.lParam = (LPARAM)str_cur_dir.GetString();

	bInfo.lpszTitle = _T("�˻��� ���丮�� ���ÿ�.");
	pildBrowse = ::SHBrowseForFolder(&bInfo);
	if(pildBrowse != NULL) {
		SHGetPathFromIDList(pildBrowse, pathName);
		str_tracking_dir = pathName;
	} else {
		return -1;
	}

	CFileFind ff_tracking_info;
	CStringArray path;
	BOOL b_working;
	b_working = ff_tracking_info.FindFile(str_tracking_dir + "\\*.txt");
	while(b_working) {
		b_working = ff_tracking_info.FindNextFile();
		str_tab_name_tracking.Add(ff_tracking_info.GetFileTitle());
		path.Add(ff_tracking_info.GetFilePath());
	}
	ff_tracking_info.Close();

	// tab ����
	int binary[KINDS_OF_BINARY][BINARY_OPTION_CNT] = {0};
	int option[KINDS_OF_BINARY] = {0};
	m_class_binary_info = new CTabDialogBinaryInfo[str_tab_name_tracking.GetSize()];
	for(int i = 0; i < str_tab_name_tracking.GetSize(); i++) {
		m_class_binary_info[i].Create(IDD_TAB_DIALOG_BINARY_INFO, &m_tab);
		FILE *fp = fopen(path[i], "rt");
		fscanf(fp, "%d", &option[BINARY_HUE]);
		fscanf(fp, "%d %d", &binary[BINARY_HUE][BINARY_OPTION_MAX], &binary[BINARY_HUE][BINARY_OPTION_MIN]);
		fscanf(fp, "%d", &option[BINARY_SATURATION]);
		fscanf(fp, "%d %d", &binary[BINARY_SATURATION][BINARY_OPTION_MAX], &binary[BINARY_SATURATION][BINARY_OPTION_MIN]);
		fscanf(fp, "%d", &option[BINARY_INTENSITY]);
		fscanf(fp, "%d %d", &binary[BINARY_INTENSITY][BINARY_OPTION_MAX], &binary[BINARY_INTENSITY][BINARY_OPTION_MIN]);
		fclose(fp);
		m_class_binary_info[i].Init(path[i], option, binary);
	}

	m_st_tracking.m_n_tracking_cnt = str_tab_name_tracking.GetSize();
	m_st_tracking.m_st_hsi = (HSIVALUE *)malloc(sizeof(HSIVALUE) * m_st_tracking.m_n_tracking_cnt);
	m_st_tracking.m_st_roi = (OBJECT *)malloc(sizeof(OBJECT) *  m_st_tracking.m_n_tracking_cnt);
	m_st_tracking.m_n_is_tracking = 0;
	TrackingGetValue();

	return 1;
}

int CQualification_TestDlg::RecognitionTabSetting()
{
	return 1;
}

void CQualification_TestDlg::SetTrackingTabInfo()
{
	for(int i = 0; i < str_tab_name_tracking.GetSize(); i++)
		m_tab.InsertItem(i, str_tab_name_tracking[i]);

	CRect rect;
	m_tab.GetClientRect(&rect);
	m_tab.AdjustRect(FALSE, rect);
	m_tab.SetCurSel(m_n_tracking_index);
	m_class_binary_info[m_n_tracking_index].MoveWindow(rect, TRUE);
	m_class_binary_info[m_n_tracking_index].ShowWindow(SW_SHOW);
	m_p_wnd_show = &m_class_binary_info[m_n_tracking_index];
}

void CQualification_TestDlg::TrackingGetValue()
{
#if defined(_OPENMP)
#pragma omp parallel for
#endif
	for(int i = 0; i < str_tab_name_tracking.GetSize(); i++) {
		m_st_tracking.m_st_hsi[i].h.min = m_class_binary_info[i].m_n_binary[SLIDER_HUE_LOW];
		m_st_tracking.m_st_hsi[i].h.max = m_class_binary_info[i].m_n_binary[SLIDER_HUE_HIGH];
		switch(m_class_binary_info[i].m_n_sat_opt) {
		case BINARY_OPTION_NOT_USE:
			m_st_tracking.m_st_hsi[i].s.min = 0;
			m_st_tracking.m_st_hsi[i].s.max = 255;
			break;
		case BINARY_OPTION_MAX:
			m_st_tracking.m_st_hsi[i].s.min = 0;
			m_st_tracking.m_st_hsi[i].s.max = m_class_binary_info[i].m_n_binary[SLIDER_SATURATION];
			break;
		case BINARY_OPTION_MIN:
			m_st_tracking.m_st_hsi[i].s.min = m_class_binary_info[i].m_n_binary[SLIDER_SATURATION];
			m_st_tracking.m_st_hsi[i].s.max = 255;
			break;
		}

		switch(m_class_binary_info[i].m_n_val_opt) {
		case BINARY_OPTION_NOT_USE:
			m_st_tracking.m_st_hsi[i].i.min = 0;
			m_st_tracking.m_st_hsi[i].i.max = 255;
			break;
		case BINARY_OPTION_MAX:
			m_st_tracking.m_st_hsi[i].i.min = 0;
			m_st_tracking.m_st_hsi[i].i.max = m_class_binary_info[i].m_n_binary[SLIDER_INTENSITY];
			break;
		case BINARY_OPTION_MIN:
			m_st_tracking.m_st_hsi[i].i.min = m_class_binary_info[i].m_n_binary[SLIDER_INTENSITY];
			m_st_tracking.m_st_hsi[i].i.max = 255;
			break;
		}
	}
}

void CQualification_TestDlg::SetRecognitionTabInfo()
{
	for(int i = 0; i < str_tab_name_recognition.GetSize(); i++)
		m_tab.InsertItem(i, str_tab_name_recognition[i]);

	CRect rect;
	m_tab.GetClientRect(&rect);
	m_tab.AdjustRect(FALSE, rect);
	m_tab.SetCurSel(m_n_recognition_index);
	/*m_class_counterfeit_info.MoveWindow(rect, TRUE);
	m_class_counterfeit_info.ShowWindow(SW_SHOW);
	m_p_wnd_show = &m_class_counterfeit_info;*/
}

void CQualification_TestDlg::RecognitionGetValue()
{
	
}

void CQualification_TestDlg::OnTcnSelchangeQtOptionTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(m_p_wnd_show != NULL) {
		m_p_wnd_show->ShowWindow(SW_HIDE);
		m_p_wnd_show = NULL;
	}

	switch(m_n_qt_type) {
	case TRACKING:
		ChangeTrackingTab();
		break;
	case RECOGNITION:
		ChangeRecognitionTab();
		break;
	default:
		break;
	}

	*pResult = 0;
}

void CQualification_TestDlg::ChangeTrackingTab()
{
	CRect rect;
	m_tab.GetClientRect(&rect);
	m_tab.AdjustRect(FALSE, rect);

	m_n_tracking_index = m_tab.GetCurSel();
	m_class_binary_info[m_n_tracking_index].MoveWindow(rect, TRUE);
	m_class_binary_info[m_n_tracking_index].ShowWindow(SW_SHOW);
	m_p_wnd_show = &m_class_binary_info[m_n_tracking_index];
}

void CQualification_TestDlg::ChangeRecognitionTab()
{
	CRect rect;
	m_tab.GetClientRect(&rect);
	m_tab.AdjustRect(FALSE, rect);

	m_n_recognition_index = m_tab.GetCurSel();
	/*m_class_binary_info[m_n_recognition_index].MoveWindow(rect, TRUE);
	m_class_binary_info[m_n_recognition_index].ShowWindow(SW_SHOW);
	m_p_wnd_show = &m_class_binary_info[m_n_recognition_index];*/
}
 