#include "stdafx.h"
#include "XmlWriter.h"
#include "ICanvas.h"
#include "IShape.h"
#include "../Paint/pugixml.hpp"

void CXmlWriter::SaveCanvas(ICanvas const & canvas, CArchive & ar)
{
	pugi::xml_document doc;
	pugi::xml_node node = doc.append_child("Shapes");

	for (size_t i = 0; i < canvas.GetShapeCount(); ++i)
	{
		auto shape = canvas.GetShapeAtIndex(i);
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
		case ShapeType::Image:
		{
			CImage img;
			CBitmap bmp;
			bmp.Attach(shape->GetBitmap());
			img.Attach(bmp);
			img.Save(L"f:\\temp\\bmp.bmp", Gdiplus::ImageFormatBMP);
			img.Detach();
			bmp.Detach();
			break;
		}
		}


		auto rect = shape->GetRect();
		param.append_attribute("left") = rect.left;
		param.append_attribute("top") = rect.top;
		param.append_attribute("right") = rect.right;
		param.append_attribute("bottom") = rect.bottom;
	}

	std::stringstream strm;
	doc.save(strm);
	ar.Write(strm.str().data(), strm.str().size());
}
