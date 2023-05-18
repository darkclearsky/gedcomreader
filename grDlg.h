
// grDlg.h : header file
//

#pragma once
#include "GedData.h"
#include "CChart.h"
#include "ResizeDialog.h"

#define MAX_GEDCOMFILENAMESIZE 132

// CgrDlg dialog
// class CgrDlg : public CDialogEx
class CgrDlg : public CResizeDialog
{
// Construction
public:
	CgrDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	int maleIconIndex, femaleIconIndex, genericIconIndex;
	CGedData Ged;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
private:
	int loadGEDFile(wchar_t *Filename);
	int buildTree();
public:
	int LogMessage(CString& Message);
protected:
	CListBox m_CtrlListLog;
	CImageList imageList;
public:
	CTreeCtrl m_CtrlTree;
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	CEdit m_CtrlTextData;
	afx_msg void OnEnChangeEditdata();
	CTabCtrl m_CtrlTab;
	CStatic m_CtrlDraw;
	CChart m_CtrlChart;
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
};
