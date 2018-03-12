#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CTabDialogBinaryInfo 대화 상자입니다.

enum BINARY_INDEX {
	BINARY_HUE,	
	BINARY_SATURATION,
	BINARY_INTENSITY,
	KINDS_OF_BINARY
};

enum SLIDER_INDEX {
	SLIDER_HUE_LOW,
	SLIDER_HUE_HIGH,
	SLIDER_SATURATION,
	SLIDER_INTENSITY,
	SLIDER_NUM
};

enum BINARY_OPTION {
	BINARY_OPTION_NOT_USE,
	BINARY_OPTION_MAX,
	BINARY_OPTION_MIN,
	BINARY_OPTION_CNT
};

class CTabDialogBinaryInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CTabDialogBinaryInfo)

public:
	CTabDialogBinaryInfo(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTabDialogBinaryInfo();
	
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB_DIALOG_BINARY_INFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void Init(CString path, int option[KINDS_OF_BINARY], int(*binary)[BINARY_OPTION_CNT]);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClickedTabDialogBinaryInfoRadioSaturation(UINT msg);	
	afx_msg void OnClickedTabDialogBinaryInfoRadioIntensity(UINT msg);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

public:
	int m_n_sat_opt;
	int m_n_val_opt;

	CString m_str_path;
	CSliderCtrl m_slider_binary[SLIDER_NUM];
	
	int m_n_binary[SLIDER_NUM];
	int m_n_hue_center;
	int m_n_saturation[BINARY_OPTION_CNT];
	int m_n_intensity[BINARY_OPTION_CNT];

	afx_msg void OnBnClickedTabDialogBinaryInfoDefault();
	afx_msg void OnBnClickedTabDialogBinaryInfoSave();		
};
