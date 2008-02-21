// RegisterCmmDlg.h : Headerdatei
//

#pragma once
#include "afxwin.h"
#include "icm.h"

// STL include
#include <string>


#define DLL_IMPORT _stdcall


extern "C"
{
	typedef DWORD (DLL_IMPORT *GetInfoPtr)(DWORD dwInfo);
}

// CRegisterCmmDlg Dialogfeld
class CRegisterCmmDlg : public CDialog
{
// Konstruktion
public:
	CRegisterCmmDlg(CWnd* pParent = NULL);	// Standardkonstruktor

// Dialogfelddaten
	enum { IDD = IDD_REGISTERCMM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung

// Implementierung
protected:
	HICON m_hIcon;

	// Generierte Funktionen für die Meldungstabellen
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	CEdit m_path;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	mutable CStatic m_statusText;

protected:
	DWORD CalcIdent(const ::std::string& fileName) const;
};
