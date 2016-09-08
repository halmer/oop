#include "stdafx.h"
#include "XmlWriter.h"
#include "PaintDoc.h"

CXmlWriter::CXmlWriter(CPaintDoc * pDoc)
	: m_pDoc(pDoc)
{
}

void CXmlWriter::WriteData(CArchive & ar)
{
	pugi::xml_document doc;
	pugi::xml_node node = doc.append_child("Shapes");
	auto & shapes = m_pDoc->GetShapesData();

	for (auto const & shape : shapes)
	{
		CRect rect = shape->GetFrameRect();
		rect.NormalizeRect();
		pugi::xml_node param = node.append_child("Shape");

		switch (shape->GetType())
		{
		case ShapeType::Rectangle:
			param.append_attribute("type") = "Rectangle";
			break;
		case ShapeType::Triangle:
			param.append_attribute("type") = "Triangle";
			break;
		case ShapeType::Ellipse:
			param.append_attribute("type") = "Ellipse";
			break;
		}
		param.append_attribute("left") = rect.left;
		param.append_attribute("top") = rect.top;
		param.append_attribute("right") = rect.right;
		param.append_attribute("bottom") = rect.bottom;
	}

	std::stringstream strm;
	doc.save(strm);
	ar.Write(strm.str().data(), strm.str().size());
}
