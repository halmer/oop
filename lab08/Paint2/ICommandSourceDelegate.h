#pragma once

class ICommandSourceDelegate
{
public:
	virtual void OnCreateRectangle() = 0;
	virtual void OnCreateTriangle() = 0;
	virtual void OnCreateEllipse() = 0;
	virtual void OnUndo() = 0;
	virtual void OnRedo() = 0;
	virtual void OnUpdateUndo(CCmdUI * pCmdUI) = 0;
	virtual void OnUpdateRedo(CCmdUI * pCmdUI) = 0;

	virtual ~ICommandSourceDelegate() = default;
};
