#pragma once

class ICanvas;

class CXmlReader
{
public:
	void LoadCanvas(ICanvas & canvas, CArchive & ar);
};
