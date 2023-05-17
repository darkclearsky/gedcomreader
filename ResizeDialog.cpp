// ResizeDialog.cpp : implementation file
//
// #include "stdafx.h"
#include "pch.h"
#include "framework.h"
#include "ResizeDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResizeDialog dialog


CResizeDialog::CResizeDialog(int IDD, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD, pParent)
{
	//{{AFX_DATA_INIT(CResizeDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	OriginalDialogWidth=0;
	OriginalDialogHeight=0;
	Caption=TRUE;
	Border=TRUE;
}


void CResizeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResizeDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CResizeDialog, CDialogEx)
	//{{AFX_MSG_MAP(CResizeDialog)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CResizeDialog::SetSize()
{
	RECT DialogRC;
	GetWindowRect(&DialogRC);
	DialogWidth=DialogRC.right-DialogRC.left;
	DialogHeight=DialogRC.bottom-DialogRC.top;

	if(OriginalDialogHeight==0)
	{
		XOverhead = GetSystemMetrics(SM_CXPADDEDBORDER) + GetSystemMetrics(SM_CXDLGFRAME) + GetSystemMetrics(SM_CXBORDER);
		YOverhead = GetSystemMetrics(SM_CXPADDEDBORDER) + GetSystemMetrics(SM_CYDLGFRAME) + GetSystemMetrics(SM_CYBORDER) + GetSystemMetrics(SM_CYCAPTION);
		OriginalDialogHeight=DialogHeight;
		OriginalDialogWidth=DialogWidth;
	}
}

void CResizeDialog::SetEnableMenu()
{
	YOverhead+=GetSystemMetrics(SM_CYMENU);
}

void CResizeDialog::SetResizeNoCaption()
{
	Caption=FALSE;
	YOverhead=XOverhead;
}

void CResizeDialog::SetResizeNoBorder()
{
	Border=FALSE;
	YOverhead=XOverhead=0;
}

/////////////////////////////////////////////////////////////////////////////
// CResizeDialog message handlers

void CResizeDialog::OnSize(UINT nType, int cx, int cy) 
{
	// TODO: Add your message handler code here
	if(!IsIconic())
	{
		RECT DialogRC,ControlRC;
		GetWindowRect(&DialogRC);
		int XChange = (DialogRC.right-DialogRC.left)-DialogWidth;
		int YChange = (DialogRC.bottom-DialogRC.top)-DialogHeight;
		BOOL Bail=FALSE;

		if(OriginalDialogHeight!=0)
		{
			if((DialogHeight+YChange)<OriginalDialogHeight)
			{
				MoveWindow(DialogRC.left,DialogRC.top,DialogRC.right-DialogRC.left,DialogHeight,TRUE);
				Bail=TRUE;
			}
			if((DialogWidth+XChange)<OriginalDialogWidth)
			{
				MoveWindow(DialogRC.left,DialogRC.top,DialogWidth,DialogRC.bottom-DialogRC.top,TRUE);
				Bail=TRUE;
			}
			if(Bail)
			{
				SetSize();
				return;
			}
		}

		CDialogEx::OnSize(nType, cx, cy);

		CWnd *Ctrl=GetWindow(GW_CHILD);
		while(Ctrl!=NULL)
		{
			long ResizeFlag=GetWindowLong(Ctrl->m_hWnd,GWLP_USERDATA);
			Ctrl->GetWindowRect(&ControlRC);

			switch(ResizeFlag)
			{
				case	RESIZE_FOLLOW_BOTTOM	:
													Ctrl->MoveWindow((ControlRC.left-DialogRC.left)-XOverhead,
																	((ControlRC.top-DialogRC.top)+YChange)-YOverhead,
																	ControlRC.right-ControlRC.left,
																	ControlRC.bottom-ControlRC.top,TRUE);
													break;
				case	RESIZE_FOLLOW_RIGHT		:
													Ctrl->MoveWindow(((ControlRC.left-DialogRC.left)+XChange)-XOverhead,
																	(ControlRC.top-DialogRC.top)-YOverhead,
																	ControlRC.right-ControlRC.left,
																	ControlRC.bottom-ControlRC.top,TRUE);
													break;
				case	RESIZE_FOLLOW_BOTTOMRIGHT:
													Ctrl->MoveWindow(((ControlRC.left-DialogRC.left)+XChange)-XOverhead,
																	((ControlRC.top-DialogRC.top)+YChange)-YOverhead,
																	ControlRC.right-ControlRC.left,
																	ControlRC.bottom-ControlRC.top,TRUE);
													break;
				case	RESIZE_ADJUST_RIGHT		:
													Ctrl->MoveWindow((ControlRC.left-DialogRC.left)-XOverhead,
																	(ControlRC.top-DialogRC.top)-YOverhead,
																	(ControlRC.right-ControlRC.left)+XChange,
																	ControlRC.bottom-ControlRC.top,TRUE);
													break;
				case	RESIZE_ADJUST_BOTTOM	:
													Ctrl->MoveWindow((ControlRC.left-DialogRC.left)-XOverhead,
																	(ControlRC.top-DialogRC.top)-YOverhead,
																	ControlRC.right-ControlRC.left,
																	(ControlRC.bottom-ControlRC.top)+YChange,TRUE);
													break;
				case	RESIZE_ADJUST_BOTTOMRIGHT:
													Ctrl->MoveWindow((ControlRC.left-DialogRC.left)-XOverhead,
																	(ControlRC.top-DialogRC.top)-YOverhead,
																	(ControlRC.right-ControlRC.left)+XChange,
																	(ControlRC.bottom-ControlRC.top)+YChange,TRUE);
													break;
				case	RESIZE_FOLLOW_BOTTOM_ADJUST_RIGHT:
													Ctrl->MoveWindow(((ControlRC.left - DialogRC.left)) - XOverhead,
																	((ControlRC.top - DialogRC.top) + YChange) - YOverhead,
																	(ControlRC.right - ControlRC.left) + XChange,
																	ControlRC.bottom - ControlRC.top, TRUE);
																	break;
			}
			Ctrl = Ctrl->GetWindow(GW_HWNDNEXT);
		}
		SetSize();
	}
}

