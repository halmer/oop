#include "stdafx.h"
#include "XmlWriter.h"
#include "ICanvas.h"
#include "IShape.h"
#include "../Paint/pugixml.hpp"

using namespace boost::filesystem;

static path GetImageDirectoryPath(CArchive &ar)
{
	CFile * file = ar.GetFile();
	path filePath(file->GetFilePath());

	std::string fileName = filePath.filename().string();
	fileName.resize(fileName.find('.'));

	return filePath.branch_path() / fileName;
}

static void SaveImage(HANDLE bitmap, path const & imagePath)
{
	CImage img;
	CBitmap bmp;

	bmp.Attach(bitmap);
	img.Attach(bmp);

	img.Save(imagePath.c_str(), Gdiplus::ImageFormatBMP);

	img.Detach();
	bmp.Detach();
}

void CXmlWriter::SaveCanvas(ICanvas const & canvas, CArchive & ar)
{
	path imageDirectoryPath = GetImageDirectoryPath(ar);
	if (is_directory(imageDirectoryPath))
	{
		remove_all(imageDirectoryPath);
	}

	pugi::xml_document doc;
	pugi::xml_node node = doc.append_child("Shapes");
	size_t imageIndex = 0;

	for (size_t i = 0; i < canvas.GetShapeCount(); ++i)
	{
		auto shape = canvas.GetShapeAtIndex(i);
		pugi::xml_node param = node.append_child("Shape");
		std::string imageName = ".";

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
			param.append_attribute("type") = "Image";

			if (!is_directory(imageDirectoryPath))
			{
				create_directory(imageDirectoryPath);
			}

			imageName = "img" + std::to_string(imageIndex++) + ".bmp";
			path imagePath = imageDirectoryPath / imageName;

			SaveImage(shape->GetBitmap(), imagePath);

			break;
		}
		}

		auto rect = shape->GetRect();
		param.append_attribute("left") = rect.left;
		param.append_attribute("top") = rect.top;
		param.append_attribute("right") = rect.right;
		param.append_attribute("bottom") = rect.bottom;

		param.append_attribute("bitmap") = imageName.data();
	}

	std::stringstream strm;
	doc.save(strm);
	ar.Write(strm.str().data(), strm.str().size());
}
