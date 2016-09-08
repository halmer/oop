#include "stdafx.h"
#include "XmlReader.h"
#include "PaintDoc.h"

CXmlReader::CXmlReader(CPaintDoc * pDoc)
	: m_pDoc(pDoc)
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
	if (shapes)
	{
		m_pDoc->DeleteAllShapes();
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
				m_pDoc->CreateShape(ShapeType::Rectangle, CRect(left, top, right, bottom));
			}
			else if (type == "Triangle")
			{
				m_pDoc->CreateShape(ShapeType::Triangle, CRect(left, top, right, bottom));
			}
			else if (type == "Ellipse")
			{
				m_pDoc->CreateShape(ShapeType::Ellipse, CRect(left, top, right, bottom));
			}
		}
	}
}
