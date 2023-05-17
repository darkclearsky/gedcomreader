
// grDlg.cpp : implementation file
//

#include "pch.h"
#include "ResizeDialog.h"
#include "framework.h"
#include "gr.h"
#include "grDlg.h"
#include "GedData.h"
#include "afxdialogex.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CgrDlg dialog



CgrDlg::CgrDlg(CWnd* pParent /*=nullptr*/)
	: CResizeDialog(IDD_GR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTLOG, m_CtrlListLog);
	DDX_Control(pDX, IDC_TREE, m_CtrlTree);
	DDX_Control(pDX, IDC_EDITDATA, m_CtrlTextData);
	DDX_Control(pDX, IDC_TAB, m_CtrlTab);
	DDX_Control(pDX, IDC_DRAW, m_CtrlDraw);
}

BEGIN_MESSAGE_MAP(CgrDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, &CgrDlg::OnSelchangedTree)
	ON_EN_CHANGE(IDC_EDITDATA, &CgrDlg::OnEnChangeEditdata)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CgrDlg::OnSelchangeTab)
END_MESSAGE_MAP()


// CgrDlg message handlers

BOOL CgrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// imageList.Create(16, 16, ILC_COLOR8 | ILC_MASK, 4, 0xff);
	imageList.Create(16, 16, ILC_COLOR8 | ILC_MASK, 4, 0xff);
	maleIconIndex = imageList.Add(AfxGetApp()->LoadIcon(IDI_ICONMALE));
	femaleIconIndex = imageList.Add(AfxGetApp()->LoadIcon(IDI_ICONFEMALE));
	genericIconIndex = imageList.Add(AfxGetApp()->LoadIconW(IDI_ICONGENERIC));
	m_CtrlTree.SetImageList(&imageList, TVSIL_NORMAL);

	// TODO: Add extra initialization here
	SetWindowLong(GetDlgItem(IDC_TREE)->m_hWnd, GWLP_USERDATA, RESIZE_ADJUST_BOTTOM);
	SetWindowLong(GetDlgItem(IDC_TAB)->m_hWnd, GWLP_USERDATA, RESIZE_ADJUST_BOTTOMRIGHT);
	SetWindowLong(GetDlgItem(IDC_EDITDATA)->m_hWnd, GWLP_USERDATA, RESIZE_ADJUST_BOTTOMRIGHT);
	SetWindowLong(GetDlgItem(IDC_DRAW)->m_hWnd, GWLP_USERDATA, RESIZE_ADJUST_BOTTOMRIGHT);
	SetWindowLong(GetDlgItem(IDC_LISTLOG)->m_hWnd, GWLP_USERDATA, RESIZE_FOLLOW_BOTTOM_ADJUST_RIGHT);

	// set up tabs and the subordinate dialogs
	m_CtrlTab.InsertItem(0, _T("Info"));
	m_CtrlTab.InsertItem(1, _T("Circle"));
	m_CtrlDraw.ShowWindow(SW_HIDE);

	SetSize();

	SetEnableMenu();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CgrDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CgrDlg::OnPaint()
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
HCURSOR CgrDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CgrDlg::OnSize(UINT nType, int cx, int cy)
{
	CResizeDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CResizeDialog::OnSize(nType, cx, cy);
}


BOOL CgrDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	int x;
	OPENFILENAME ofn;
	wchar_t GedComFilename[MAX_GEDCOMFILENAMESIZE] = _T("");

	switch (LOWORD(wParam)) 
	{
		case ID_FILE_EXIT:
			if(AfxMessageBox(_T("Really quit?"), MB_OKCANCEL| MB_ICONQUESTION) == IDOK)
				PostQuitMessage(0);
			break;
		case ID_FILE_OPEN:
			memset(&ofn, 0, sizeof(OPENFILENAME));
			// memset(&GedComFilename, 0, MAX_GEDCOMFILENAMESIZE);
			// ZeroMemory(GedComFilename, 0, MAX_GEDCOMFILENAMESIZE);
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = this->m_hWnd;
			ofn.lpstrFile = GedComFilename;
			ofn.nMaxFile = MAX_GEDCOMFILENAMESIZE;
			ofn.lpstrFilter = _T("GEDCOM files\0*.GED\0");
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_EXPLORER;
			// if (GetOpenFileNameA((LPOPENFILENAMEA)&ofn)) {
			if(GetOpenFileName(&ofn)) {
				if (!loadGEDFile(GedComFilename)) {
					AfxMessageBox(_T("An error occured, could not load file"), MB_OK | MB_ICONEXCLAMATION);
				} else {
					CString Msg;
//					/*
					Msg.Format(_T("%ld individuals loaded"), Ged.indiList.GetCount());
					LogMessage(Msg);
					Msg.Format(_T("%ld families loaded"), Ged.famList.GetCount());
					LogMessage(Msg);
//					*/
					buildTree();
					LogMessage(CString(_T("Tree built")));
					// populate the tree control with direct descendents
					if (!Ged.indiList.IsEmpty()) {
						CList<CIndi, CIndi&>indiStack;
						// CIndi& indi = Ged.indiList.GetHead();
						CIndi* pindi = &Ged.indiList.GetHead();
						HTREEITEM hParentTree = TVI_ROOT;
						while (TRUE) {
							while (pindi != NULL) {
								CString cleanName = pindi->givenName + _T(" ") + pindi->surname;
								pindi->hti = m_CtrlTree.InsertItem(cleanName, hParentTree);
								m_CtrlTree.SetItemData(pindi->hti, (DWORD_PTR)pindi);
								switch (pindi->sex) {
								case SEX_MALE:
									m_CtrlTree.SetItemImage(pindi->hti, maleIconIndex, maleIconIndex);
									break;
								case SEX_FEMALE:
									m_CtrlTree.SetItemImage(pindi->hti, femaleIconIndex, femaleIconIndex);
									break;
								default:
									m_CtrlTree.SetItemImage(pindi->hti, genericIconIndex, genericIconIndex);
									break;
								}
								indiStack.AddTail(*pindi);
								hParentTree = pindi->hti;
								pindi = pindi->father;
							}
							if (indiStack.IsEmpty()) {
								break;
							}
							pindi = &indiStack.RemoveTail();
							hParentTree = pindi->hti;
							pindi = pindi->mother;
						}
					}
				}
			}
			break;
	}
	return(TRUE);
	// return CResizeDialog::OnCommand(wParam, lParam);
}


void CgrDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	PostQuitMessage(0);
	CResizeDialog::OnClose();
}


int CgrDlg::loadGEDFile(wchar_t *filename)
{
	return(Ged.load(filename));
}

int CgrDlg::buildTree()
{
	return(Ged.buildTree());
}

int CgrDlg::LogMessage(CString& Message)
{
	// TODO: Add your implementation code here.
	m_CtrlListLog.AddString(Message);
	return 0;
}


BOOL CgrDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class

	return CResizeDialog::OnNotify(wParam, lParam, pResult);
}


void CgrDlg::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM hTreeSelectedItem = pNMTreeView->itemNew.hItem;
	CIndi* pindi = (CIndi *)m_CtrlTree.GetItemData(hTreeSelectedItem);
	CString Msg;
	Msg.Format(_T("GEDCOM individual refernence: %s\r\nName: %s\r\nSource:%s\r\nBirth date:%s - %s\r\nDied: %s - %s"),
		pindi->reference,
		pindi->name,
		pindi->source,
		pindi->birthDate,
		pindi->birthPlace,
		pindi->deathDate,
		pindi->deathPlace);
	m_CtrlTextData.SetWindowText(Msg);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CgrDlg::OnEnChangeEditdata()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CResizeDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CgrDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	int TabSelected = m_CtrlTab.GetCurSel();
	switch (TabSelected) {
	case 0 : 
		m_CtrlTextData.ShowWindow(SW_SHOW);
		m_CtrlDraw.ShowWindow(SW_HIDE);
		break;
	case 1 :
		m_CtrlTextData.ShowWindow(SW_HIDE);
		m_CtrlDraw.ShowWindow(SW_SHOW);
		break;
	}
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
