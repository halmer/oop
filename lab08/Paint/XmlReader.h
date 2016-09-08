#pragma once

class CPaintDoc;

class CXmlReader
{
public:
	CXmlReader(CPaintDoc * pDoc);
	void ReadData(CArchive & ar);

private:
	CPaintDoc * m_pDoc;
};
