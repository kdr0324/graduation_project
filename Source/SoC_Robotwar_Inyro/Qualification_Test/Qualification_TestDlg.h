
// Qualification_TestDlg.h : ��� ����
//

#pragma once

#include "afxwin.h"
#include "afxcmn.h"

#include "../MFC/VideoProcessing.h"
#include "../MFC/PictureControlDraw.h"
#include "TabDialogBinaryInfo.h"

#include "../Source/Tracking/Tracking.h"
#include "../Source/Recognition/Recognition.h"


enum {
	TRACKING = 0,
	RECOGNITION = 1
};

#define RESULT_IMAGE_CNT 4

// CQualification_TestDlg ��ȭ ����
class CQualification_TestDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CQualification_TestDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QUALIFICATION_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnBnClickedQtBtnSelect();
	int TrackingTabSetting();
	int RecognitionTabSetting();
	void SetTrackingTabInfo();
	void SetRecognitionTabInfo();
	void ChangeTrackingTab();
	void ChangeRecognitionTab();

public:
	BOOL is_init;

	CaptureCAM_VFW *capture;

	CStatic m_ctrl_camera;  // ��ķ������ ǥ�� �� ������
	CStatic m_ctrl_result[4];  // ��������� ǥ�� �� ������
	CPictureControlDraw m_pcd_camera;
	CPictureControlDraw m_pcd_result[4];

	int(*QualficationMain)(IMG_INFO *, void *);
	
	void *p_buf;
		
	CTabCtrl m_tab;
	int m_n_qt_type;
	afx_msg void OnTcnSelchangeQtOptionTab(NMHDR *pNMHDR, LRESULT *pResult);
	
	CStringArray str_tab_name_tracking;
	int m_n_tracking_index;
	TRACKINGINFO m_st_tracking;
	void TrackingGetValue();
	
	CStringArray str_tab_name_recognition;
	int m_n_recognition_index;
	RECOGNITIONINFO m_st_recognition;
	void RecognitionGetValue();

	CWnd* m_p_wnd_show;
	CTabDialogBinaryInfo *m_class_binary_info;	

};
