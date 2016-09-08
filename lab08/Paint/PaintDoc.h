#pragma once
#include "Shape.h"
#include "History.h"
#include "Model.h"
#include "IPaintView.h"
#include "XmlWriter.h"
#include "XmlReader.h"

class CPaintDoc : public CDocument
{
public:
	virtual ~CPaintDoc();
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

	void InitViewSignal(IPaintView * view);

	void CreateShape(ShapeType type, CRect const & rect);
	void OffsetSelectedShape(OffsetType type, CPoint const & delta);
	void DeleteSelectedShape();
	void DeleteAllShapes();
	void SetSelectedShape(std::shared_ptr<CShape> const & shape);

	void Undo();
	void Redo();
	bool CanUndo() const;
	bool CanRedo() const;

	std::vector<std::shared_ptr<CShape>> const & GetShapesData() const;

private:
	void AddShape(std::shared_ptr<CShape> const & shape, boost::optional<size_t> position);
	void DeleteShape(boost::optional<size_t> position);
	void DeleteShapes();
	void ChangeShape(std::shared_ptr<CShape> const & shape, size_t position);
	
	void OffsetShape(OffsetType type, CPoint const & currPoint);
	void EndingOffsetShape(OffsetType type);
	void SetModifiedFlagAndUpdateView(LPARAM lHint);

	std::vector<std::shared_ptr<CShape>> m_shapes;
	CModel & m_model;
	IPaintView * m_view;
	CXmlWriter m_writer;
	CXmlReader m_reader;
	CHistory m_history;
	
	std::shared_ptr<CShape> m_offsetShape;
	std::shared_ptr<CShape> m_selectedShape;
	boost::optional<CPoint> m_firstPoint;
	boost::optional<CPoint> m_lastPoint;
	std::vector<std::shared_ptr<CShape>>::iterator m_itSelectedShape;

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
