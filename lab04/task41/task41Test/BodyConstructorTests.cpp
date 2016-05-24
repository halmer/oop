#include "stdafx.h"
#include "../task41/BodyConstructor.h"

using namespace std;

bool IsEqualVectors(vector<double> const & args, vector<double> const & expectedArgs)
{
	if (args.size() != expectedArgs.size())
	{
		return false;
	}
	return equal(args.begin(), args.end(), expectedArgs.begin());
}

struct BodyConstructor_
{
	BodyConstructor_()
		: constructor(is)
	{}

	stringstream is;
	CBodyConstructor constructor;
};
//����������� ���
BOOST_FIXTURE_TEST_SUITE(BodyConstructor, BodyConstructor_)
	//��������� ������������ ������� ����������
	BOOST_AUTO_TEST_CASE(validates_input_arguments)
	{
		{
			is << "\n";
			vector<double> args;
			BOOST_CHECK(constructor.GetArguments(args, 0));
			BOOST_CHECK(IsEqualVectors(args, {}));
		}
		{
			is << "\n";
			vector<double> args;
			BOOST_CHECK(!constructor.GetArguments(args, 1));
		}
		{
			is << "1 2 a\n";
			vector<double> args;
			BOOST_CHECK(!constructor.GetArguments(args, 2));
		}
		{
			is << "a 1 2 3\n";
			vector<double> args;
			BOOST_CHECK(!constructor.GetArguments(args, 3));
		}
		{
			is << "1 2.8 3 4\n";
			vector<double> args;
			BOOST_CHECK(constructor.GetArguments(args, 4));
			BOOST_CHECK(IsEqualVectors(args, { 1, 2.8, 3, 4}));
		}
		{//���������� ������
			is << "1 2 3\n";
			vector<double> args;
			BOOST_CHECK(!constructor.GetArguments(args, 4));
		}
		{//���������� ������ 
			is << "1 2 3 4 5\n";
			vector<double> args;
			BOOST_CHECK(!constructor.GetArguments(args, 4));
		}
		{//������������� ��������
			is << "1 2 -3 4\n";
			vector<double> args;
			BOOST_CHECK(!constructor.GetArguments(args, 4));
		}
	}
	//����� ������� �����
	BOOST_AUTO_TEST_CASE(can_create_cone)
	{
		is << "id 10 10 10\n";
		constructor.CreateBody(BodyType::Cone);
		auto expectedString = R"(+ - ^ Cone id:
	density = 10
	volume = 1047.197551
	mass = 10471.97551
	radius = 10
	height = 10
)";
		BOOST_CHECK_EQUAL(constructor.ToStringBodies(), expectedString);
	}
	//����� ������� �������
	BOOST_AUTO_TEST_CASE(can_create_cylinder)
	{
		is << "id 10 10 10\n";
		constructor.CreateBody(BodyType::Cylinder);
		auto expectedString = R"(+ - ^ Cylinder id:
	density = 10
	volume = 3141.592654
	mass = 31415.92654
	radius = 10
	height = 10
)";
		BOOST_CHECK_EQUAL(constructor.ToStringBodies(), expectedString);
	}
	//����� ������� ������������
	BOOST_AUTO_TEST_CASE(can_create_parallelepiped)
	{
		is << "id 10 10 10 10\n";
		constructor.CreateBody(BodyType::Parallelepiped);
		auto expectedString = R"(+ - ^ Parallelepiped id:
	density = 10
	volume = 1000
	mass = 10000
	width = 10
	height = 10
	depth = 10
)";
		BOOST_CHECK_EQUAL(constructor.ToStringBodies(), expectedString);
	}
	//����� ������� �����
	BOOST_AUTO_TEST_CASE(can_create_sphere)
	{
		is << "id 10 10\n";
		constructor.CreateBody(BodyType::Sphere);
		auto expectedString = R"(+ - ^ Sphere id:
	density = 10
	volume = 4188.790205
	mass = 41887.90205
	radius = 10
)";
		BOOST_CHECK_EQUAL(constructor.ToStringBodies(), expectedString);
	}
	//����� ������� ��������� ����
	BOOST_AUTO_TEST_CASE(can_create_compound)
	{
		is << "id\n";
		constructor.CreateBody(BodyType::Compound);
		auto expectedString = R"(+ - ^ Compound id:
	density = 0
	volume = 0
	mass = 0
)";
		BOOST_CHECK_EQUAL(constructor.ToStringBodies(), expectedString);
	}
	//����� ���������� ����(� ��������� ��� � ������)
	BOOST_AUTO_TEST_CASE(can_move_bodeis)
	{
		//������� ���������(compound) � �����
		is << "compound\n" << "cone 10 10 10\n";
		constructor.CreateBody(BodyType::Compound);
		constructor.CreateBody(BodyType::Cone);
		auto expectedString = R"(- ^ Compound compound:
	density = 0
	volume = 0
	mass = 0
+ Cone cone:
	density = 10
	volume = 1047.197551
	mass = 10471.97551
	radius = 10
	height = 10
)";
		BOOST_CHECK_EQUAL(constructor.ToStringBodies(), expectedString);

		//����������� ����� � compound
		is << "cone compound\n";
		constructor.MoveBody();
		expectedString = R"(+ - ^ Compound compound:
	density = 10
	volume = 1047.197551
	mass = 10471.97551
	* Cone cone:
		density = 10
		volume = 1047.197551
		mass = 10471.97551
		radius = 10
		height = 10
)";
		BOOST_CHECK_EQUAL(constructor.ToStringBodies(), expectedString);
		
		//������� �����
		is << "sphere 10 10\n";
		constructor.CreateBody(BodyType::Sphere);
		expectedString = R"(- ^ Compound compound:
	density = 10
	volume = 1047.197551
	mass = 10471.97551
	* Cone cone:
		density = 10
		volume = 1047.197551
		mass = 10471.97551
		radius = 10
		height = 10
+ Sphere sphere:
	density = 10
	volume = 4188.790205
	mass = 41887.90205
	radius = 10
)";
		BOOST_CHECK_EQUAL(constructor.ToStringBodies(), expectedString);

		//����������� ����� � compound
		is << "sphere compound\n";
		constructor.MoveBody();
		expectedString = R"(+ - ^ Compound compound:
	density = 10
	volume = 5235.987756
	mass = 52359.87756
	* Cone cone:
		density = 10
		volume = 1047.197551
		mass = 10471.97551
		radius = 10
		height = 10
	* Sphere sphere:
		density = 10
		volume = 4188.790205
		mass = 41887.90205
		radius = 10
)";
		BOOST_CHECK_EQUAL(constructor.ToStringBodies(), expectedString);

		//����������� ����� � ������
		is << "cone root\n";
		constructor.MoveBody();
		expectedString = R"(+ Compound compound:
	density = 10
	volume = 4188.790205
	mass = 41887.90205
	* Sphere sphere:
		density = 10
		volume = 4188.790205
		mass = 41887.90205
		radius = 10
- ^ Cone cone:
	density = 10
	volume = 1047.197551
	mass = 10471.97551
	radius = 10
	height = 10
)";
		BOOST_CHECK_EQUAL(constructor.ToStringBodies(), expectedString);

		//������� ���������(compound1) � �������
		//����������� ������� � compound1, ����� compound � compound1
		is << "compound1\n" << "cylinder 5 5 5\n" 
		   << "cylinder compound1\n" << "compound compound1\n";
		constructor.CreateBody(BodyType::Compound);
		constructor.CreateBody(BodyType::Cylinder);
		constructor.MoveBody();
		constructor.MoveBody();
		expectedString = R"(+ Compound compound1:
	density = 9.571428571
	volume = 4581.489286
	mass = 43851.39746
	* Compound compound:
		density = 10
		volume = 4188.790205
		mass = 41887.90205
		* Sphere sphere:
			density = 10
			volume = 4188.790205
			mass = 41887.90205
			radius = 10
	* Cylinder cylinder:
		density = 5
		volume = 392.6990817
		mass = 1963.495408
		radius = 5
		height = 5
- ^ Cone cone:
	density = 10
	volume = 1047.197551
	mass = 10471.97551
	radius = 10
	height = 10
)";
		BOOST_CHECK_EQUAL(constructor.ToStringBodies(), expectedString);

		//����������� ����� � compound
		is << "cone compound\n";
		constructor.MoveBody();
		expectedString = R"(+ - ^ Compound compound1:
	density = 9.651162791
	volume = 5628.686838
	mass = 54323.37297
	* Compound compound:
		density = 10
		volume = 5235.987756
		mass = 52359.87756
		* Cone cone:
			density = 10
			volume = 1047.197551
			mass = 10471.97551
			radius = 10
			height = 10
		* Sphere sphere:
			density = 10
			volume = 4188.790205
			mass = 41887.90205
			radius = 10
	* Cylinder cylinder:
		density = 5
		volume = 392.6990817
		mass = 1963.495408
		radius = 5
		height = 5
)";
		BOOST_CHECK_EQUAL(constructor.ToStringBodies(), expectedString);

		//����������� ����� � compound1
		is << "cone compound1\n";
		constructor.MoveBody();
		expectedString = R"(+ - ^ Compound compound1:
	density = 9.651162791
	volume = 5628.686838
	mass = 54323.37297
	* Compound compound:
		density = 10
		volume = 4188.790205
		mass = 41887.90205
		* Sphere sphere:
			density = 10
			volume = 4188.790205
			mass = 41887.90205
			radius = 10
	* Cone cone:
		density = 10
		volume = 1047.197551
		mass = 10471.97551
		radius = 10
		height = 10
	* Cylinder cylinder:
		density = 5
		volume = 392.6990817
		mass = 1963.495408
		radius = 5
		height = 5
)";
		BOOST_CHECK_EQUAL(constructor.ToStringBodies(), expectedString);

		//����������� ����� � ������, ������� � ������, compound � ������
		is << "cone root\n" << "cylinder root\n" << "compound root\n";
		constructor.MoveBody();
		constructor.MoveBody();
		constructor.MoveBody();
		expectedString = R"(+ Compound compound:
	density = 10
	volume = 4188.790205
	mass = 41887.90205
	* Sphere sphere:
		density = 10
		volume = 4188.790205
		mass = 41887.90205
		radius = 10
- ^ Compound compound1:
	density = 0
	volume = 0
	mass = 0
Cone cone:
	density = 10
	volume = 1047.197551
	mass = 10471.97551
	radius = 10
	height = 10
Cylinder cylinder:
	density = 5
	volume = 392.6990817
	mass = 1963.495408
	radius = 5
	height = 5
)";
		BOOST_CHECK_EQUAL(constructor.ToStringBodies(), expectedString);
	}
	//��������� ������������ ����������� ���
	BOOST_AUTO_TEST_CASE(validates_moving_bodies)
	{
		//������� compound, compound1, cone
		is << "compound\n" << "compound1\n" << "cone 10 10 10\n";
		constructor.CreateBody(BodyType::Compound);
		constructor.CreateBody(BodyType::Compound);
		constructor.CreateBody(BodyType::Cone);
		auto expectedString = R"(- ^ Compound compound:
	density = 0
	volume = 0
	mass = 0
Compound compound1:
	density = 0
	volume = 0
	mass = 0
+ Cone cone:
	density = 10
	volume = 1047.197551
	mass = 10471.97551
	radius = 10
	height = 10
)";
		BOOST_CHECK_EQUAL(constructor.ToStringBodies(), expectedString);
		
		//������ compound1 ����������� � cone ��� � root
		is << "compound1 cone\n" << "compound1 root\n";
		constructor.MoveBody();//Moving impossible
		constructor.MoveBody();//Moving impossible
		BOOST_CHECK_EQUAL(constructor.ToStringBodies(), expectedString);

		//���������� compound1 � compound, � cone � compound1
		is << "compound1 compound\n" << "cone compound1\n";
		constructor.MoveBody();
		constructor.MoveBody();
		expectedString = R"(+ - ^ Compound compound:
	density = 10
	volume = 1047.197551
	mass = 10471.97551
	* Compound compound1:
		density = 10
		volume = 1047.197551
		mass = 10471.97551
		* Cone cone:
			density = 10
			volume = 1047.197551
			mass = 10471.97551
			radius = 10
			height = 10
)";
		BOOST_CHECK_EQUAL(constructor.ToStringBodies(), expectedString);

		//������ �����������: ���� ��� ���� ����� ����; � ����� ��������;
		//�������������� ����; ���� � ����; �� � ��������� ����;
		is << "compound1 compound\n" << "compound compound1\n"
			<< "noexist compound1\n" << "compound1 noexist\n"
			<< "compound1 compound1\n" << "compound1 cone\n";
		constructor.MoveBody();//Moving impossible
		constructor.MoveBody();//Moving impossible
		constructor.MoveBody();//Invalid id
		constructor.MoveBody();//Invalid id
		constructor.MoveBody();//Moving impossible
		constructor.MoveBody();//Moving impossible
		BOOST_CHECK_EQUAL(constructor.ToStringBodies(), expectedString);
		
		//������� compound2 � ����������� � compound1. ����������� cone � compound2
		is << "compound2\n" << "compound2 compound1\n" << "cone compound2\n";
		constructor.CreateBody(BodyType::Compound);
		constructor.MoveBody();
		constructor.MoveBody();
		expectedString = R"(+ - ^ Compound compound:
	density = 10
	volume = 1047.197551
	mass = 10471.97551
	* Compound compound1:
		density = 10
		volume = 1047.197551
		mass = 10471.97551
		* Compound compound2:
			density = 10
			volume = 1047.197551
			mass = 10471.97551
			* Cone cone:
				density = 10
				volume = 1047.197551
				mass = 10471.97551
				radius = 10
				height = 10
)";
		BOOST_CHECK_EQUAL(constructor.ToStringBodies(), expectedString);
	}
BOOST_AUTO_TEST_SUITE_END()
