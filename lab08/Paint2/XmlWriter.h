#pragma once

class ICanvas;

class CXmlWriter
{
public:
	void SaveCanvas(CArchive & ar, const ICanvas & canvas);

};
