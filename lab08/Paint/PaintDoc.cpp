#include "stdafx.h"

#ifndef SHARED_HANDLERS
#include "Paint.h"
#endif

#include "PaintDoc.h"
#include <propkey.h>
#include "Rectangle.h"
#include "Triangle.h"
#include "Ellipse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

IMPLEMENT_DYNCREATE(CPaintDoc, CDocument)

BEGIN_MESSAGE_MAP(CPaintDoc, CDocument)
END_MESSAGE_MAP()

CPaintDoc::CPaintDoc()
	: m_model(theApp.m_model)
	, m_view(nullptr)
	, m_writer(this)
	, m_reader(this)
{
	m_model.DoOnAddShape(std::bind(&CPaintDoc::AddShape, this, placeholders::_1, placeholders::_2));
	m_model.DoOnDeleteShape(std::bind(&CPaintDoc::DeleteShape, this, placeholders::_1));
	m_model.DoOnDeleteAllShapes(std::bind(&CPaintDoc::DeleteShapes, this));
	m_model.DoOnOffsetShape(std::bind(&CPaintDoc::ChangeShape, this, placeholders::_1, placeholders::_2));
}

void CPaintDoc::InitViewSignal(IPaintView * view)
{
	if (m_view)
	{
		return;
	}

	m_view = view;

	m_view->DoOnButtonRectangle(std::bind(&CPaintDoc::CreateShape, this, ShapeType::Rectangle, placeholders::_1));
	m_view->DoOnButtonTriangle(std::bind(&CPaintDoc::CreateShape, this, ShapeType::Triangle, placeholders::_1));
	m_view->DoOnButtonEllipse(std::bind(&CPaintDoc::CreateShape, this, ShapeType::Ellipse, placeholders::_1));
	m_view->DoOnShapeSelection(std::bind(&CPaintDoc::SetSelectedShape, this, placeholders::_1));
	m_view->DoOnMouseMoveWithLButtonDown(std::bind(&CPaintDoc::OffsetShape, this, placeholders::_1, placeholders::_2));
	m_view->DoOnLButtonUp(std::bind(&CPaintDoc::EndingOffsetShape, this, placeholders::_1));
	m_view->DoOnButtonUndo(std::bind(&CPaintDoc::Undo, this));
	m_view->DoOnButtonRedo(std::bind(&CPaintDoc::Redo, this));
	m_view->DoOnKeyDeleteDown(std::bind(&CPaintDoc::DeleteSelectedShape, this));
}

CPaintDoc::~CPaintDoc()
{
}

BOOL CPaintDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	DeleteAllShapes();

	return TRUE;
}

void CPaintDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		m_writer.WriteData(ar);
	}
	else
	{
		m_reader.ReadData(ar);
		m_history.Reset();

		UpdateAllViews(NULL, REDRAW_UPDATESCROLL_RESETSELECT);
	}
}

void CPaintDoc::CreateShape(ShapeType type, CRect const & rect)
{
	shared_ptr<CShape> shape;
	if (type == ShapeType::Rectangle)
	{
		shape = make_shared<CRectangle>(rect);
	}
	else if (type == ShapeType::Triangle)
	{
		shape = make_shared<CTriangle>(rect);
	}
	else if (type == ShapeType::Ellipse)
	{
		shape = make_shared<CEllipse>(rect);
	}
	else
	{
		return;
	}

	m_history.AddCommand({
		std::bind(&CModel::AddShape, &m_model, shape, boost::none),
		std::bind(&CModel::DeleteShape, &m_model, boost::none)
	});

	m_model.AddShape(shape, boost::none);
}

void CPaintDoc::OffsetSelectedShape(OffsetType type, CPoint const & delta)
{
	if (!m_selectedShape)
	{
		return;
	}
	
	m_history.AddCommand({
		std::bind(&CModel::OffsetShape, &m_model, m_selectedShape, delta, type),
		std::bind(&CModel::OffsetShape, &m_model, m_selectedShape, -delta, type)
	});

	m_model.OffsetShape(m_selectedShape, delta, type);
}

void CPaintDoc::DeleteSelectedShape()
{
	if (!m_selectedShape)
	{
		return;
	}
	
	auto it = find(m_shapes.begin(), m_shapes.end(), m_selectedShape);
	if (it == m_shapes.end())
	{
		return;
	}
	size_t position = distance(m_shapes.begin(), it);

	m_history.AddCommand({
		std::bind(&CModel::DeleteShape, &m_model, position),
		std::bind(&CModel::AddShape, &m_model, m_selectedShape, position)
	});

	m_model.DeleteShape(position);
}

void CPaintDoc::DeleteAllShapes()
{
	m_history.Reset();
	m_model.DeleteAllShapes();
}

void CPaintDoc::SetSelectedShape(std::shared_ptr<CShape> const & shape)
{
	m_selectedShape = shape;
}

void CPaintDoc::Undo()
{
	m_history.Undo();

	SetModifiedFlagAndUpdateView(REDRAW_UPDATESCROLL_RESETSELECT);
}

void CPaintDoc::Redo()
{
	m_history.Redo();

	SetModifiedFlagAndUpdateView(REDRAW_UPDATESCROLL_RESETSELECT);
}

bool CPaintDoc::CanUndo() const
{
	return m_history.CanUndo();
}

bool CPaintDoc::CanRedo() const
{
	return m_history.CanRedo();
}

vector<shared_ptr<CShape>> const & CPaintDoc::GetShapesData() const
{
	return m_shapes;
}

void CPaintDoc::AddShape(std::shared_ptr<CShape> const & shape, boost::optional<size_t> position)
{
	if (position)
	{
		assert(*position <= m_shapes.size());
		m_shapes.insert(m_shapes.begin() + *position, shape);
	}
	else
	{
		m_shapes.push_back(shape);
	}

	SetModifiedFlagAndUpdateView(REDRAW_UPDATESCROLL);
}

void CPaintDoc::DeleteShape(boost::optional<size_t> position)
{
	if (position)
	{
		assert(*position < m_shapes.size());
		m_shapes.erase(m_shapes.begin() + *position);
	}
	else
	{
		m_shapes.pop_back();
	}

	SetModifiedFlagAndUpdateView(REDRAW_UPDATESCROLL_RESETSELECT);
}

void CPaintDoc::DeleteShapes()
{
	m_shapes.clear();

	UpdateAllViews(NULL, REDRAW_UPDATESCROLL_RESETSELECT);
}

void CPaintDoc::ChangeShape(std::shared_ptr<CShape> const & shape, size_t position)
{
	assert(position < m_shapes.size());
	m_shapes[position] = shape;

	SetModifiedFlagAndUpdateView(REDRAW_UPDATESCROLL);
}

void CPaintDoc::OffsetShape(OffsetType type, CPoint const & currPoint)
{
	if (!m_lastPoint)
	{
		m_firstPoint = currPoint;
		m_lastPoint = currPoint;
	}

	if (!m_offsetShape)
	{
		m_itSelectedShape = find(m_shapes.begin(), m_shapes.end(), m_selectedShape);
		if (m_itSelectedShape == m_shapes.end())
		{
			return;
		}

		m_offsetShape = (*m_itSelectedShape)->Clone();
		*m_itSelectedShape = m_offsetShape;
		m_view->SetMovingShape(m_offsetShape);
	}

	m_offsetShape->Offset(currPoint - *m_lastPoint, type);
	m_lastPoint = currPoint;

	UpdateAllViews(NULL, REDRAW);
}

void CPaintDoc::EndingOffsetShape(OffsetType type)
{
	if (!m_offsetShape)
	{
		return;
	}

	if (*m_lastPoint != *m_firstPoint)
	{
		OffsetSelectedShape(type, *m_lastPoint - *m_firstPoint);
	}
	else
	{
		*m_itSelectedShape = m_selectedShape;
	}

	m_offsetShape = nullptr;
	m_firstPoint = boost::none;
	m_lastPoint = boost::none;
}

void CPaintDoc::SetModifiedFlagAndUpdateView(LPARAM lHint)
{
	SetModifiedFlag();
	UpdateAllViews(NULL, lHint);
}

#ifdef _DEBUG
void CPaintDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPaintDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif

#ifdef SHARED_HANDLERS
void CPaintDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

void CPaintDoc::InitializeSearchContent()
{
	CString strSearchContent;
	SetSearchContent(strSearchContent);
}

void CPaintDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif
