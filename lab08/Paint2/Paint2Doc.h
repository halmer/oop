#pragma once

class IDocument;
class IPaint2View;

class CPaint2Doc : public CDocument
{
public:
	virtual ~CPaint2Doc();
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive & ar);
	std::shared_ptr<::IDocument> GetDoc();
/*
	void InitView(IPaint2View * view);
*/

protected:
	CPaint2Doc();
	DECLARE_DYNCREATE(CPaint2Doc)
	DECLARE_MESSAGE_MAP()
#ifdef SHARED_HANDLERS
	void SetSearchContent(const CString& value);
#endif

private:
	void NewDocument();
	
	std::shared_ptr<::IDocument> m_doc;
/*
	IPaint2View * m_view;
*/

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
