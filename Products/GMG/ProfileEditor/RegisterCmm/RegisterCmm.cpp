// RegisterCmm.cpp : Definiert das Klassenverhalten für die Anwendung.
//

#include "stdafx.h"
#include "RegisterCmm.h"
#include "RegisterCmmDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRegisterCmmApp

BEGIN_MESSAGE_MAP(CRegisterCmmApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CRegisterCmmApp-Erstellung

#include "cp_mx.h"
#include "cp_mx_i.c"

CRegisterCmmApp::CRegisterCmmApp()
{
	::CoInitializeEx(NULL, COINIT_MULTITHREADED);

	CComQIPtr<IMxManager> mxManagerPtr;

	HRESULT hr = mxManagerPtr.CoCreateInstance(CLSID_MxManager);
	if (FAILED(hr)){
		return;
	}
}


CRegisterCmmApp::~CRegisterCmmApp()
{
	::CoUninitialize();
}


// Das einzige CRegisterCmmApp-Objekt

CRegisterCmmApp theApp;


// CRegisterCmmApp Initialisierung

BOOL CRegisterCmmApp::InitInstance()
{
	CWinApp::InitInstance();

	// Standardinitialisierung
	// Wenn Sie diese Features nicht verwenden und die Größe
	// der ausführbaren Datei verringern möchten, entfernen Sie
	// die nicht erforderlichen Initialisierungsroutinen.
	// Ändern Sie den Registrierungsschlüssel unter dem Ihre Einstellungen gespeichert sind.
	// TODO: Ändern Sie diese Zeichenfolge entsprechend,
	// z.B. zum Namen Ihrer Firma oder Organisation.
	SetRegistryKey(_T("Vom lokalen Anwendungs-Assistenten generierte Anwendungen"));

	CRegisterCmmDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Fügen Sie hier Code ein, um das Schließen des
		//  Dialogfelds über OK zu steuern
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Fügen Sie hier Code ein, um das Schließen des
		//  Dialogfelds über "Abbrechen" zu steuern
	}

	// Da das Dialogfeld geschlossen wurde, FALSE zurückliefern, so dass wir die
	//  Anwendung verlassen, anstatt das Nachrichtensystem der Anwendung zu starten.
	return FALSE;
}
