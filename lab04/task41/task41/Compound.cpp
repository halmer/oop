#include "stdafx.h"
#include "Compound.h"

using namespace std;

CCompound::CCompound(string const & id)
	: CBody("Compound", id)
{
}

double CCompound::GetDensity() const
{
	if (GetVolume() > 0.0)
	{
		return GetMass() / GetVolume();
	}
	
	return 0.0;
}

double CCompound::GetVolume() const
{
	double volume = 0;
	for (auto const & body : m_bodies)
	{
		volume += body->GetVolume();
	}

	return volume;
}

double CCompound::GetMass() const
{
	double mass = 0;
	for (auto const & body : m_bodies)
	{
		mass += body->GetMass();
	}

	return mass;
}

bool CCompound::AddBody(shared_ptr<CBody> const & body)
{
	auto it = m_bodies.find(body);
	if (it != m_bodies.end())
	{
		return false;
	}

	if (IsContainedIn(body))
	{
		return false;
	}

	m_bodies.insert(body);

	return true;
}

bool CCompound::RemoveBody(shared_ptr<CBody> const & body)
{
	auto it = m_bodies.find(body);
	if (it != m_bodies.end())
	{
		m_bodies.erase(it);
		return true;
	}

	return false;
}

void CCompound::AppendProperties(ostream & strm) const
{
	for (auto const & body : m_bodies)
	{
		bool isAddTab = true;
		strm << "\t* ";
		for (auto ch : body->ToString())
		{
			if (ch != '\t')
			{
				strm << ch;
				isAddTab = true;
			}
			else if (isAddTab)
			{
				strm << "\t\t";
				isAddTab = false;
			}
			else
			{
				strm << ch;
			}
		}
	}
}

bool CCompound::IsContainedIn(shared_ptr<CBody> const & body) const
{
	auto compound = dynamic_pointer_cast<CCompound>(body);
	if (compound)
	{
		if (this == compound.get())
		{
			return true;
		}

		for (auto const & item : compound->m_bodies)
		{
			if (IsContainedIn(item))
			{
				return true;
			}
		}
	}

	return false;
}
