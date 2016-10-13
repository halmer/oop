#include "stdafx.h"
#include "XmlReader.h"
#include "Canvas.h"
#include "Shape.h"
#include "../Paint/pugixml.hpp"

std::unique_ptr<ICanvas> CXmlReader::LoadCanvas(CArchive & ar)
{
	CFile * file = ar.GetFile();
	pugi::xml_document doc;
	if (!doc.load_file((LPCTSTR)file->GetFilePath()))//-V2005
	{
		return nullptr;
	}

	pugi::xml_node shapes = doc.child("Shapes");
	if (!shapes)
	{
		return nullptr;
	}

	std::unique_ptr<ICanvas> canvas = std::make_unique<CCanvas>();
	for (auto const & shape : shapes.children("Shape"))
	{
		std::stringstream strm;
		strm <<
			shape.attribute("type").value() << " " <<
			shape.attribute("left").value() << " " <<
			shape.attribute("top").value() << " " <<
			shape.attribute("right").value() << " " <<
			shape.attribute("bottom").value();
		std::string type;
		int left = 0, top = 0, right = 0, bottom = 0;
		strm >> type >> left >> top >> right >> bottom;

		if (strm)
		{
			if (type == "Rectangle")
			{
				canvas->InsertShape(
					std::make_shared<CShape>(CRect(left, top, right, bottom), ShapeType::Rectangle), boost::none
				);
			}
			else if (type == "Triangle")
			{
				canvas->InsertShape(
					std::make_shared<CShape>(CRect(left, top, right, bottom), ShapeType::Triangle), boost::none
				);
			}
			else if (type == "Ellipse")
			{
				canvas->InsertShape(
					std::make_shared<CShape>(CRect(left, top, right, bottom), ShapeType::Ellipse), boost::none
				);
			}
		}
	}

	return canvas;
}
