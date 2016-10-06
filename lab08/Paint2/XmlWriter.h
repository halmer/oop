#pragma once

class ICanvas;

class CXmlWriter
{
public:
	CXmlWriter(ICanvas & canvas);
	void WriteData(CArchive & ar);

private:
	ICanvas & m_canvas;
};
