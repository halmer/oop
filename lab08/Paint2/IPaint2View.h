#pragma once

class IPaint2View
{
public:
	virtual CPoint GetPointInViewCenter() const = 0;

	virtual ~IPaint2View() = default;
};
