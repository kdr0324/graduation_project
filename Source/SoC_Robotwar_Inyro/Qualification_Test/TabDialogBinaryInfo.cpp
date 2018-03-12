// TabDialogBinaryInfo.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Qualification_Test.h"
#include "TabDialogBinaryInfo.h"
#include "afxdialogex.h"


// CTabDialogBinaryInfo 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTabDialogBinaryInfo, CDialogEx)

CTabDialogBinaryInfo::CTabDialogBinaryInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TAB_DIALOG_BINARY_INFO, pParent)	
	, m_n_sat_opt(0)
	, m_n_val_opt(0)
{
	for(int i = 0; i < SLIDER_NUM; i++)
		m_n_binary[i] = 0;
}

CTabDialogBinaryInfo::~CTabDialogBinaryInfo()
{
}

void CTabDialogBinaryInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TAB_DIALOG_BINARY_INFO_EDIT_HUE_LOW, m_n_binary[SLIDER_HUE_LOW]);
	DDX_Text(pDX, IDC_TAB_DIALOG_BINARY_INFO_EDIT_HUE_HIGH, m_n_binary[SLIDER_HUE_HIGH]);
	DDX_Control(pDX, IDC_TAB_DIALOG_BINARY_INFO_SLIDER_HUE_LOW, m_slider_binary[SLIDER_HUE_LOW]);
	DDX_Control(pDX, IDC_TAB_DIALOG_BINARY_INFO_SLIDER_HUE_HIGH, m_slider_binary[SLIDER_HUE_HIGH]);	
	DDX_Radio(pDX, IDC_TAB_DIALOG_BINARY_INFO_RADIO_SAT_NOT_USE, m_n_sat_opt);
	DDX_Control(pDX, IDC_TAB_DIALOG_BINARY_INFO_SLIDER_SAT, m_slider_binary[SLIDER_SATURATION]);
	DDX_Text(pDX, IDC_TAB_DIALOG_BINARY_INFO_EDIT_SAT, m_n_binary[SLIDER_SATURATION]);
	DDX_Radio(pDX, IDC_TAB_DIALOG_BINARY_INFO_RADIO_VAL_NOT_USE, m_n_val_opt);
	DDX_Control(pDX, IDC_TAB_DIALOG_BINARY_INFO_SLIDER_VAL, m_slider_binary[SLIDER_INTENSITY]);
	DDX_Text(pDX, IDC_TAB_DIALOG_BINARY_INFO_EDIT_VAL, m_n_binary[SLIDER_INTENSITY]);
}


BEGIN_MESSAGE_MAP(CTabDialogBinaryInfo, CDialogEx)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_TAB_DIALOG_BINARY_INFO_DEFAULT, &CTabDialogBinaryInfo::OnBnClickedTabDialogBinaryInfoDefault)
	ON_BN_CLICKED(IDC_TAB_DIALOG_BINARY_INFO_SAVE, &CTabDialogBinaryInfo::OnBnClickedTabDialogBinaryInfoSave)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_TAB_DIALOG_BINARY_INFO_RADIO_SAT_NOT_USE, IDC_TAB_DIALOG_BINARY_INFO_RADIO_SAT_MIN, OnClickedTabDialogBinaryInfoRadioSaturation)	
	ON_CONTROL_RANGE(BN_CLICKED, IDC_TAB_DIALOG_BINARY_INFO_RADIO_VAL_NOT_USE, IDC_TAB_DIALOG_BINARY_INFO_RADIO_VAL_MIN, OnClickedTabDialogBinaryInfoRadioIntensity)
END_MESSAGE_MAP()


// CTabDialogBinaryInfo 메시지 처리기입니다.


BOOL CTabDialogBinaryInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	
	return TRUE;  // return TRUE unless you set the focus to a control
								// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CTabDialogBinaryInfo::Init(CString path, int option[KINDS_OF_BINARY], int (*binary)[BINARY_OPTION_CNT])
{
	m_str_path = path;
	m_n_hue_center = option[BINARY_HUE];

	m_slider_binary[SLIDER_HUE_LOW].SetRange(m_n_hue_center - 90, m_n_hue_center);
	m_slider_binary[SLIDER_HUE_HIGH].SetRange(m_n_hue_center, m_n_hue_center + 90);
	m_slider_binary[SLIDER_SATURATION].SetRange(0, 255);
	m_slider_binary[SLIDER_INTENSITY].SetRange(0, 255);

	m_slider_binary[SLIDER_HUE_LOW].SetPos(binary[BINARY_HUE][BINARY_OPTION_MIN]);
	m_slider_binary[SLIDER_HUE_HIGH].SetPos(binary[BINARY_HUE][BINARY_OPTION_MAX]);
	m_n_binary[SLIDER_HUE_LOW] = (binary[BINARY_HUE][BINARY_OPTION_MIN] + 360) % 360;
	m_n_binary[SLIDER_HUE_HIGH] = (binary[BINARY_HUE][BINARY_OPTION_MAX] + 360) % 360;

	for(int i = 0; i < BINARY_OPTION_CNT; i++) {
		m_n_saturation[i] = binary[BINARY_SATURATION][i];
		m_n_intensity[i] = binary[BINARY_INTENSITY][i];
	}
	
	m_n_saturation[0] = m_n_intensity[0] = -1;
	m_n_sat_opt = option[BINARY_SATURATION];
	m_n_binary[SLIDER_SATURATION] = m_n_saturation[m_n_sat_opt];
	if(m_n_sat_opt) {
		m_slider_binary[SLIDER_SATURATION].SetPos(binary[BINARY_SATURATION][m_n_sat_opt]);
	} else {
		m_slider_binary[SLIDER_SATURATION].EnableWindow(FALSE);
	}

	m_n_val_opt = option[BINARY_INTENSITY];
	m_n_binary[SLIDER_INTENSITY] = m_n_intensity[m_n_val_opt];
	if(m_n_val_opt) {
		m_slider_binary[SLIDER_INTENSITY].SetPos(binary[BINARY_INTENSITY][m_n_val_opt]);
	} else {
		m_slider_binary[SLIDER_INTENSITY].EnableWindow(FALSE);
	}

	for(int i = 0; i < SLIDER_NUM; i++) {
		m_slider_binary[i].SetLineSize(1); // 키보드 커서키로 슬라이더를 움직일때의 증가 크기를 설정
		m_slider_binary[i].SetPageSize(10); // 키보드의 PgUp, PgDn키를 누르거나 마우스로 슬라이더의 몸동을 클릭시 움직일 크기 
	}

	CString str;
	str.Format(_T("%d"), (m_n_hue_center + 360) % 360);
	GetDlgItem(IDC_TAB_DIALOG_BINARY_INFO_EDIT_HUE_CENTER)->SetWindowText(str);
	str.Format(_T("%d"), (m_n_hue_center - 90 + 360) % 360);
	GetDlgItem(IDC_TAB_DIALOG_BINARY_INFO_EDIT_HUE_MIN)->SetWindowText(str);
	str.Format(_T("%d"), (m_n_hue_center + 90 + 360) % 360);
	GetDlgItem(IDC_TAB_DIALOG_BINARY_INFO_EDIT_HUE_MAX)->SetWindowText(str);
	str.Format(_T("%d"), 255);
	GetDlgItem(IDC_TAB_DIALOG_BINARY_INFO_EDIT_SAT_MAX)->SetWindowText(str);
	GetDlgItem(IDC_TAB_DIALOG_BINARY_INFO_EDIT_VAL_MAX)->SetWindowText(str);
	str.Format(_T("%d"), 0);
	GetDlgItem(IDC_TAB_DIALOG_BINARY_INFO_EDIT_SAT_MIN)->SetWindowText(str);
	GetDlgItem(IDC_TAB_DIALOG_BINARY_INFO_EDIT_VAL_MIN)->SetWindowText(str);

	UpdateData(false); // Control에 값을 내보내기 위해서 작성
}

BOOL CTabDialogBinaryInfo::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN) {   // 키보드가 눌렸을때 처리
		switch(pMsg->wParam) {
		case VK_ESCAPE: // ESC키를 눌렀을때..
		case VK_RETURN: // Enter키를 눌렀을때..
		// 방향키를 눌렀을때..
		case VK_LEFT:
		case VK_RIGHT:
		case VK_UP:
		case VK_DOWN:
		// TAB키를 눌렀을때
		case VK_TAB:
		// SPACE키를 눌렀을때
		case VK_SPACE:
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CTabDialogBinaryInfo::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CSliderCtrl *p_slider = NULL;
	int *p_binary = NULL;
	if(pScrollBar) {
#if defined(_OPENMP)
#pragma omp parallel for
#endif	
		for(int i = 0; i < SLIDER_NUM; i++) {
			if(pScrollBar == (CScrollBar *)&m_slider_binary[i]) {
				p_slider = &m_slider_binary[i];
				p_binary = &m_n_binary[i];
			}
		}
	}

	if(p_slider != NULL) {		
		*p_binary = (p_slider->GetPos() + 360) % 360;
	}

	UpdateData(false); // Edit Control에 값을 내보내기 위해서 작성
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CTabDialogBinaryInfo::OnBnClickedTabDialogBinaryInfoDefault()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_n_sat_opt = 0;
	m_n_val_opt = 0;
	for(int i = 0; i < BINARY_OPTION_CNT; i++) {
		m_n_saturation[i] = 100;
		m_n_intensity[i] = 100;
	}
	m_n_saturation[0] = m_n_intensity[0] = -1;

	m_n_binary[SLIDER_HUE_LOW] = m_n_hue_center - 40;
	m_n_binary[SLIDER_HUE_HIGH] = m_n_hue_center + 40;
	m_n_binary[SLIDER_SATURATION] = m_n_saturation[m_n_sat_opt];
	m_n_binary[SLIDER_INTENSITY] = m_n_intensity[m_n_val_opt];

	for(int i = 0; i < SLIDER_NUM; i++) {
		m_slider_binary[i].SetPos(m_n_binary[i]);		
	}
	m_n_binary[SLIDER_HUE_LOW] = (m_n_binary[SLIDER_HUE_LOW] + 360) % 360;
	m_n_binary[SLIDER_HUE_HIGH] = (m_n_binary[SLIDER_HUE_HIGH] + 360) % 360;
	m_slider_binary[SLIDER_SATURATION].EnableWindow(FALSE);
	m_slider_binary[SLIDER_INTENSITY].EnableWindow(FALSE);

	UpdateData(false); // Edit Control에 값을 내보내기 위해서 작성
}

void CTabDialogBinaryInfo::OnBnClickedTabDialogBinaryInfoSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	FILE *fp = fopen(m_str_path.GetString(), "wt");
	fprintf(fp, "%d\n", m_n_hue_center);
	if(m_n_binary[SLIDER_HUE_LOW] > m_n_binary[SLIDER_HUE_HIGH])
		fprintf(fp, "%d %d\n", m_n_binary[SLIDER_HUE_HIGH], m_n_binary[SLIDER_HUE_LOW] - 360);
	else
		fprintf(fp, "%d %d\n", m_n_binary[SLIDER_HUE_HIGH], m_n_binary[SLIDER_HUE_LOW]);
	fprintf(fp, "%d\n", m_n_sat_opt);
	fprintf(fp, "%d %d\n", m_n_saturation[BINARY_OPTION_MAX], m_n_saturation[BINARY_OPTION_MIN]);
	fprintf(fp, "%d\n", m_n_val_opt);
	fprintf(fp, "%d %d\n", m_n_intensity[BINARY_OPTION_MAX], m_n_intensity[BINARY_OPTION_MIN]);
	fclose(fp);	
}


void CTabDialogBinaryInfo::OnClickedTabDialogBinaryInfoRadioSaturation(UINT msg)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_n_saturation[m_n_sat_opt] = m_n_binary[SLIDER_SATURATION];
	m_n_intensity[m_n_val_opt] = m_n_binary[SLIDER_INTENSITY];

	UpdateData(true); // Control로 부터 값을 얻어옴

	m_n_binary[SLIDER_SATURATION] = m_n_saturation[m_n_sat_opt];
	m_slider_binary[SLIDER_SATURATION].SetPos(m_n_binary[SLIDER_SATURATION]);
	switch(m_n_sat_opt) {
	case 0:
		m_slider_binary[SLIDER_SATURATION].EnableWindow(FALSE);
		break;
	case 1:
	case 2:		
		m_slider_binary[SLIDER_SATURATION].EnableWindow(TRUE);
		break;
	}

	UpdateData(false);
}

void CTabDialogBinaryInfo::OnClickedTabDialogBinaryInfoRadioIntensity(UINT msg)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_n_intensity[m_n_val_opt] = m_n_binary[SLIDER_INTENSITY];

	UpdateData(true); // Control로 부터 값을 얻어옴

	m_n_binary[SLIDER_INTENSITY] = m_n_intensity[m_n_val_opt];
	m_slider_binary[SLIDER_INTENSITY].SetPos(m_n_binary[SLIDER_INTENSITY]);
	switch(m_n_val_opt) {
	case 0:
		m_slider_binary[SLIDER_INTENSITY].EnableWindow(FALSE);
		break;
	case 1:
	case 2:		
		m_slider_binary[SLIDER_INTENSITY].EnableWindow(TRUE);
		break;
	}

	UpdateData(false);
}
