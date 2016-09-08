#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"
#include "Model.h"

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
	CModel m_model;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CPaintApp theApp;
