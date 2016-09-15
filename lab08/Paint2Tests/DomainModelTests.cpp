#include "stdafx.h"
#include "../Paint2/RectangleShape.h"
#include "../Paint2/TriangleShape.h"
#include "../Paint2/EllipseShape.h"
#include "../Paint2/Canvas.h"

using namespace std;

struct DomainModelTests_
{
	CRect rect0 = CRect(0, 0, 0, 0);
	CRect rect15 = CRect(0, 0, 15, 15);
};

BOOST_FIXTURE_TEST_SUITE(DomainModelTests, DomainModelTests_)
	//
	BOOST_AUTO_TEST_CASE(rectangle)
	{
		CRectangleShape rectangle(rect0);
		BOOST_CHECK_EQUAL(rectangle.GetRect(), rect0);
		
		rectangle.SetRect(rect15);
		BOOST_CHECK_EQUAL(rectangle.GetRect(), rect15);

		BOOST_CHECK(rectangle.GetType() == ShapeType::Rectangle);
	}
	//
	BOOST_AUTO_TEST_CASE(trianglePointer)
	{
		shared_ptr<IShape> triangle(new CTriangleShape(rect0));
		BOOST_CHECK_EQUAL(triangle->GetRect(), rect0);

		triangle->SetRect(rect15);
		BOOST_CHECK_EQUAL(triangle->GetRect(), rect15);

		BOOST_CHECK(triangle->GetType() == ShapeType::Triangle);
	}
	//
	BOOST_AUTO_TEST_CASE(ellipsePointer)
	{
		shared_ptr<IShape> ellipse(new CEllipseShape(rect0));
		BOOST_CHECK_EQUAL(ellipse->GetRect(), rect0);

		ellipse->SetRect(rect15);
		BOOST_CHECK_EQUAL(ellipse->GetRect(), rect15);

		BOOST_CHECK(ellipse->GetType() == ShapeType::Ellipse);
	}
	//
	BOOST_AUTO_TEST_CASE(canvasPointer)
	{
		shared_ptr<ICanvas> canvas(new CCanvas());
		BOOST_CHECK_EQUAL(canvas->GetShapeCount(), 0);

		shared_ptr<IShape> rectangle(new CRectangleShape(rect15));
		shared_ptr<IShape> triangle(new CTriangleShape(rect15));
		shared_ptr<IShape> ellipse(new CEllipseShape(rect15));
		canvas->InsertShape(rectangle, 0);
		canvas->InsertShape(triangle, boost::none);
		canvas->InsertShape(ellipse, 0);
		BOOST_CHECK_EQUAL(canvas->GetShapeCount(), 3);

		canvas->DeleteShape(rectangle);
		BOOST_CHECK_EQUAL(canvas->GetShapeCount(), 2);

		canvas->DeleteShape(rectangle);
		BOOST_CHECK_EQUAL(canvas->GetShapeCount(), 2);

		BOOST_CHECK_EQUAL(canvas->GetShapeAtIndex(0), ellipse);
		BOOST_CHECK_EQUAL(canvas->GetShapeAtIndex(1), triangle);

		canvas->InsertShape(rectangle, boost::none);
		BOOST_CHECK_EQUAL(canvas->GetShapeAtIndex(2), rectangle);

		BOOST_CHECK(canvas->GetShapeAtIndex(3) == nullptr);
	}
BOOST_AUTO_TEST_SUITE_END()
