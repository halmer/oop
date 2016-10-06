#pragma once
#include "IPaint2View.h"
#include "CanvasPresenter.h"
#include "CanvasView.h"

struct ViewCreationParams : CCreateContext
{
	
};

class CPaint2View : public CScrollView, IPaint2View
{
public:
	virtual ~CPaint2View();
	CPaint2Doc* GetDocument() const;
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnDraw(CDC* pDC);

	afx_msg void OnCreateRectangle();
	afx_msg void OnCreateTriangle();
	afx_msg void OnCreateEllipse();
	afx_msg void OnUndo();
	afx_msg void OnRedo();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnUpdateUndo(CCmdUI * pCmdUI);
	afx_msg void OnUpdateRedo(CCmdUI * pCmdUI);
	
	CPoint GetPointInViewCenter() const override;

protected:
	CPaint2View();
	DECLARE_DYNCREATE(CPaint2View)
	DECLARE_MESSAGE_MAP()
	virtual void OnInitialUpdate();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

private:
	void SetScroll();

	CCanvasView m_canvasView;
	CCanvasPresenter m_presenter;

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

#ifndef _DEBUG  // debug version in Paint2View.cpp
inline CPaint2Doc* CPaint2View::GetDocument() const
   { return reinterpret_cast<CPaint2Doc*>(m_pDocument); }
#endif
