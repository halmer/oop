#include "stdafx.h"
#include "XmlReader.h"
#include "Canvas.h"
#include "Shape.h"
#include "../Paint/pugixml.hpp"

using namespace boost::filesystem;

std::unique_ptr<ICanvas> CXmlReader::LoadCanvas(CArchive & ar)
{
	CFile * file = ar.GetFile();
	path filePath(file->GetFilePath());

	std::string fileName = filePath.filename().string();
	fileName.resize(fileName.find('.'));

	pugi::xml_document doc;
	if (!doc.load_file(filePath.c_str()))
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
			shape.attribute("bottom").value() << " " <<
			shape.attribute("bitmap").value();
		std::string type, bitmapName;
		int left = 0, top = 0, right = 0, bottom = 0;
		strm >> type >> left >> top >> right >> bottom >> bitmapName;

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
			else if (type == "Image")
			{
				path imagePath = filePath.branch_path() / fileName / bitmapName;

				HANDLE hBitmap = LoadImage(NULL, imagePath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				canvas->InsertShape(
					std::make_shared<CShape>(CRect(left, top, right, bottom), ShapeType::Image, hBitmap), boost::none
				);
			}
		}
	}

	return canvas;
}
