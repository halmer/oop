#pragma once

class CFrame
{
public:
	CFrame();
	
	void SetFrame(CRect const & rect);
	void DrawFrame(CDC * pDC);
	boost::optional<size_t> GetSelectedMark(CPoint const & point) const;
	bool IsInvertedCursor() const;
		
private:
	void InitMarks();

	CRect m_frame;
	std::vector<CRect> m_marks;
	CPen m_markPen;
	CBrush m_markBrush;
	CBrush m_frameBrush;
};
