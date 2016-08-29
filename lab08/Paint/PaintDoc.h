#pragma once
#include "Shape.h"
#include "History.h"

class CPaintDoc : public CDocument
{
public:
	virtual ~CPaintDoc();
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

	void CreateRectangle(CRect const & rect, bool useHistory = true);
	void CreateTriangle(CRect const & rect, bool useHistory = true);
	void CreateEllipse(CRect const & rect, bool useHistory = true);
	void OffsetShape(std::shared_ptr<CShape> const & shape, CPoint const & delta, OffsetType type, bool useHistory = false);
	void DeleteShape(std::shared_ptr<CShape> const & shape, bool useHistory = true);
	
	std::shared_ptr<CShape> GetShapeContainingPoint(CPoint const & point) const;
	CRect GetFrameRectOfShape(std::shared_ptr<CShape> const & shape) const;
	std::function<void(CDC*)> DrawShapes() const;
	CSize GetDocSize() const;

	void Undo();
	void Redo();
	bool CanUndo() const;
	bool CanRedo() const;

private:
	void Create(std::shared_ptr<CShape> const & shape);
	void Create(std::shared_ptr<CShape> const & shape, size_t position);
	void Delete();
	void Delete(size_t position);
	void Offset(std::shared_ptr<CShape> const & shape, CPoint const & delta, OffsetType type);
	
	std::vector<std::shared_ptr<CShape>> m_shapes;
	CHistory m_history;

protected:
	CPaintDoc();
	DECLARE_DYNCREATE(CPaintDoc)
	DECLARE_MESSAGE_MAP()
#ifdef SHARED_HANDLERS
	void SetSearchContent(const CString& value);
#endif

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
