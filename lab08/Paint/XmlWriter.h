#pragma once

class CPaintDoc;

class CXmlWriter
{
public:
	CXmlWriter(CPaintDoc * pDoc);
	void WriteData(CArchive & ar);

private:
	CPaintDoc * m_pDoc;
};
