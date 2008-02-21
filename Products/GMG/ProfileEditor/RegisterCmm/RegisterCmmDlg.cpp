// RegisterCmmDlg.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "RegisterCmm.h"
#include "RegisterCmmDlg.h"
#include ".\registercmmdlg.h"
#include "icm.h"

#include "iwin/CDllFunctionsProvider.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRegisterCmmDlg Dialogfeld



CRegisterCmmDlg::CRegisterCmmDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegisterCmmDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRegisterCmmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_path);
	DDX_Control(pDX, IDC_STATUS_TEXT, m_statusText);
}

BEGIN_MESSAGE_MAP(CRegisterCmmDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
END_MESSAGE_MAP()


// CRegisterCmmDlg Meldungshandler

BOOL CRegisterCmmDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Symbol für dieses Dialogfeld festlegen. Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden

	// TODO: Hier zusätzliche Initialisierung einfügen
	
	return TRUE;  // Geben Sie TRUE zurück, außer ein Steuerelement soll den Fokus erhalten
}

// Wenn Sie dem Dialogfeld eine Schaltfläche "Minimieren" hinzufügen, benötigen Sie 
//  den nachstehenden Code, um das Symbol zu zeichnen. Für MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch ausgeführt.

void CRegisterCmmDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext zum Zeichnen

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Symbol in Clientrechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// Die System ruft diese Funktion auf, um den Cursor abzufragen, der angezeigt wird, während der Benutzer
//  das minimierte Fenster mit der Maus zieht.
HCURSOR CRegisterCmmDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRegisterCmmDlg::OnBnClickedOk()
{
	CString text;
	m_path.GetWindowText(text);

	DWORD identCode = CalcIdent((LPCSTR)text);
	if (identCode != 0){
		if (!RegisterCMM(NULL, identCode, text)){
			UINT errorCode = GetLastError();

			CString message;
			message.Format("Windows Error Code %d", errorCode);
			m_statusText.SetWindowText(message);
		}
		else{
			m_statusText.SetWindowText("OK");
		}
	}
}


void CRegisterCmmDlg::OnBnClickedButton1()
{
	TCHAR szFilters[] = "_T(CMM Modules (*.dll)|*.dll||";

	CFileDialog fileDlg (TRUE, NULL, NULL, OFN_FILEMUSTEXIST, szFilters, this);

	if (fileDlg.DoModal () == IDOK){
		CString pathName = fileDlg.GetPathName();

		CString fileName = fileDlg.GetPathName();

		m_path.SetWindowText(fileName);
	}
}


void CRegisterCmmDlg::OnBnClickedButton2()
{
	CString text;
	m_path.GetWindowText(text);

	DWORD identCode = CalcIdent((LPCSTR)text);
	if (identCode == 0){
		identCode = 0x474d4720;	// GMG
	}

	if (!UnregisterCMM(NULL, identCode)){
		UINT errorCode = GetLastError();

		CString message;
		message.Format("Windows Error Code %d", errorCode);
		m_statusText.SetWindowText(message);
	}
	else{
		m_statusText.SetWindowText("OK");
	}
}


// protected methods
	
DWORD CRegisterCmmDlg::CalcIdent(const ::std::string& fileName) const
{
	iwin::CDllFunctionsProvider provider;

	if (provider.OpenDll(fileName)){
		GetInfoPtr infoFunction = (GetInfoPtr)provider.GetFunction("CMGetInfo");
		if (infoFunction != NULL){
			return infoFunction(CMM_IDENT);
		}
		else{
			m_statusText.SetWindowText("Cannot get ICM identification code!");
		}
	}
	else{
		m_statusText.SetWindowText("Library not found");
	}

	return 0;
}


