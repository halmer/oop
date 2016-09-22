#pragma once

class ICommandSourceDelegate
{
public:
	virtual void OnCreateRectangle() = 0;
	virtual void OnCreateTriangle() = 0;
	virtual void OnCreateEllipse() = 0;
	virtual void OnUndo() = 0;
	virtual void OnRedo() = 0;

	virtual ~ICommandSourceDelegate() = default;
};
