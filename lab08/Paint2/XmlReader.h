#pragma once

class ICanvas;

class CXmlReader
{
public:
	CXmlReader(ICanvas & canvas);
	void ReadData(CArchive & ar);

private:
	ICanvas & m_canvas;
};
