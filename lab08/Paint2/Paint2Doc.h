#pragma once
#include "XmlReader.h"
#include "XmlWriter.h"
#include "Doc.h"

class CPaint2Doc : public CDocument
{
public:
	virtual ~CPaint2Doc();
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

protected:
	CPaint2Doc();
	DECLARE_DYNCREATE(CPaint2Doc)
	DECLARE_MESSAGE_MAP()
#ifdef SHARED_HANDLERS
	void SetSearchContent(const CString& value);
#endif

private:
	CXmlReader m_reader; // TODO: move to Serialize
	CXmlWriter m_writer; // TODO: move to Serialize

	std::unique_ptr<::IDocument> m_doc;

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif
};
