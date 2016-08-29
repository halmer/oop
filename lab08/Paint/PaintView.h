#pragma once
#include "Frame.h"

class CPaintView : public CScrollView
{	
public:
	virtual ~CPaintView();
	CPaintDoc* GetDocument() const;
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnDraw(CDC* pDC);
	
	afx_msg void OnButtonRectangle();
	afx_msg void OnButtonTriangle();
	afx_msg void OnButtonEllipse();
	afx_msg void OnButtonUndo();
	afx_msg void OnButtonRedo();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnUpdateButtonUndo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonRedo(CCmdUI *pCmdUI);
	
protected:
	CPaintView();
	DECLARE_DYNCREATE(CPaintView)
	DECLARE_MESSAGE_MAP()
	virtual void OnInitialUpdate();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

private:
	CPoint GetTopLeftForShape() const;
	OffsetType GetOffsetType(size_t frameMark) const;
	void SetAppCursor(CPoint const & point);
	void ChangeCursor(bool isSelectedShape, bool isSelectedMark);
	bool IsOverMaxPosition(CPoint const & point) const;
	
	CPen m_pen;
	CBrush m_brush;
	HCURSOR m_cursorArrow;
	HCURSOR m_cursorSizeAll;
	HCURSOR m_cursorSizeNWSE;
	HCURSOR m_cursorSizeNESW;

	boost::optional<CPoint> m_firstPoint;
	boost::optional<CPoint> m_lastPoint;
	bool m_useHistory;
	
	CFrame m_frame;
	boost::optional<size_t> m_selectedMark;
	std::shared_ptr<CShape> m_selectedShape;
	std::shared_ptr<CShape> m_selectedShapeWhenMoving;

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

#ifndef _DEBUG
inline CPaintDoc* CPaintView::GetDocument() const
   { return reinterpret_cast<CPaintDoc*>(m_pDocument); }
#endif
