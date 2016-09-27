#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"
#include "Canvas.h"
#include "Doc.h"
#include "CanvasPresenter.h"

class CPaint2App : public CWinAppEx
{
public:
	CPaint2App();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	CCanvas m_canvas;
	CDoc m_doc;
	
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CPaint2App theApp;
