#include "stdafx.h"
#include "Compound.h"
#include "Cone.h"
#include "Cylinder.h"
#include "Parallelepiped.h"
#include "Sphere.h"
#include "BodyConstructor.h"

using namespace std;

static const string idRoot = "root";

CBodyConstructor::CBodyConstructor(istream & is)
	: m_is(is)
{
	m_bodies.emplace(idRoot, Body());
	m_bodies[idRoot].isConsider = false;
}

void CBodyConstructor::CreateBody(BodyType body)
{
	string id;
	m_is >> id;
	auto it = m_bodies.find(id);
	if (it != m_bodies.end())
	{
		cout << "Invalid id\n";
		while (cin.get() != '\n')
		{}
		return;
	}

	bool isCreated = false;
	if (body == BodyType::Compound)
	{
		isCreated = CreateCompound(id);
	}
	else if (body == BodyType::Cone)
	{
		isCreated = CreateCone(id);
	}
	else if (body == BodyType::Cylinder)
	{
		isCreated = CreateCylinder(id);
	}
	else if (body == BodyType::Parallelepiped)
	{
		isCreated = CreateParallelepiped(id);
	}
	else if (body == BodyType::Sphere)
	{
		isCreated = CreateSphere(id);
	}

	if (!isCreated)
	{
		cout << "Invalid arguments\n";
	}
}

bool CBodyConstructor::GetArguments(vector<double> & args, uint8_t argCount)
{
	string str;
	getline(m_is, str);
	stringstream strm;
	strm << str;
	for (double arg = 0; strm >> arg;)
	{
		args.push_back(arg);
	}

	bool isNegative = any_of(args.begin(), args.end(),
		[](double x) { return x < 0.0; });

	return (strm.eof() && 
			args.size() == argCount &&
			!isNegative) ? true : false;
}

bool CBodyConstructor::CreateCompound(string const & id)
{
	vector<double> args;
	if (GetArguments(args, 0))
	{
		m_bodies.emplace(id, Body(make_shared<CCompound>
			(CCompound(id))));
		return true;
	}

	return false;
}

bool CBodyConstructor::CreateCone(string const & id)
{
	vector<double> args;
	if (GetArguments(args, 3))
	{
		m_bodies.emplace(id, Body(make_shared<CCone>
			(CCone(id, args[0], args[1], args[2]))));
		return true;
	}

	return false;
}

bool CBodyConstructor::CreateCylinder(string const & id)
{
	vector<double> args;
	if (GetArguments(args, 3))
	{
		m_bodies.emplace(id, Body(make_shared<CCylinder>
			(CCylinder(id, args[0], args[1], args[2]))));
		return true;
	}

	return false;
}

bool CBodyConstructor::CreateParallelepiped(string const & id)
{
	vector<double> args;
	if (GetArguments(args, 4))
	{
		m_bodies.emplace(id, Body(make_shared<CParallelepiped>
			(CParallelepiped(id, args[0], args[1], args[2], args[3]))));
		return true;
	}

	return false;
}

bool CBodyConstructor::CreateSphere(string const & id)
{
	vector<double> args;
	if (GetArguments(args, 2))
	{
		m_bodies.emplace(id, Body(make_shared<CSphere>
			(CSphere(id, args[0], args[1]))));
		return true;
	}

	return false;
}

void CBodyConstructor::MoveBody()
{
	string str;
	getline(m_is, str);
	stringstream strm;
	strm << str;
	string idMoved, idReceiver;
	strm >> idMoved >> idReceiver;

	auto iterMoved = m_bodies.find(idMoved);
	auto iterReceiver = m_bodies.find(idReceiver);
	if (iterMoved != m_bodies.end() && iterReceiver != m_bodies.end() && strm.eof())
	{
		auto receiver = dynamic_pointer_cast<CCompound>(iterReceiver->second.body);
		if (iterMoved->second.parent != receiver)
		{
			if ((receiver && idMoved != idRoot && receiver->AddBody(iterMoved->second.body)) 
				|| idReceiver == idRoot)
			{
				auto parent = dynamic_pointer_cast<CCompound>(iterMoved->second.parent);
				if (parent)
				{
					parent->RemoveBody(iterMoved->second.body);
				}
				iterMoved->second.parent = receiver;
				iterMoved->second.isConsider = (receiver) ? false : true;

				return;
			}
		}

		cout << "Moving impossible\n";
	}
	else
	{
		cout << "Invalid id\n";
	}
}

void CBodyConstructor::BodiesList() const
{
	cout << ToStringBodies();
}


std::shared_ptr<CBody> CBodyConstructor::GetBodyMaxMass() const
{
	auto itMaxMass = max_element(m_bodies.begin(), m_bodies.end(),
		[](auto const & lhs, auto const & rhs)
	{
		if (lhs.second.isConsider && rhs.second.isConsider)
		{
			return lhs.second.body->GetMass() < rhs.second.body->GetMass();
		}
		else if (lhs.second.isConsider)
		{
			return false;
		}
		else if (rhs.second.isConsider)
		{
			return true;
		}
		return false;
	});

	return itMaxMass->second.body;
}

std::shared_ptr<CBody> CBodyConstructor::GetBodyMinMass() const
{
	auto itMinMass = min_element(m_bodies.begin(), m_bodies.end(),
		[](auto const & lhs, auto const & rhs)
	{
		if (lhs.second.isConsider && rhs.second.isConsider)
		{
			return lhs.second.body->GetMass() < rhs.second.body->GetMass();
		}
		else if (lhs.second.isConsider)
		{
			return true;
		}
		else if (rhs.second.isConsider)
		{
			return false;
		}
		return false;
	});

	return itMinMass->second.body;
}

std::shared_ptr<CBody> CBodyConstructor::GetBodyMinWeightInWater() const
{
	auto itMinWeight = min_element(m_bodies.begin(), m_bodies.end(),
		[](auto const & lhs, auto const & rhs)
	{
		if (lhs.second.isConsider && rhs.second.isConsider)
		{
			return (lhs.second.body->GetMass() - lhs.second.body->GetVolume()) <
				   (rhs.second.body->GetMass() - rhs.second.body->GetVolume());
		}
		else if (lhs.second.isConsider)
		{
			return true;
		}
		else if (rhs.second.isConsider)
		{
			return false;
		}
		return false;
	});

	return itMinWeight->second.body;
}

string CBodyConstructor::ToStringBodies() const
{
	auto bodyMaxMass = GetBodyMaxMass();
	auto bodyMinMass = GetBodyMinMass();
	auto bodyMinWeightInWater = GetBodyMinWeightInWater();

	string result;
	for (auto const & body : m_bodies)
	{
		if (body.second.isConsider)
		{
			if (body.second.body == bodyMaxMass)
			{
				result += "+ ";
			}
			if (body.second.body == bodyMinMass)
			{
				result += "- ";
			}
			if (body.second.body == bodyMinWeightInWater)
			{
				result += "^ ";
			}
				
			result += body.second.body->ToString();
		}
	}
	
	return result;
}
