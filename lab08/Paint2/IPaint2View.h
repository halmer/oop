#pragma once

enum class UpdateType
{
	Redraw,
	RedrawUpdateScroll,
};


class ICanvasView;

class ICanvasViewOwner
{
public:
	virtual void Update(UpdateType type) = 0;
	virtual CPoint GetPointInViewCenter() const = 0;
	virtual std::shared_ptr<ICanvasView> GetCanvasView() const = 0;

	virtual ~ICanvasViewOwner() = default;
};


class IPaint2View
{
public:
	virtual void Initialize() = 0;
	
	virtual ~IPaint2View() = default;
};
