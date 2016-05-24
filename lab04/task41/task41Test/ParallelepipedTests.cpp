#include "stdafx.h"
#include "../task41/Parallelepiped.h"

struct Parallelepiped_
{
	const double expectedWidth = 17.45;
	const double expectedHeight = 36.9;
	const double expectedDepth = 5.3;
	const double expectedDensity = 18.77;
	const double expectedVolume = 3412.69;
	const CParallelepiped  parallelepiped;
	Parallelepiped_()
		: parallelepiped("id", expectedDensity, expectedWidth, expectedHeight, expectedDepth)
	{}
};
// Параллелепипед
BOOST_FIXTURE_TEST_SUITE(Parallelepiped, Parallelepiped_)
	// является объемным телом
	BOOST_AUTO_TEST_CASE(is_a_body)
	{
		BOOST_CHECK(static_cast<const CBody*>(&parallelepiped));
	}
	// имеет ширину
	BOOST_AUTO_TEST_CASE(has_a_width)
	{
		BOOST_CHECK_EQUAL(parallelepiped.GetWidth(), expectedWidth);
	}
	// имеет высоту
	BOOST_AUTO_TEST_CASE(has_a_height)
	{
		BOOST_CHECK_EQUAL(parallelepiped.GetHeight(), expectedHeight);
	}
	// имеет глубину
	BOOST_AUTO_TEST_CASE(has_a_depth)
	{
		BOOST_CHECK_EQUAL(parallelepiped.GetDepth(), expectedDepth);
	}
	// имеет плотность
	BOOST_AUTO_TEST_CASE(has_a_density)
	{
		BOOST_CHECK_EQUAL(static_cast<const CBody &>(parallelepiped).GetDensity(), expectedDensity);
	}
	// имеет объем
	BOOST_AUTO_TEST_CASE(has_a_volume)
	{
		BOOST_CHECK_CLOSE_FRACTION(static_cast<const CBody &>(parallelepiped).GetVolume(), expectedVolume, 1e-5);
	}
	// имеет массу
	BOOST_AUTO_TEST_CASE(has_a_mass)
	{
		BOOST_CHECK_CLOSE_FRACTION(static_cast<const CBody &>(parallelepiped).GetMass(), expectedVolume * expectedDensity, 1e-5);
	}
	// имеет строковое представление
	BOOST_AUTO_TEST_CASE(can_be_converted_to_string)
	{
		const auto expectedString = R"(Parallelepiped id:
	density = 18.77
	volume = 3412.6965
	mass = 64056.3133
	width = 17.45
	height = 36.9
	depth = 5.3
)";
		BOOST_CHECK_EQUAL(static_cast<const CBody &>(parallelepiped).ToString(), expectedString);
	}
BOOST_AUTO_TEST_SUITE_END()
