#if !defined(AFX_RESIZEDIALOG_H__68BEA6C1_89F9_11D2_9EDD_0080C7170C6D__INCLUDED_)
#define AFX_RESIZEDIALOG_H__68BEA6C1_89F9_11D2_9EDD_0080C7170C6D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ResizeDialog.h : header file
//
// GWL_USERDATA values to define how controls behave when the dialog is resized
// Use ::SetWindowLong(GWL_USERDATA,<one of these #defines) in the InitDialog routine
//  to set up this behavior.
#define RESIZE_DO_NOTHING					0x0000			// Control stays in same place
#define RESIZE_FOLLOW_BOTTOM				0x0001			// Control will move with the bottom of the dialog when resized
#define RESIZE_FOLLOW_RIGHT					0x0002			// Control moves with right side of dialog
#define RESIZE_FOLLOW_BOTTOMRIGHT			RESIZE_FOLLOW_BOTTOM|RESIZE_FOLLOW_RIGHT		// Control moves with right and bottom of dialog
#define RESIZE_ADJUST_RIGHT					0x0004			// Control will resize in accordance with right side of dialog
#define RESIZE_ADJUST_BOTTOM				0x0008			// Control will resize in accordnace with the bottom of the dialog
#define RESIZE_ADJUST_BOTTOMRIGHT			RESIZE_ADJUST_RIGHT|RESIZE_ADJUST_BOTTOM		// Control will resize with the bottom and right sides of dialog
#define RESIZE_FOLLOW_BOTTOM_ADJUST_RIGHT	RESIZE_FOLLOW_BOTTOM|RESIZE_ADJUST_RIGHT		// Control will move up/down with bottom but resize to the right
/////////////////////////////////////////////////////////////////////////////
// CResizeDialog dialog

class CResizeDialog : public CDialogEx
{
// Construction
public:
	CResizeDialog(int IDD, CWnd* pParent = NULL);   // standard constructor
	void SetSize();
	void SetResizeNoCaption();
	void SetResizeNoBorder();
	void SetEnableMenu();
		// Dialog Data
	//{{AFX_DATA(CResizeDialog)
//	enum { IDD = _UNKNOWN_RESOURCE_ID_ };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResizeDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int DialogWidth,DialogHeight;
	int OriginalDialogWidth,OriginalDialogHeight;
	int XOverhead;	// Captions, frames, etc..
	int YOverhead;	// sames as above, 
	BOOL Caption;
	BOOL Border;
	// Generated message map functions
	//{{AFX_MSG(CResizeDialog)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESIZEDIALOG_H__68BEA6C1_89F9_11D2_9EDD_0080C7170C6D__INCLUDED_)
