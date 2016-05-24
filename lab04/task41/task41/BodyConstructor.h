#pragma once
#include "Body.h"

enum class BodyType
{
	Compound,
	Cone,
	Cylinder,
	Parallelepiped,
	Sphere
};

class CBodyConstructor
{
public:
	CBodyConstructor(std::istream & is);

	void CreateBody(BodyType body);
	void MoveBody();
	void BodiesList() const;

	bool GetArguments(std::vector<double> & args, uint8_t argCount);
	std::string ToStringBodies() const;

private:
	struct Body
	{
		Body() = default;
		Body(std::shared_ptr<CBody> const & ptr)
			: body(ptr)
		{}

		std::shared_ptr<CBody> body = nullptr;
		std::shared_ptr<CBody> parent = nullptr;
		bool isConsider = true;
	};

	bool CreateCompound(std::string const & id);
	bool CreateCone(std::string const & id);
	bool CreateCylinder(std::string const & id);
	bool CreateParallelepiped(std::string const & id);
	bool CreateSphere(std::string const & id);
	std::shared_ptr<CBody> GetBodyMaxMass() const;
	std::shared_ptr<CBody> GetBodyMinMass() const;
	std::shared_ptr<CBody> GetBodyMinWeightInWater() const;

	std::map<std::string, Body> m_bodies;
	std::istream & m_is;
};
