#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"

class CPaintApp : public CWinAppEx
{
public:
	CPaintApp();
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();
	UINT m_nAppLook = 0;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CPaintApp theApp;
