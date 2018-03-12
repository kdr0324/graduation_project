
// Huro_CompetitionDlg.h : header file
//

#pragma once

#include "../Source/Amazon/Memory.h"

// CHuro_CompetitionDlg dialog
class CHuro_CompetitionDlg : public CDialogEx
{
// Construction
public:
	CHuro_CompetitionDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HURO_COMPETITION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAlign();
	afx_msg void OnBnClickedButtonBarricadeUd();
	afx_msg void OnBnClickedButtonRedStair();
	afx_msg void OnBnClickedButtonRedStairAlign();
	afx_msg void OnBnClickedButtonMine();
	afx_msg void OnBnClickedButtonHurdle();
	afx_msg void OnBnClickedButtonStairUp();
	afx_msg void OnBnClickedButtonGreenBridge();
	afx_msg void OnBnClickedButtonStairDown();
	afx_msg void OnBnClickedButtonBallDetect();
	afx_msg void OnBnClickedButtonGolf();
	afx_msg void OnBnClickedButtonYellowHoleUp();
	afx_msg void OnBnClickedButtonYellowHole();
	afx_msg void OnBnClickedButtonYellowHoleDown();
	afx_msg void OnBnClickedButtonBarricadeSide();	

public:
	CFileFind m_ffFileFinder;
	BOOL m_bWorking;
	CString m_strImagePath;
	CString m_strImageNoExtension;
	IMG_INFO *m_imOpenedBitmap;
	FILE *fp_result;
};
