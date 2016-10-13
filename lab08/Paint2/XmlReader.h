#pragma once

class ICanvas;

class CXmlReader
{
public:
	std::unique_ptr<ICanvas> LoadCanvas(CArchive & ar);
};
