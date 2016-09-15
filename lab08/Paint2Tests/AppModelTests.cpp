#include "stdafx.h"
#include "../Paint2/EditableRectangle.h"
#include "../Paint2/EditableCanvas.h"
#include "../Paint2/Selection.h"
#include "../Paint2/Canvas.h"
#include "../Paint/History.h"

using namespace std;

struct AppModelTests_
{
	CRect rect0 = CRect(0, 0, 0, 0);
	CRect rect15 = CRect(0, 0, 15, 15);
};

BOOST_FIXTURE_TEST_SUITE(AppModelTests, AppModelTests_)
	//
	BOOST_AUTO_TEST_CASE(rectangle)
	{
		CEditableRectangle rectangle(rect0);
		BOOST_CHECK_EQUAL(rectangle.GetRect(), rect0);

		rectangle.SetRect(rect15);
		BOOST_CHECK_EQUAL(rectangle.GetRect(), rect15);

		BOOST_CHECK(rectangle.GetType() == EditableShapeType::Rectangle);

		int x = 0;
		auto action = [&x](){ x = 5; };
		rectangle.DoOnShapeChange(action);

		BOOST_CHECK_EQUAL(x, 0);
		rectangle.Commit();
		BOOST_CHECK_EQUAL(x, 5);
	}
	//
	BOOST_AUTO_TEST_CASE(rectanglePointer)
	{
		shared_ptr<IEditableShape> rectangle(new CEditableRectangle(rect0));
		BOOST_CHECK_EQUAL(rectangle->GetRect(), rect0);

		rectangle->SetRect(rect15);
		BOOST_CHECK_EQUAL(rectangle->GetRect(), rect15);

		BOOST_CHECK(rectangle->GetType() == EditableShapeType::Rectangle);

		int x = 0;
		auto action = [&x]() { x = 5; };
		rectangle->DoOnShapeChange(action);

		BOOST_CHECK_EQUAL(x, 0);
		rectangle->Commit();
		BOOST_CHECK_EQUAL(x, 5);
	}
	//
	BOOST_AUTO_TEST_CASE(EditableCanvas)
	{
		CCanvas model;
		CHistory history;
		CEditableCanvas canvas(model, history);

		canvas.AddShape(ShapeType::Rectangle, rect0);
		BOOST_CHECK_EQUAL(canvas.GetShapeCount(), 1);

		history.Undo();
		BOOST_CHECK_EQUAL(canvas.GetShapeCount(), 0);
		history.Redo();
		BOOST_CHECK_EQUAL(canvas.GetShapeCount(), 1);

		canvas.AddShape(ShapeType::Triangle, rect15);
		BOOST_CHECK_EQUAL(canvas.GetShapeCount(), 2);

		auto rectangle = canvas.GetShapeAtIndex(0);
		BOOST_CHECK_EQUAL(rectangle->GetRect(), rect0);
		BOOST_CHECK(rectangle->GetType() == ShapeType::Rectangle);

		canvas.SelectShape(rectangle);
		canvas.DeleteSelection();
		BOOST_CHECK_EQUAL(canvas.GetShapeCount(), 1);

		auto triangle = canvas.GetShapeAtIndex(0);
		BOOST_CHECK_EQUAL(triangle->GetRect(), rect15);
		BOOST_CHECK(triangle->GetType() == ShapeType::Triangle);

		history.Undo();
		BOOST_CHECK_EQUAL(canvas.GetShapeCount(), 2);
		BOOST_CHECK(canvas.GetShapeAtIndex(0) == rectangle);
		BOOST_CHECK(canvas.GetShapeAtIndex(1) == triangle);

		canvas.SelectShape(rectangle);
		auto selection = canvas.GetSelection();

		selection->SetRect(rect15);
		auto rectangleNew = selection->GetShape();

		BOOST_CHECK_EQUAL(model.GetShapeCount(), 2);
		BOOST_CHECK(model.GetShapeAtIndex(0) == rectangle);
		BOOST_CHECK(model.GetShapeAtIndex(1) == triangle);
		BOOST_CHECK_EQUAL(canvas.GetShapeCount(), 2);
		BOOST_CHECK(canvas.GetShapeAtIndex(0) == rectangleNew);
		BOOST_CHECK(canvas.GetShapeAtIndex(1) == triangle);

		selection->Commit();
		BOOST_CHECK_EQUAL(model.GetShapeCount(), 2);
		auto rectangleNewModel = model.GetShapeAtIndex(0);
		BOOST_CHECK_EQUAL(rectangleNewModel->GetRect(), rect15);
		BOOST_CHECK(rectangleNewModel->GetType() == ShapeType::Rectangle);
		BOOST_CHECK(model.GetShapeAtIndex(1) == triangle);
		BOOST_CHECK_EQUAL(canvas.GetShapeCount(), 2);
		BOOST_CHECK(canvas.GetShapeAtIndex(0) == rectangleNew);
		BOOST_CHECK(canvas.GetShapeAtIndex(1) == triangle);
	}

BOOST_AUTO_TEST_SUITE_END()