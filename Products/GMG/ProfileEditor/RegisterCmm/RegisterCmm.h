// RegisterCmm.h : Hauptheaderdatei für die RegisterCmm-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error 'stdafx.h' muss vor dieser Datei in PCH eingeschlossen werden.
#endif

#include "resource.h"		// Hauptsymbole


// CRegisterCmmApp:
// Siehe RegisterCmm.cpp für die Implementierung dieser Klasse
//

class CRegisterCmmApp : public CWinApp
{
public:
	CRegisterCmmApp();
	~CRegisterCmmApp();

// Überschreibungen
	public:
	virtual BOOL InitInstance();

// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern CRegisterCmmApp theApp;
