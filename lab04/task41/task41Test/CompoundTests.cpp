#include "stdafx.h"
#include "../task41/Compound.h"
#include "../task41/Cone.h"
#include "../task41/Parallelepiped.h"
#include "../task41/Sphere.h"

using namespace std;

struct Compound_
{
	Compound_()
		: compound("compound")
	{}

	const double densitySphere = 8.8;
	const double radiusSphere = 25.4;
	const double volumeSphere = (pow(radiusSphere, 3) * M_PI) * 4 / 3;

	const double densityCone = 2.1;
	const double radiusCone = 36.8;
	const double heightCone = 12.41;
	const double volumeCone = (pow(radiusCone, 2) * M_PI * heightCone) / 3;

	const double expectedMass = volumeSphere * densitySphere + volumeCone * densityCone;
	const double expectedVolume = volumeSphere + volumeCone;
	const double expectedDensity = expectedMass / expectedVolume;
	
	CCompound compound;
};
//��������� ����
BOOST_FIXTURE_TEST_SUITE(Compound, Compound_)
	// �������� �������� �����
	BOOST_AUTO_TEST_CASE(is_a_body)
	{
		BOOST_CHECK(static_cast<const CBody*>(&compound));
	}
	//����� ���������
	BOOST_AUTO_TEST_CASE(has_a_density)
	{
		//������
		BOOST_CHECK_EQUAL(static_cast<const CBody &>(compound).GetDensity(), 0.0);

		//�������� �����
		BOOST_CHECK(compound.AddBody(make_shared<CSphere>("sphere", densitySphere, radiusSphere)));
		BOOST_CHECK_EQUAL(static_cast<const CBody &>(compound).GetDensity(), densitySphere);

		//�������� �����
		BOOST_CHECK(compound.AddBody(make_shared<CCone>("cone", densityCone, radiusCone, heightCone)));
		BOOST_CHECK_EQUAL(static_cast<const CBody &>(compound).GetDensity(), expectedDensity);
	}
	//����� �����
	BOOST_AUTO_TEST_CASE(has_a_volume)
	{	
		//������
		BOOST_CHECK_EQUAL(static_cast<const CBody &>(compound).GetVolume(), 0.0);

		//�������� �����
		BOOST_CHECK(compound.AddBody(make_shared<CSphere>("sphere", densitySphere, radiusSphere)));
		BOOST_CHECK_EQUAL(static_cast<const CBody &>(compound).GetVolume(), volumeSphere);

		//�������� �����
		BOOST_CHECK(compound.AddBody(make_shared<CCone>("cone", densityCone, radiusCone, heightCone)));
		BOOST_CHECK_EQUAL(static_cast<const CBody &>(compound).GetVolume(), expectedVolume);
	}
	//����� �����
	BOOST_AUTO_TEST_CASE(has_a_mass)
	{
		//������
		BOOST_CHECK_EQUAL(static_cast<const CBody &>(compound).GetMass(), 0.0);

		//�������� �����
		BOOST_CHECK(compound.AddBody(make_shared<CSphere>("sphere", densitySphere, radiusSphere)));
		BOOST_CHECK_EQUAL(static_cast<const CBody &>(compound).GetMass(), volumeSphere * densitySphere);

		//�������� �����
		BOOST_CHECK(compound.AddBody(make_shared<CCone>("cone", densityCone, radiusCone, heightCone)));
		BOOST_CHECK_EQUAL(static_cast<const CBody &>(compound).GetMass(), expectedMass);
	}
	//����� ��������� �������������
	BOOST_AUTO_TEST_CASE(can_be_converted_to_string)
	{
		//������
		auto expectedString = R"(Compound compound:
	density = 0
	volume = 0
	mass = 0
)";	
		BOOST_CHECK_EQUAL(static_cast<const CBody &>(compound).ToString(), expectedString);

		//�������� �����
		BOOST_CHECK(compound.AddBody(make_shared<CSphere>("sphere", densitySphere, radiusSphere)));
		expectedString = R"(Compound compound:
	density = 8.8
	volume = 68641.97317
	mass = 604049.3639
	* Sphere sphere:
		density = 8.8
		volume = 68641.97317
		mass = 604049.3639
		radius = 25.4
)";
		BOOST_CHECK_EQUAL(static_cast<const CBody &>(compound).ToString(), expectedString);

		//�������� �����
		BOOST_CHECK(compound.AddBody(make_shared<CCone>("cone", densityCone, radiusCone, heightCone)));
		expectedString = R"(Compound compound:
	density = 7.43272602
	volume = 86241.2992
	mass = 641007.9486
	* Sphere sphere:
		density = 8.8
		volume = 68641.97317
		mass = 604049.3639
		radius = 25.4
	* Cone cone:
		density = 2.1
		volume = 17599.32603
		mass = 36958.58467
		radius = 36.8
		height = 12.41
)";
		BOOST_CHECK_EQUAL(static_cast<const CBody &>(compound).ToString(), expectedString);
	}
	//����� �������� � ������� ����
	BOOST_AUTO_TEST_CASE(can_add_and_delete_body)
	{
		//��������� �������������
		auto parallelepiped = make_shared<CParallelepiped>("p", 1, 10, 10, 10);
		BOOST_CHECK(compound.AddBody(parallelepiped));
		auto expectedString = R"(Compound compound:
	density = 1
	volume = 1000
	mass = 1000
	* Parallelepiped p:
		density = 1
		volume = 1000
		mass = 1000
		width = 10
		height = 10
		depth = 10
)";
		BOOST_CHECK_EQUAL(compound.ToString(), expectedString);

		//��������� ��� �� �������������
		BOOST_CHECK(!compound.AddBody(parallelepiped));
		BOOST_CHECK_EQUAL(compound.ToString(), expectedString);

		//������� ���� �������� ��� � ��������� ����
		BOOST_CHECK(!compound.RemoveBody(make_shared<CCone>("c", 1, 2, 3)));
		BOOST_CHECK_EQUAL(compound.ToString(), expectedString);

		//������� ������������
		BOOST_CHECK(compound.RemoveBody(parallelepiped));
		expectedString = R"(Compound compound:
	density = 0
	volume = 0
	mass = 0
)";
		BOOST_CHECK_EQUAL(compound.ToString(), expectedString);
	}
	//�� ����� ���� ��������� � ����� ��������
	BOOST_AUTO_TEST_CASE(can_not_added_to_their_children)
	{
		auto compound1 = make_shared<CCompound>("compound1");
		auto compound2 = make_shared<CCompound>("compound2");
		auto compound3 = make_shared<CCompound>("compound3");
		auto parallelepiped = make_shared<CParallelepiped>("p", 1, 10, 10, 10);
		
		BOOST_CHECK(compound.AddBody(compound1));
		BOOST_CHECK(compound1->AddBody(compound2));
		BOOST_CHECK(compound2->AddBody(compound3));
		BOOST_CHECK(compound2->AddBody(parallelepiped));
		auto expectedString = R"(Compound compound:
	density = 1
	volume = 1000
	mass = 1000
	* Compound compound1:
		density = 1
		volume = 1000
		mass = 1000
		* Compound compound2:
			density = 1
			volume = 1000
			mass = 1000
			* Compound compound3:
				density = 0
				volume = 0
				mass = 0
			* Parallelepiped p:
				density = 1
				volume = 1000
				mass = 1000
				width = 10
				height = 10
				depth = 10
)";
		BOOST_CHECK_EQUAL(compound.ToString(), expectedString);
		BOOST_CHECK(!compound3->AddBody(compound1));
		BOOST_CHECK_EQUAL(compound.ToString(), expectedString);

		//��������� ��������� ���� ���� � ����
		BOOST_CHECK(!compound1->AddBody(compound1));
		BOOST_CHECK_EQUAL(compound.ToString(), expectedString);
	}
BOOST_AUTO_TEST_SUITE_END()
