#include "stdafx.h"

#ifndef SHARED_HANDLERS
#include "Paint2.h"
#endif

#include "Paint2Doc.h"
#include <propkey.h>
#include "Canvas.h"
#include "Doc.h"
#include "IPaint2View.h"
#include "XmlReader.h"
#include "XmlWriter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CPaint2Doc, CDocument)

BEGIN_MESSAGE_MAP(CPaint2Doc, CDocument)
END_MESSAGE_MAP()

CPaint2Doc::CPaint2Doc()
/*
	: m_view(nullptr)
*/
{
}

CPaint2Doc::~CPaint2Doc()
{
}

BOOL CPaint2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	NewDocument();

	return TRUE;
}

void CPaint2Doc::Serialize(CArchive & ar)
{
	if (ar.IsStoring())
	{
		m_doc->Save([&ar](ICanvas const & canvas) {
			CXmlWriter writer;
			writer.SaveCanvas(canvas, ar);
		});
	}
	else
	{
		NewDocument();

		m_doc->Load([&ar](ICanvas & canvas) {
			CXmlReader reader;
			reader.LoadCanvas(canvas, ar);
		});
	}
}

std::shared_ptr<::IDocument> CPaint2Doc::GetDoc()
{
	return m_doc;
}

/*
void CPaint2Doc::InitView(IPaint2View * view)
{
	m_view = view;
	m_view->Initialize();
}
*/

void CPaint2Doc::NewDocument()
{
	m_doc = std::make_shared<CDoc>(std::make_unique<CCanvas>());
	auto & editableCanvas = m_doc->GetEditableCanvas();

	editableCanvas.DoOnInsertShape([this](std::shared_ptr<IEditableShape> const & /*shape*/, boost::optional<size_t> /*position*/) {
		SetModifiedFlag();
	});
	editableCanvas.DoOnDeleteShape([this](std::shared_ptr<IEditableShape> const & /*shape*/) {
		SetModifiedFlag();
	});
	editableCanvas.DoOnChangeShape([this](std::shared_ptr<IEditableShape> const & /*shape*/) {
		SetModifiedFlag();
	});

// /*
// 	if (m_view)
// 	{
// 		m_view->Initialize();
// 	}
// */
}

#ifdef SHARED_HANDLERS
void CPaint2Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
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

void CPaint2Doc::InitializeSearchContent()
{
	CString strSearchContent;
	SetSearchContent(strSearchContent);
}

void CPaint2Doc::SetSearchContent(const CString& value)
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

#ifdef _DEBUG
void CPaint2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPaint2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
