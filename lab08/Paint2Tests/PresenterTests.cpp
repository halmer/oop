#include "stdafx.h"
#include "../Paint2/CanvasPresenter.h"
#include "../Paint2/CanvasView.h"
#include "../Paint2/Canvas.h"
#include "../Paint2/Doc.h"
#include "../Paint2/EditableShape.h"
#include "../Paint2/EditableCanvas.h"

using namespace std;
//
//struct PresenterTests_
//{
//	PresenterTests_()
//		: doc(canvas)
//		, presenter(doc, canvasView)
//	{}
//	CCanvas canvas;
//	CDoc doc;
//	CCanvasView canvasView;
//	CCanvasPresenter presenter;
//
//	CRect rect0 = CRect(0, 0, 0, 0);
//	CRect rect15 = CRect(0, 0, 15, 15);
//};
//
//BOOST_FIXTURE_TEST_SUITE(PresenterTests, PresenterTests_)
//	//
//	BOOST_AUTO_TEST_CASE(test1)
//	{
//		BOOST_CHECK_EQUAL(canvasView.GetShapeCount(), 0);
//		//presenter.OnCreateRectangle();
//		//BOOST_CHECK_EQUAL(canvasView.GetShapeCount(), 1);
//		//auto shape = doc.GetCanvas().GetShapeAtIndex(0);
//		//BOOST_CHECK_EQUAL(shape->GetRect(), rect15);
//		//BOOST_CHECK(shape->GetType() == ShapeType::Rectangle);
//	}
//BOOST_AUTO_TEST_SUITE_END()
