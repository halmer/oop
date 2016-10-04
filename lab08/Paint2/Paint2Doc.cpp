 #include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Paint2.h"
#endif

#include "Paint2Doc.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CPaint2Doc, CDocument)

BEGIN_MESSAGE_MAP(CPaint2Doc, CDocument)
END_MESSAGE_MAP()

CPaint2Doc::CPaint2Doc()
{
	auto & editableCanvas = theApp.m_doc.GetCanvas();
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

CPaint2Doc::~CPaint2Doc()
{
}

BOOL CPaint2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	theApp.m_doc.NewDocument();

	return TRUE;
}

void CPaint2Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
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

// Support for Search Handlers
void CPaint2Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
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

#endif // SHARED_HANDLERS

#ifdef _DEBUG
void CPaint2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPaint2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG
