#include "stdafx.h"
#include "../task34/CalcProcessor.h"

using namespace std;

struct CalcProcessor_
{
	bool VerifyExpectedValue(string const & id, double expectedValue)
	{
		auto val = calc.GetValue(id);
		if (!val)
		{
			return false;
		}
		if (isnan(expectedValue))
		{
			return isnan(*val);
		}
		if (isnan(*val))
		{
			return false;
		}
		BOOST_REQUIRE_CLOSE_FRACTION(*val, expectedValue, 1e-6);
		return true;
	}
	bool VerifyExpectedIdsValues(map<string, double> const & vals, map<string, double> const & expectedVals)
	{
		if (vals.size() != expectedVals.size())
		{
			return false;
		}
		auto expectedVal = expectedVals.begin();
		for (auto const & val : vals)
		{
			if (val.first != expectedVal->first)
			{
				return false;
			}
			if (isnan(val.second) && !isnan(expectedVal->second))
			{
				return false;
			}
			if (isnan(expectedVal->second) && !isnan(val.second))
			{
				return false;
			}
			if (!isnan(val.second) && !isnan(expectedVal->second))
			{
				BOOST_REQUIRE_CLOSE_FRACTION(val.second, expectedVal->second, 1e-6);
			}
			++expectedVal;
		}
		return true;
	}
	CCalcProcessor calc;
	string x = "x";
	string y = "y";
	string f = "f";
	string f1 = "f1";
	string f2 = "f2";
	string wrong = "wrong";
};

BOOST_FIXTURE_TEST_SUITE(CalcProcessor, CalcProcessor_)
	//
	BOOST_AUTO_TEST_CASE(create_variable_with_value_NAN)
	{
		BOOST_CHECK(calc.CreateVar(x));
		BOOST_CHECK(VerifyExpectedValue(x, NAN));
	}
	//
	BOOST_AUTO_TEST_CASE(variable_identifier_must_be_unique)
	{
		BOOST_CHECK(calc.CreateVar(x));
		BOOST_CHECK(!calc.CreateVar(x));
		BOOST_CHECK(VerifyExpectedValue(x, NAN));
	}
	//
	BOOST_AUTO_TEST_CASE(can_assigns_variable_numeric_value)
	{
		BOOST_CHECK(calc.CreateVar(x));

		BOOST_CHECK(calc.SetVar(x, 0));
		BOOST_CHECK(VerifyExpectedValue(x, 0));

		BOOST_CHECK(calc.SetVar(x, 12.34));
		BOOST_CHECK(VerifyExpectedValue(x, 12.34));

		BOOST_CHECK(calc.SetVar(x, -12345));
		BOOST_CHECK(VerifyExpectedValue(x, -12345));
	}
	//
	BOOST_AUTO_TEST_CASE(can_assigns_variable_current_value_of_previously_declared_identifier)
	{
		BOOST_CHECK(calc.CreateVar(x));
		BOOST_CHECK(calc.CreateVar(y));
		BOOST_CHECK(calc.SetVar(x, y));
		BOOST_CHECK(VerifyExpectedValue(x, NAN));

		BOOST_CHECK(calc.SetVar(y, 12.34));
		BOOST_CHECK(calc.SetVar(x, y));
		BOOST_CHECK(VerifyExpectedValue(x, 12.34));

		BOOST_CHECK(calc.SetVar(y, 5));
		BOOST_CHECK(VerifyExpectedValue(x, 12.34));

		BOOST_CHECK(calc.SetVar(x, y));
		BOOST_CHECK(VerifyExpectedValue(x, 5));

		BOOST_CHECK(!calc.SetVar(x, wrong));
		BOOST_CHECK(VerifyExpectedValue(x, 5));
	}
	//
	BOOST_AUTO_TEST_CASE(assigns_functions_is_impossible)
	{
		BOOST_CHECK(calc.SetVar(x, 5));
		BOOST_CHECK(calc.SetVar(y, 7));
		BOOST_CHECK(calc.CreateFn(f, x, "+", y));
		BOOST_CHECK(VerifyExpectedValue(f, 12));

		BOOST_CHECK(!calc.SetVar(f, 0));
		BOOST_CHECK(!calc.SetVar(f, x));
		BOOST_CHECK(VerifyExpectedValue(f, 12));
	}
	//
	BOOST_AUTO_TEST_CASE(if_variable_is_not_declared_it_is_declared)
	{
		BOOST_CHECK(calc.GetValue(x) == boost::none);
		BOOST_CHECK(calc.SetVar(x, 5));
		BOOST_CHECK(VerifyExpectedValue(x, 5));
	}
	//
	BOOST_AUTO_TEST_CASE(can_declares_function_whose_value_is_value_of_previously_declared_identifier_in_moment_of_calculation)
	{
		BOOST_CHECK(calc.CreateVar(x));
		BOOST_CHECK(calc.CreateFn(f, x));
		BOOST_CHECK(VerifyExpectedValue(f, NAN));

		BOOST_CHECK(calc.SetVar(x, 12.34));
		BOOST_CHECK(VerifyExpectedValue(f, 12.34));

		BOOST_CHECK(calc.CreateFn(f1, f));
		BOOST_CHECK(VerifyExpectedValue(f1, 12.34));

		BOOST_CHECK(calc.SetVar(x, -5));
		BOOST_CHECK(VerifyExpectedValue(f, -5));
		BOOST_CHECK(VerifyExpectedValue(f1, -5));

		BOOST_CHECK(!calc.CreateFn(f2, wrong));
	}
	//
	BOOST_AUTO_TEST_SUITE(can_declares_function_whose_value_is_result_of_applying_binary_operator_to_values_of_previously_declared_identifiers_at_time_of_calculation)
		//
		BOOST_AUTO_TEST_CASE(operator_plus)
		{
			BOOST_CHECK(calc.CreateVar(x));
			BOOST_CHECK(calc.CreateVar(y));
			BOOST_CHECK(calc.CreateFn(f, x, "+", y));
			BOOST_CHECK(VerifyExpectedValue(f, NAN));

			BOOST_CHECK(calc.SetVar(x, 5));
			BOOST_CHECK(VerifyExpectedValue(f, NAN));

			BOOST_CHECK(calc.SetVar(y, 7));
			BOOST_CHECK(VerifyExpectedValue(f, plus<double>()(5, 7)));

			BOOST_CHECK(calc.SetVar(x, 12.34));
			BOOST_CHECK(VerifyExpectedValue(f, plus<double>()(12.34, 7)));

			BOOST_CHECK(calc.SetVar(y, -123.45));
			BOOST_CHECK(VerifyExpectedValue(f, plus<double>()(12.34, -123.45)));

			BOOST_CHECK(!calc.CreateFn(f1, wrong, "+", x));
		}
		//
		BOOST_AUTO_TEST_CASE(operator_minus)
		{
			BOOST_CHECK(calc.CreateVar(x));
			BOOST_CHECK(calc.CreateVar(y));
			BOOST_CHECK(calc.CreateFn(f, x, "-", y));
			BOOST_CHECK(VerifyExpectedValue(f, NAN));

			BOOST_CHECK(calc.SetVar(x, 5));
			BOOST_CHECK(VerifyExpectedValue(f, NAN));

			BOOST_CHECK(calc.SetVar(y, 7));
			BOOST_CHECK(VerifyExpectedValue(f, minus<double>()(5, 7)));

			BOOST_CHECK(calc.SetVar(x, 12.34));
			BOOST_CHECK(VerifyExpectedValue(f, minus<double>()(12.34, 7)));

			BOOST_CHECK(calc.SetVar(y, -123.45));
			BOOST_CHECK(VerifyExpectedValue(f, minus<double>()(12.34, -123.45)));
		}
		//
		BOOST_AUTO_TEST_CASE(operator_multiplies)
		{
			BOOST_CHECK(calc.CreateVar(x));
			BOOST_CHECK(calc.CreateVar(y));
			BOOST_CHECK(calc.CreateFn(f, x, "*", y));
			BOOST_CHECK(VerifyExpectedValue(f, NAN));

			BOOST_CHECK(calc.SetVar(x, 5));
			BOOST_CHECK(VerifyExpectedValue(f, NAN));

			BOOST_CHECK(calc.SetVar(y, 7));
			BOOST_CHECK(VerifyExpectedValue(f, multiplies<double>()(5, 7)));

			BOOST_CHECK(calc.SetVar(x, 12.34));
			BOOST_CHECK(VerifyExpectedValue(f, multiplies<double>()(12.34, 7)));

			BOOST_CHECK(calc.SetVar(y, -123.45));
			BOOST_CHECK(VerifyExpectedValue(f, multiplies<double>()(12.34, -123.45)));
		}
		//
		BOOST_AUTO_TEST_CASE(operator_divides)
		{
			BOOST_CHECK(calc.CreateVar(x));
			BOOST_CHECK(calc.CreateVar(y));
			BOOST_CHECK(calc.CreateFn(f, x, "/", y));
			BOOST_CHECK(VerifyExpectedValue(f, NAN));

			BOOST_CHECK(calc.SetVar(x, 5));
			BOOST_CHECK(VerifyExpectedValue(f, NAN));

			BOOST_CHECK(calc.SetVar(y, 7));
			BOOST_CHECK(VerifyExpectedValue(f, divides<double>()(5, 7)));

			BOOST_CHECK(calc.SetVar(x, 12.34));
			BOOST_CHECK(VerifyExpectedValue(f, divides<double>()(12.34, 7)));

			BOOST_CHECK(calc.SetVar(y, -123.45));
			BOOST_CHECK(VerifyExpectedValue(f, divides<double>()(12.34, -123.45)));
		}
	BOOST_AUTO_TEST_SUITE_END()
	//
	BOOST_AUTO_TEST_CASE(function_identifier_must_be_unique)
	{
		BOOST_CHECK(calc.SetVar(x, 5));
		BOOST_CHECK(calc.SetVar(y, 7));
		BOOST_CHECK(calc.CreateFn(f, x, "+", y));
		BOOST_CHECK(VerifyExpectedValue(f, 12));

		BOOST_CHECK(!calc.CreateFn(f, x, "/", y));
	}
	//
	BOOST_AUTO_TEST_CASE(can_give_value_of_previously_declared_identifier)
	{
		BOOST_CHECK(calc.GetValue(x) == boost::none);

		BOOST_CHECK(calc.CreateVar(x));
		auto val = calc.GetValue(x);
		BOOST_CHECK(val && isnan(*val));

		BOOST_CHECK(calc.SetVar(x, 5));
		val = calc.GetValue(x);
		BOOST_REQUIRE(val);
		BOOST_REQUIRE_CLOSE_FRACTION(*val, 5, 1e-6);

		BOOST_CHECK(calc.SetVar(x, 123.45));
		val = calc.GetValue(x);
		BOOST_REQUIRE(val);
		BOOST_REQUIRE_CLOSE_FRACTION(*val, 123.45, 1e-6);

		BOOST_CHECK(calc.CreateFn(f, x, "+", x));
		val = calc.GetValue(f);
		BOOST_REQUIRE(val);
		BOOST_REQUIRE_CLOSE_FRACTION(*val, plus<double>()(123.45, 123.45), 1e-6);

		BOOST_CHECK(calc.SetVar(x, -12.34));
		val = calc.GetValue(f);
		BOOST_REQUIRE(val);
		BOOST_REQUIRE_CLOSE_FRACTION(*val, plus<double>()(-12.34, -12.34), 1e-6);

		val = calc.GetValue(wrong);
		BOOST_CHECK(!val);
	}
	//
	BOOST_AUTO_TEST_CASE(can_give_names_and_values_of_all_previously_declared_variables)
	{
		BOOST_CHECK(VerifyExpectedIdsValues(calc.GetVarsValues(), { }));
		
		BOOST_CHECK(calc.CreateVar(x));
		BOOST_CHECK(VerifyExpectedIdsValues(calc.GetVarsValues(), { { "x", NAN } }));

		BOOST_CHECK(calc.SetVar(x, 5));
		BOOST_CHECK(VerifyExpectedIdsValues(calc.GetVarsValues(), { { "x", 5 } }));

		BOOST_CHECK(calc.CreateVar(y));
		BOOST_CHECK(VerifyExpectedIdsValues(calc.GetVarsValues(), { { "x", 5 },{ "y", NAN } }));

		BOOST_CHECK(calc.SetVar(y, 12.34));
		BOOST_CHECK(VerifyExpectedIdsValues(calc.GetVarsValues(), { { "x", 5 },{ "y", 12.34 } }));

		BOOST_CHECK(calc.CreateFn(f, x, "+", y));
		BOOST_CHECK(VerifyExpectedValue(f, plus<double>()(5, 12.34)));
		BOOST_CHECK(VerifyExpectedIdsValues(calc.GetVarsValues(), { { "x", 5 },{ "y", 12.34 } }));
	}
	//
	BOOST_AUTO_TEST_CASE(can_give_names_and_values_of_all_previously_declared_functions)
	{
		BOOST_CHECK(VerifyExpectedIdsValues(calc.GetFnsValues(), {}));

		BOOST_CHECK(calc.CreateVar(x));
		BOOST_CHECK(calc.CreateVar(y));
		BOOST_CHECK(calc.CreateFn(f, x, "+", y));
		BOOST_CHECK(VerifyExpectedIdsValues(calc.GetFnsValues(), { { "f", NAN } }));

		BOOST_CHECK(calc.CreateFn(f1, x, "*", y));
		BOOST_CHECK(VerifyExpectedIdsValues(calc.GetFnsValues(), { { "f", NAN },{ "f1", NAN } }));

		BOOST_CHECK(calc.SetVar(x, 5));
		BOOST_CHECK(calc.SetVar(y, 7));
		BOOST_CHECK(VerifyExpectedIdsValues(calc.GetFnsValues(), { { "f", 12 },{ "f1", 35 } }));
	}
	//
	BOOST_AUTO_TEST_CASE(MalovTest1)
	{
		BOOST_CHECK(calc.CreateVar(x));
		BOOST_CHECK(VerifyExpectedValue(x, NAN));

		BOOST_CHECK(calc.SetVar(x, 42));
		BOOST_CHECK(VerifyExpectedValue(x, 42));

		BOOST_CHECK(calc.SetVar(x, 1.234));
		BOOST_CHECK(VerifyExpectedValue(x, 1.234));

		BOOST_CHECK(calc.SetVar(y, x));
		BOOST_CHECK(calc.SetVar(x, 99));
		BOOST_CHECK(VerifyExpectedIdsValues(calc.GetVarsValues(), { { x, 99 },{ y, 1.234 } }));
	}
	//
	BOOST_AUTO_TEST_CASE(MalovTest2)
	{
		BOOST_CHECK(calc.CreateVar(x));
		BOOST_CHECK(calc.CreateVar(y));
		BOOST_CHECK(calc.CreateFn("XPlusY", x, "+", y));
		BOOST_CHECK(VerifyExpectedValue("XPlusY", NAN));

		BOOST_CHECK(calc.SetVar(x, 3));
		BOOST_CHECK(calc.SetVar(y, 4));
		BOOST_CHECK(VerifyExpectedValue("XPlusY", 7));

		BOOST_CHECK(calc.SetVar(x, 10));
		BOOST_CHECK(VerifyExpectedValue("XPlusY", 14));

		BOOST_CHECK(calc.SetVar("z", 3.5));
		BOOST_CHECK(calc.CreateFn("XPlusYDivZ", "XPlusY", "/", "z"));
		BOOST_CHECK(VerifyExpectedIdsValues(calc.GetFnsValues(), { { "XPlusY", 14 },{ "XPlusYDivZ", 4 } }));
	}
	//
	BOOST_AUTO_TEST_CASE(MalovTest3)
	{
		BOOST_CHECK(calc.SetVar("v", 42));
		BOOST_CHECK(calc.SetVar("variable", "v"));
		BOOST_CHECK(calc.CreateFn("function", "v"));
		BOOST_CHECK(calc.SetVar("v", 43));
		BOOST_CHECK(VerifyExpectedValue("variable", 42));

		BOOST_CHECK(VerifyExpectedValue("function", 43));

	}
	//
	BOOST_AUTO_TEST_CASE(MalovTest4)
	{
		BOOST_CHECK(calc.CreateVar("radius"));
		BOOST_CHECK(calc.SetVar("pi", 3.14159265f));
		BOOST_CHECK(calc.CreateFn("radiusSquared", "radius", "*", "radius"));
		BOOST_CHECK(calc.CreateFn("circleArea", "pi", "*", "radiusSquared"));
		BOOST_CHECK(calc.SetVar("radius", 10));
		BOOST_CHECK(VerifyExpectedValue("circleArea", multiplies<double>()(3.14159265f, 100)));
		
		BOOST_CHECK(calc.SetVar("circle10Area", "circleArea"));
		BOOST_CHECK(calc.SetVar("radius", 20));
		BOOST_CHECK(calc.SetVar("circle20Area", "circleArea"));
		BOOST_CHECK(VerifyExpectedIdsValues(calc.GetFnsValues(), {
			{ "circleArea", multiplies<double>()(3.14159265f, 400) },
			{ "radiusSquared", 400 }
		}));
		BOOST_CHECK(VerifyExpectedIdsValues(calc.GetVarsValues(), {
			{ "circle10Area", multiplies<double>()(3.14159265f, 100) },
			{ "circle20Area", multiplies<double>()(3.14159265f, 400) },
			{ "pi", 3.14159265f },
			{ "radius", 20 },
		}));
	}
	//
	BOOST_AUTO_TEST_CASE(MalovTest5)
	{
		BOOST_CHECK(calc.SetVar("v0", 0));
		BOOST_CHECK(calc.SetVar("v1", 1));
		BOOST_CHECK(calc.CreateFn("fib0", "v0"));
		BOOST_CHECK(calc.CreateFn("fib1", "v1"));
		BOOST_CHECK(calc.CreateFn("fib2", "fib1", "+", "fib0"));
		BOOST_CHECK(calc.CreateFn("fib3", "fib2", "+", "fib1"));
		BOOST_CHECK(calc.CreateFn("fib4", "fib3", "+", "fib2"));
		BOOST_CHECK(calc.CreateFn("fib5", "fib4", "+", "fib3"));
		BOOST_CHECK(calc.CreateFn("fib6", "fib5", "+", "fib4"));
		BOOST_CHECK(VerifyExpectedIdsValues(calc.GetFnsValues(), {
			{ "fib0", 0 }, { "fib1", 1 }, { "fib2", 1 }, { "fib3", 2 },
			{ "fib4", 3 }, { "fib5", 5 }, { "fib6", 8 },
		}));

		BOOST_CHECK(calc.SetVar("v0", 1));
		BOOST_CHECK(VerifyExpectedIdsValues(calc.GetFnsValues(), {
			{ "fib0", 1 },{ "fib1", 1 },{ "fib2", 2 },{ "fib3", 3 },
			{ "fib4", 5 },{ "fib5", 8 },{ "fib6", 13 },
		}));
	}
BOOST_AUTO_TEST_SUITE_END()
