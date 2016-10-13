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
{
}

CPaint2Doc::~CPaint2Doc()
{
}

BOOL CPaint2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	m_doc = std::make_shared<CDoc>(std::make_unique<CCanvas>());
	SubscribeSignals();

	return TRUE;
}

BOOL CPaint2Doc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	return TRUE;
}

void CPaint2Doc::Serialize(CArchive & ar)
{
	if (ar.IsStoring())
	{
		CXmlWriter writer;
		writer.SaveCanvas(m_doc->GetCanvas(), ar);
	}
	else
	{
		CXmlReader reader;	
		m_doc = std::make_shared<CDoc>(reader.LoadCanvas(ar));
		
		SubscribeSignals();
	}
}

std::shared_ptr<::IDocument> CPaint2Doc::GetDoc()
{
	return m_doc;
}

void CPaint2Doc::SubscribeSignals()
{
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

