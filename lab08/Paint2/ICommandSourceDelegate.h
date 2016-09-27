#pragma once

class ICommandSourceDelegate
{
public:
	virtual void OnCreateRectangle() = 0;
	virtual void OnCreateTriangle() = 0;
	virtual void OnCreateEllipse() = 0;
	virtual void OnUndo() = 0;
	virtual void OnRedo() = 0;
/*
	virtual void OnLButtonDown(UINT nFlags, CPoint point) = 0;
	virtual void OnLButtonUp(UINT nFlags, CPoint point) = 0;
	virtual void OnMouseMove(UINT nFlags, CPoint point) = 0;
*/
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) = 0;
	virtual void OnUpdateUndo(CCmdUI * pCmdUI) = 0;
	virtual void OnUpdateRedo(CCmdUI * pCmdUI) = 0;

	virtual ~ICommandSourceDelegate() = default;
};
