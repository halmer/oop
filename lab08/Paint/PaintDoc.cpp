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
{
}

CPaintDoc::~CPaintDoc()
{
}

BOOL CPaintDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_shapes.clear();
	m_history.Reset();
	UpdateAllViews(NULL, 1);

	return TRUE;
}

void CPaintDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		pugi::xml_document doc;
		pugi::xml_node node = doc.append_child("Shapes");

		for (auto const & shape : m_shapes)
		{
			CRect rect = shape->GetFrameRect();
			rect.NormalizeRect();
			pugi::xml_node param = node.append_child("Shape");
			param.append_attribute("type") = shape->GetType().data();
			param.append_attribute("left") = rect.left;
			param.append_attribute("top") = rect.top;
			param.append_attribute("right") = rect.right;
			param.append_attribute("bottom") = rect.bottom;
		}

		stringstream strm;
		doc.save(strm);
		ar.Write(strm.str().data(), strm.str().size());
	}
	else
	{
		CFile * file = ar.GetFile();
		pugi::xml_document doc;
		if (!doc.load_file((LPCTSTR)file->GetFilePath()))//-V2005
		{
			return;
		}

		pugi::xml_node shapes = doc.child("Shapes");
		if (shapes)
		{
			m_shapes.clear();
			m_history.Reset();
		}

		for (auto const & shape : shapes.children("Shape"))
		{
			stringstream strm;
			strm <<
				shape.attribute("type").value() << " " <<
				shape.attribute("left").value() << " " <<
				shape.attribute("top").value() << " " <<
				shape.attribute("right").value() << " " <<
				shape.attribute("bottom").value();
			string type;
			int left = 0, top = 0, right = 0, bottom = 0;
			strm >> type >> left >> top >> right >> bottom;
			if (strm)
			{
				if (type == "Rectangle")
				{
					CreateRectangle(CRect(left, top, right, bottom), false);
				}
				else if (type == "Triangle")
				{
					CreateTriangle(CRect(left, top, right, bottom), false);
				}
				else if (type == "Ellipse")
				{
					CreateEllipse(CRect(left, top, right, bottom), false);
				}
			}
		}
		UpdateAllViews(NULL, 1);
	}
}

void CPaintDoc::CreateRectangle(CRect const & rect, bool useHistory /*= true*/)
{
	if (useHistory)
	{
		m_history.AddCommandAndExecute({
			bind(static_cast<void(CPaintDoc::*)(shared_ptr<CShape> const &)>(&CPaintDoc::Create), this, make_shared<CRectangle>(rect)),
			bind(static_cast<void(CPaintDoc::*)()>(&CPaintDoc::Delete), this)
		});
	}
	else
	{
		Create(make_shared<CRectangle>(rect));
	}

	SetModifiedFlag();
	UpdateAllViews(NULL);
}

void CPaintDoc::CreateTriangle(CRect const & rect, bool useHistory /*= true*/)
{
	if (useHistory)
	{
		m_history.AddCommandAndExecute({
			bind(static_cast<void(CPaintDoc::*)(shared_ptr<CShape> const &)>(&CPaintDoc::Create), this, make_shared<CTriangle>(rect)),
			bind(static_cast<void(CPaintDoc::*)()>(&CPaintDoc::Delete), this)
		});
	}
	else
	{
		Create(make_shared<CTriangle>(rect));
	}

	SetModifiedFlag();
	UpdateAllViews(NULL);
}

void CPaintDoc::CreateEllipse(CRect const & rect, bool useHistory /*= true*/)
{
	if (useHistory)
	{
		m_history.AddCommandAndExecute({
			bind(static_cast<void(CPaintDoc::*)(shared_ptr<CShape> const &)>(&CPaintDoc::Create), this, make_shared<CEllipse>(rect)),
			bind(static_cast<void(CPaintDoc::*)()>(&CPaintDoc::Delete), this)
		});
	}
	else
	{
		Create(make_shared<CEllipse>(rect));
	}

	SetModifiedFlag();
	UpdateAllViews(NULL);
}

void CPaintDoc::OffsetShape(shared_ptr<CShape> const & shape, CPoint const & delta, OffsetType type, bool useHistory /* = false*/)
{
	if (useHistory)
	{
		m_history.AddCommand({
			bind(&CPaintDoc::Offset, this, shape, delta, type),
			bind(&CPaintDoc::Offset, this, shape, -delta, type)
		});
	}
	else
	{
		Offset(shape, delta, type);

		SetModifiedFlag();
		UpdateAllViews(NULL, 2);
	}
}

void CPaintDoc::DeleteShape(shared_ptr<CShape> const & shape, bool useHistory /*= true*/)
{
	auto it = find(m_shapes.begin(), m_shapes.end(), shape);
	size_t position = distance(m_shapes.begin(), it);

	if (useHistory)
	{
		m_history.AddCommandAndExecute({
			bind(static_cast<void(CPaintDoc::*)(size_t)>(&CPaintDoc::Delete), this, position),
			bind(static_cast<void(CPaintDoc::*)(shared_ptr<CShape> const &, size_t)>(&CPaintDoc::Create), this, shape, position)
		});
	}
	else
	{
		Delete(position);
	}

	SetModifiedFlag();
	UpdateAllViews(NULL, 1);
}

shared_ptr<CShape> CPaintDoc::GetShapeContainingPoint(CPoint const & point) const
{
	for (auto const & shape : boost::adaptors::reverse(m_shapes))
	{
		if (shape && shape->IsPointInShape(point))
		{
			return shape;
		}
	}

	return nullptr;
}

CRect CPaintDoc::GetFrameRectOfShape(std::shared_ptr<CShape> const & shape) const
{
	return shape->GetFrameRect();
}

function<void(CDC*)> CPaintDoc::DrawShapes() const
{	
	return [this](CDC * pDC) {
		for (auto const & shape : m_shapes)
		{
			shape->DrawShape()(pDC);
		}
	};
}

CSize CPaintDoc::GetDocSize() const
{
	int maxRight = 0;
	int maxBottom = 0;

	for (auto const & shape : m_shapes)
	{
		CPoint point = shape->GetBottomRight();
		maxRight = max(maxRight, point.x);
		maxBottom = max(maxBottom, point.y);
	}

	return { maxRight, maxBottom };
}

void CPaintDoc::Undo()
{
	m_history.Undo();

	SetModifiedFlag();
	UpdateAllViews(NULL, 1);
}

void CPaintDoc::Redo()
{
	m_history.Redo();

	SetModifiedFlag();
	UpdateAllViews(NULL, 1);
}

bool CPaintDoc::CanUndo() const
{
	return m_history.CanUndo();
}

bool CPaintDoc::CanRedo() const
{
	return m_history.CanRedo();
}

void CPaintDoc::Create(shared_ptr<CShape> const & shape)
{
	m_shapes.push_back(shape);
}

void CPaintDoc::Create(shared_ptr<CShape> const & shape, size_t position)
{
	m_shapes.insert(m_shapes.begin() + position, shape);
}

void CPaintDoc::Delete()
{
	m_shapes.pop_back();
}

void CPaintDoc::Delete(size_t position)
{
	m_shapes.erase(m_shapes.begin() + position);
}

void CPaintDoc::Offset(shared_ptr<CShape> const & shape, CPoint const & delta, OffsetType type)
{
	shape->Offset(delta, type);
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
