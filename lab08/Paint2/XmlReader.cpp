#include "stdafx.h"
#include "XmlReader.h"
#include "ICanvas.h"
#include "Shape.h"
#include "../Paint/pugixml.hpp"

CXmlReader::CXmlReader(ICanvas & canvas)
	: m_canvas(canvas)
{
}

void CXmlReader::ReadData(CArchive & ar)
{
	CFile * file = ar.GetFile();
	pugi::xml_document doc;
	if (!doc.load_file((LPCTSTR)file->GetFilePath()))//-V2005
	{
		return;
	}

	pugi::xml_node shapes = doc.child("Shapes");
	if (!shapes)
	{
		return;
	}

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
				m_canvas.InsertShape(
					std::make_shared<CShape>(CRect(left, top, right, bottom), ShapeType::Rectangle), boost::none
				);
			}
			else if (type == "Triangle")
			{
				m_canvas.InsertShape(
					std::make_shared<CShape>(CRect(left, top, right, bottom), ShapeType::Triangle), boost::none
				);
			}
			else if (type == "Ellipse")
			{
				m_canvas.InsertShape(
					std::make_shared<CShape>(CRect(left, top, right, bottom), ShapeType::Ellipse), boost::none
				);
			}
		}
	}
}
