#include "stdafx.h"
#include "../task41/Cone.h"

struct Cone_
{
	const double expectedRadius = 42.8;
	const double expectedHeight = 12.41;
	const double expectedDensity = 2.1;
	const double expectedVolume = 23806.08;
	const CCone  cone;
	Cone_()
		: cone("id", expectedDensity, expectedRadius, expectedHeight)
	{}
};
// Сфера
BOOST_FIXTURE_TEST_SUITE(Cone, Cone_)
	// является объемным телом
	BOOST_AUTO_TEST_CASE(is_a_body)
	{
		BOOST_CHECK(static_cast<const CBody*>(&cone));
	}
	// имеет радиус основания
	BOOST_AUTO_TEST_CASE(has_a_radius_of_base)
	{
		BOOST_CHECK_EQUAL(cone.GetRadius(), expectedRadius);
	}
	// имеет высоту
	BOOST_AUTO_TEST_CASE(has_a_height)
	{
		BOOST_CHECK_EQUAL(cone.GetHeight(), expectedHeight);
	}
	// имеет плотность
	BOOST_AUTO_TEST_CASE(has_a_density)
	{
		BOOST_CHECK_EQUAL(static_cast<const CBody &>(cone).GetDensity(), expectedDensity);
	}
	// имеет объем
	BOOST_AUTO_TEST_CASE(has_a_volume)
	{
		BOOST_CHECK_CLOSE_FRACTION(static_cast<const CBody &>(cone).GetVolume(), expectedVolume, 1e-6);
	}
	// имеет массу
	BOOST_AUTO_TEST_CASE(has_a_mass)
	{
		BOOST_CHECK_CLOSE_FRACTION(static_cast<const CBody &>(cone).GetMass(), expectedVolume * expectedDensity, 1e-6);
	}
	// имеет строковое представление
	BOOST_AUTO_TEST_CASE(can_be_converted_to_string)
	{
		const auto expectedString = R"(Cone id:
	density = 2.1
	volume = 23806.08267
	mass = 49992.77362
	radius = 42.8
	height = 12.41
)";
		BOOST_CHECK_EQUAL(static_cast<const CBody &>(cone).ToString(), expectedString);
	}
BOOST_AUTO_TEST_SUITE_END()
