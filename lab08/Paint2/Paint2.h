#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"
#include "Canvas.h"
#include "Doc.h"

class CPaint2App : public CWinAppEx
{
public:
	CPaint2App();
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();
	
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
	CCanvas m_canvas;
	CDoc m_doc;
};

extern CPaint2App theApp;
