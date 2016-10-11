#pragma once

class ICanvas;

class CXmlWriter
{
public:
	void SaveCanvas(ICanvas const & canvas, CArchive & ar);
};
