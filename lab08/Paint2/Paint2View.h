#pragma once
#include "IPaint2View.h"

class CCanvasView;
class ICommandSourceDelegate;

class CPaint2View : 
	public CScrollView, 
	IPaint2View, 
	ICanvasViewOwner
{
public:
	virtual ~CPaint2View();
	CPaint2Doc* GetDocument() const;

private:
	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) override;
	BOOL PreCreateWindow(CREATESTRUCT& cs) override;
	void OnDraw(CDC* pDC) override;

	// IPaint2View methods
	void Initialize() override;

	// ICanvasViewOwner methods
	void Update(UpdateType type) override;
	CPoint GetPointInViewCenter() const override;
	std::shared_ptr<ICanvasView> GetCanvasView() const override;

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

protected:
	CPaint2View();
	DECLARE_DYNCREATE(CPaint2View)
	DECLARE_MESSAGE_MAP()
	virtual void OnInitialUpdate();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

private:
	std::shared_ptr<CCanvasView> m_canvasView;
	std::unique_ptr<ICommandSourceDelegate> m_delegate;

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
