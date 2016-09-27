#include "stdafx.h"
#include "../Paint2/Canvas.h"
#include "../Paint2/Shape.h"

using namespace std;

struct DomainModelTests_
{
	CRect rect0 = CRect(0, 0, 0, 0);
	CRect rect15 = CRect(0, 0, 15, 15);
};

BOOST_FIXTURE_TEST_SUITE(DomainModelTests, DomainModelTests_)
	//
	BOOST_AUTO_TEST_CASE(shape)
	{
		CShape shape(rect0, ShapeType::Triangle);
		BOOST_CHECK_EQUAL(shape.GetRect(), rect0);
		BOOST_CHECK(shape.GetType() == ShapeType::Triangle);
		
		int countChangeShape = 0;
		auto action = [&countChangeShape](IShape const *) { ++countChangeShape; };
		shape.DoOnShapeChange(action);

		shape.SetRect(rect15);
		BOOST_CHECK_EQUAL(shape.GetRect(), rect15);
		BOOST_CHECK_EQUAL(countChangeShape, 1);
	}
	//
	BOOST_AUTO_TEST_CASE(canvas)
	{
		shared_ptr<ICanvas> canvas(new CCanvas());
		BOOST_CHECK_EQUAL(canvas->GetShapeCount(), 0);

		int countInsertShape = 0;
		auto action = [&countInsertShape](std::shared_ptr<IShape> const &, boost::optional<size_t>){ ++countInsertShape; };
		canvas->DoOnInsertShape(action);

		auto rectangle = std::make_shared<CShape>(rect15, ShapeType::Rectangle);
		auto triangle = std::make_shared<CShape>(rect15, ShapeType::Triangle);
		auto ellipse = std::make_shared<CShape>(rect15, ShapeType::Ellipse);
		canvas->InsertShape(rectangle, 0);
		canvas->InsertShape(triangle, boost::none);
		canvas->InsertShape(ellipse, 0);
		BOOST_CHECK_EQUAL(canvas->GetShapeCount(), 3);
		BOOST_CHECK_EQUAL(countInsertShape, 3);

		canvas->DeleteShape(rectangle);
		BOOST_CHECK_EQUAL(canvas->GetShapeCount(), 2);

		canvas->DeleteShape(rectangle);
		BOOST_CHECK_EQUAL(canvas->GetShapeCount(), 2);

		BOOST_CHECK_EQUAL(canvas->GetShapeAtIndex(0), ellipse);
		BOOST_CHECK_EQUAL(canvas->GetShapeAtIndex(1), triangle);

		canvas->InsertShape(rectangle, boost::none);
		BOOST_CHECK_EQUAL(canvas->GetShapeAtIndex(2), rectangle);
	}
BOOST_AUTO_TEST_SUITE_END()
