#include "stdafx.h"
#include "Body.h"

using namespace std;

CBody::CBody(string const & type, string const & id)
	: m_type(type)
	, m_id(id)
{
}

CBody::~CBody()
{
}

string CBody::ToString() const
{
	ostringstream strm;
	strm << m_type << " " << m_id << ":" << endl << setprecision(10)
		<< "\tdensity = " << GetDensity() << endl
		<< "\tvolume = " << GetVolume() << endl
		<< "\tmass = " << GetMass() << endl;
	AppendProperties(strm);
	return strm.str();
}
