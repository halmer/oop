#include "stdafx.h"
#include "../Paint2/EditableShape.h"
#include "../Paint2/EditableCanvas.h"
#include "../Paint2/Selection.h"
#include "../Paint2/Canvas.h"
#include "../Paint/History.h"
#include "../Paint2/Doc.h"
#include "../Paint2/Shape.h"
#include <initializer_list>

using namespace std;

struct AppModelTests_
{
	AppModelTests_()
		: doc(domainModel)
	{}

	bool VerifyExpectedShapes(initializer_list<pair<ShapeType, CRect>> shapes)
	{
		auto & canvas = doc.GetCanvas();
		if (canvas.GetShapeCount() != domainModel.GetShapeCount() || domainModel.GetShapeCount() != shapes.size())
		{
			return false;
		}

		size_t index = 0;
		bool result = true;
		for (auto const & item : shapes)
		{
			result &= domainModel.GetShapeAtIndex(index)->GetType() == item.first;
			result &= (domainModel.GetShapeAtIndex(index)->GetRect() == item.second) != 0;
			result &= canvas.GetShapeAtIndex(index)->GetType() == domainModel.GetShapeAtIndex(index)->GetType();
			result &= (canvas.GetShapeAtIndex(index)->GetRect() == domainModel.GetShapeAtIndex(index)->GetRect()) != 0;
			++index;
		}

		return result;
	}

	bool VerifyExpectedShapes(initializer_list<pair<ShapeType, CRect>> shapes, initializer_list<pair<ShapeType, CRect>> editableShapes)
	{
		auto & canvas = doc.GetCanvas();
		if (   canvas.GetShapeCount() != domainModel.GetShapeCount()
			|| domainModel.GetShapeCount() != shapes.size()
			|| domainModel.GetShapeCount() != editableShapes.size())
		{
			return false;
		}

		size_t index = 0;
		bool result = true;
		for (auto const & item : shapes)
		{
			result &= domainModel.GetShapeAtIndex(index)->GetType() == item.first;
			result &= (domainModel.GetShapeAtIndex(index)->GetRect() == item.second) != 0;
			++index;
		}

		index = 0;
		for (auto const & item : editableShapes)
		{
			result &= canvas.GetShapeAtIndex(index)->GetType() == item.first;
			result &= (canvas.GetShapeAtIndex(index)->GetRect() == item.second) != 0;
			++index;
		}

		return result;
	}

	CCanvas domainModel;
	CDoc doc;
	CRect rect0 = CRect(0, 0, 0, 0);
	CRect rect15 = CRect(0, 0, 15, 15);
};

BOOST_FIXTURE_TEST_SUITE(AppModelTests, AppModelTests_)
	//
	BOOST_AUTO_TEST_CASE(editable_shape)
	{
		auto shape = make_shared<CShape>(rect0, ShapeType::Rectangle);
		CHistory history;
		CEditableShape editableShape(shape, history);
		BOOST_CHECK_EQUAL(editableShape.GetRect(), rect0);

		editableShape.SetRect(rect15);
		BOOST_CHECK_EQUAL(editableShape.GetRect(), rect15);

		editableShape.Offset(CPoint(-15, -15), OffsetType::BottomRight);
		BOOST_CHECK_EQUAL(editableShape.GetRect(), rect0);

		BOOST_CHECK(editableShape.GetType() == ShapeType::Rectangle);

		IEditableShape const * shapePointer = nullptr;
		auto action = [&shapePointer](IEditableShape const * shape) { shapePointer = shape; };
		editableShape.DoOnShapeChange(action);

		editableShape.SetRect(rect15);
		BOOST_CHECK(shapePointer == &editableShape);
		
		shapePointer = nullptr;
		editableShape.Commit();
		BOOST_CHECK(shapePointer == &editableShape);
	}
	//
	BOOST_AUTO_TEST_CASE(editable_canvas)
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
		BOOST_CHECK(canvas.GetShapeAtIndex(0)->GetRect() == rect0);
		BOOST_CHECK(canvas.GetShapeAtIndex(0)->GetType() == ShapeType::Rectangle);
		BOOST_CHECK(canvas.GetShapeAtIndex(1) == triangle);

		canvas.SelectShape(triangle);
		auto selection = canvas.GetSelection();

		selection->SetRect(rect0);

		BOOST_CHECK(model.GetShapeAtIndex(1)->GetRect() == rect15);
		BOOST_CHECK(model.GetShapeAtIndex(1)->GetType() == ShapeType::Triangle);
		BOOST_CHECK(canvas.GetShapeAtIndex(1)->GetRect() == rect0);
		BOOST_CHECK(canvas.GetShapeAtIndex(1)->GetType() == ShapeType::Triangle);
		
		selection->Commit();
		BOOST_CHECK(model.GetShapeAtIndex(1)->GetRect() == rect0);
		BOOST_CHECK(model.GetShapeAtIndex(1)->GetType() == ShapeType::Triangle);
		BOOST_CHECK(canvas.GetShapeAtIndex(1)->GetRect() == rect0);
		BOOST_CHECK(canvas.GetShapeAtIndex(1)->GetType() == ShapeType::Triangle);

		history.Undo();
		BOOST_CHECK(model.GetShapeAtIndex(1)->GetRect() == rect15);
		BOOST_CHECK(model.GetShapeAtIndex(1)->GetType() == ShapeType::Triangle);
		BOOST_CHECK(canvas.GetShapeAtIndex(1)->GetRect() == rect15);
		BOOST_CHECK(canvas.GetShapeAtIndex(1)->GetType() == ShapeType::Triangle);

		history.Redo();
		BOOST_CHECK(model.GetShapeAtIndex(1)->GetRect() == rect0);
		BOOST_CHECK(model.GetShapeAtIndex(1)->GetType() == ShapeType::Triangle);
		BOOST_CHECK(canvas.GetShapeAtIndex(1)->GetRect() == rect0);
		BOOST_CHECK(canvas.GetShapeAtIndex(1)->GetType() == ShapeType::Triangle);
	}
	//
	BOOST_AUTO_TEST_CASE(document_undo_redo)
	{
		BOOST_CHECK(!doc.CanUndo() && !doc.CanRedo());

		auto & canvas = doc.GetCanvas();
		canvas.AddShape(ShapeType::Rectangle, rect0);
		BOOST_CHECK(VerifyExpectedShapes(
			{ { ShapeType::Rectangle, rect0 } }));

		canvas.AddShape(ShapeType::Triangle, rect15);
		BOOST_CHECK(VerifyExpectedShapes(
			{ {ShapeType::Rectangle, rect0},{ShapeType::Triangle, rect15} }));

		BOOST_CHECK(doc.CanUndo() && !doc.CanRedo());
		doc.Undo();
		BOOST_CHECK(VerifyExpectedShapes(
			{ { ShapeType::Rectangle, rect0 } }));

		BOOST_CHECK(doc.CanUndo() && doc.CanRedo());
		doc.Undo();
		BOOST_CHECK(VerifyExpectedShapes({}));

		BOOST_CHECK(!doc.CanUndo() && doc.CanRedo());
		doc.Undo();
		BOOST_CHECK(VerifyExpectedShapes({}));
		
		BOOST_CHECK(!doc.CanUndo() && doc.CanRedo());
		doc.Redo();
		BOOST_CHECK(VerifyExpectedShapes(
			{ { ShapeType::Rectangle, rect0 } }));
		
		BOOST_CHECK(doc.CanUndo() && doc.CanRedo());
		doc.Redo();
		BOOST_CHECK(VerifyExpectedShapes(
			{ { ShapeType::Rectangle, rect0 },{ ShapeType::Triangle, rect15 } }));

		BOOST_CHECK(doc.CanUndo() && !doc.CanRedo());
		doc.Redo();
		BOOST_CHECK(VerifyExpectedShapes(
			{ { ShapeType::Rectangle, rect0 },{ ShapeType::Triangle, rect15 } }));
	}
	//
	BOOST_AUTO_TEST_CASE(document_delete)
	{
		auto & canvas = doc.GetCanvas();
		canvas.AddShape(ShapeType::Rectangle, rect0);
		canvas.AddShape(ShapeType::Triangle, rect15);
		BOOST_CHECK(VerifyExpectedShapes(
			{ { ShapeType::Rectangle, rect0 },{ ShapeType::Triangle, rect15 } }));

		canvas.SelectShape(canvas.GetShapeAtIndex(0));
		canvas.DeleteSelection();
		BOOST_CHECK(VerifyExpectedShapes(
			{ { ShapeType::Triangle, rect15 } }));

		canvas.SelectShape(canvas.GetShapeAtIndex(0));
		canvas.DeleteSelection();
		BOOST_CHECK(VerifyExpectedShapes({}));

		doc.Undo();
		BOOST_CHECK(VerifyExpectedShapes(
			{ { ShapeType::Triangle, rect15 } }));
		
		doc.Undo();
		BOOST_CHECK(VerifyExpectedShapes(
			{ { ShapeType::Rectangle, rect0 },{ ShapeType::Triangle, rect15 } }));

		doc.Undo();
		BOOST_CHECK(VerifyExpectedShapes(
			{ { ShapeType::Rectangle, rect0 }}));

		doc.Undo();
		BOOST_CHECK(VerifyExpectedShapes({}));

		doc.Redo();
		BOOST_CHECK(VerifyExpectedShapes(
			{ { ShapeType::Rectangle, rect0 } }));
	}
	//
	BOOST_AUTO_TEST_CASE(document_change)
	{
		auto & canvas = doc.GetCanvas();
		canvas.AddShape(ShapeType::Rectangle, rect0);
		canvas.AddShape(ShapeType::Triangle, rect15);
		BOOST_CHECK(VerifyExpectedShapes(
			{ { ShapeType::Rectangle, rect0 },{ ShapeType::Triangle, rect15 } }));

		canvas.SelectShape(canvas.GetShapeAtIndex(0));
		auto shape = canvas.GetSelection();
		shape->SetRect(rect15);
		BOOST_CHECK(VerifyExpectedShapes(
			{ { ShapeType::Rectangle, rect0 },{ ShapeType::Triangle, rect15 } },
			{ { ShapeType::Rectangle, rect15 },{ ShapeType::Triangle, rect15 } }
		));

		shape->Commit();
		BOOST_CHECK(VerifyExpectedShapes(
			{ { ShapeType::Rectangle, rect15 },{ ShapeType::Triangle, rect15 } },
			{ { ShapeType::Rectangle, rect15 },{ ShapeType::Triangle, rect15 } }
		));

		shape->Offset(CPoint(-15, -15), OffsetType::BottomRight);
		BOOST_CHECK(VerifyExpectedShapes(
			{ { ShapeType::Rectangle, rect15 },{ ShapeType::Triangle, rect15 } },
			{ { ShapeType::Rectangle, rect0 },{ ShapeType::Triangle, rect15 } }
		));

		shape->Commit();
		BOOST_CHECK(VerifyExpectedShapes(
			{ { ShapeType::Rectangle, rect0 },{ ShapeType::Triangle, rect15 } },
			{ { ShapeType::Rectangle, rect0 },{ ShapeType::Triangle, rect15 } }
		));

		doc.Undo();
		BOOST_CHECK(VerifyExpectedShapes(
			{ { ShapeType::Rectangle, rect15 },{ ShapeType::Triangle, rect15 } },
			{ { ShapeType::Rectangle, rect15 },{ ShapeType::Triangle, rect15 } }
		));

		doc.Undo();
		BOOST_CHECK(VerifyExpectedShapes(
			{ { ShapeType::Rectangle, rect0 },{ ShapeType::Triangle, rect15 } },
			{ { ShapeType::Rectangle, rect0 },{ ShapeType::Triangle, rect15 } }
		));

		doc.Undo();
		BOOST_CHECK(VerifyExpectedShapes(
			{ { ShapeType::Rectangle, rect0 } },
			{ { ShapeType::Rectangle, rect0 } }
		));

		doc.Redo();
		BOOST_CHECK(VerifyExpectedShapes(
			{ { ShapeType::Rectangle, rect0 },{ ShapeType::Triangle, rect15 } },
			{ { ShapeType::Rectangle, rect0 },{ ShapeType::Triangle, rect15 } }
		));
	}
BOOST_AUTO_TEST_SUITE_END()
