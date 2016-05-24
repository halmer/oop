#include "stdafx.h"
#include "../task41/Cylinder.h"

struct Cylinder_
{
	const double expectedRadius = 42.8;
	const double expectedHeight = 19.3;
	const double expectedDensity = 24.5;
	const double expectedVolume = 111069.47;
	const CCylinder сylinder;
	Cylinder_()
		: сylinder("id", expectedDensity, expectedRadius, expectedHeight)
	{}
};
// Цилиндр
BOOST_FIXTURE_TEST_SUITE(Cylinder, Cylinder_)
	// является объемным телом
	BOOST_AUTO_TEST_CASE(is_a_body)
	{
		BOOST_CHECK(static_cast<const CBody*>(&сylinder));
	}
	// имеет радиус основания
	BOOST_AUTO_TEST_CASE(has_a_radius_of_base)
	{
		BOOST_CHECK_EQUAL(сylinder.GetRadius(), expectedRadius);
	}
	// имеет высоту
	BOOST_AUTO_TEST_CASE(has_a_height)
	{
		BOOST_CHECK_EQUAL(сylinder.GetHeight(), expectedHeight);
	}
	// имеет плотность
	BOOST_AUTO_TEST_CASE(has_a_density)
	{
		BOOST_CHECK_EQUAL(static_cast<const CBody &>(сylinder).GetDensity(), expectedDensity);
	}
	// имеет объем
	BOOST_AUTO_TEST_CASE(has_a_volume)
	{
		BOOST_CHECK_CLOSE_FRACTION(static_cast<const CBody &>(сylinder).GetVolume(), expectedVolume, 1e-6);
	}
	// имеет массу
	BOOST_AUTO_TEST_CASE(has_a_mass)
	{
		BOOST_CHECK_CLOSE_FRACTION(static_cast<const CBody &>(сylinder).GetMass(), expectedVolume * expectedDensity, 1e-6);
	}
	// имеет строковое представление
	BOOST_AUTO_TEST_CASE(can_be_converted_to_string)
	{
		const auto expectedString = R"(Cylinder id:
	density = 24.5
	volume = 111069.4752
	mass = 2721202.142
	radius = 42.8
	height = 19.3
)";
		BOOST_CHECK_EQUAL(static_cast<const CBody &>(сylinder).ToString(), expectedString);
	}
BOOST_AUTO_TEST_SUITE_END()
