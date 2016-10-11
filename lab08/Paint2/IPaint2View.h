#pragma once

enum class UpdateType
{
	Redraw,
	RedrawUpdateScroll,
};

class IPaint2View
{
public:
	virtual void Initialize() = 0;
	virtual void Update(UpdateType type) = 0;
	virtual CPoint GetPointInViewCenter() const = 0;
	
	virtual ~IPaint2View() = default;
};
