#include "stdafx.h"
#include "../task34/Calculator.h"

using namespace std;

class Calculator_
{
public:
	Calculator_()
		: m_calc(m_in, m_out)
		, expr()
	{}
	
	bool VerifyExpectedOutput(initializer_list<string> lst)
	{
		m_in.clear();
		m_out.str("");

		m_in.str(expr);
		m_calc.Run();

		string expectedOut;
		for (auto const & str : lst)
		{
			expectedOut += str + '\n';
		}
		if (expectedOut.compare(m_out.str()) == 0)
		{
			return true;
		}

		cout << m_out.str() << "!=\n" << expectedOut;
		return false;
	}
	char const * expr;
private:
	istringstream m_in;
	ostringstream m_out;
	CCalculator m_calc;
};

BOOST_FIXTURE_TEST_SUITE(Calculator, Calculator_)
	//поддерживает идентификаторы
	BOOST_AUTO_TEST_CASE(supports_identifiers)
	{
		expr = R"(
		let x=1
		let _=2
		let xyz=3
		let XYZ=4
		let x_z=5
		let _x_=6
		let x1=7
		let x1y=8
		print x1y)";
		BOOST_CHECK(VerifyExpectedOutput({ "8.00" }));

		expr = R"(
		var 1)";
		BOOST_CHECK(VerifyExpectedOutput({ "invalid syntax" }));

		expr = R"(
		var 1xyz)";
		BOOST_CHECK(VerifyExpectedOutput({ "invalid syntax" }));

		expr = R"(
		var !@#)";
		BOOST_CHECK(VerifyExpectedOutput({ "invalid syntax" }));

		expr = R"(
		var xy!z)";
		BOOST_CHECK(VerifyExpectedOutput({ "invalid syntax" }));
	}
	//поддерживает команды
	BOOST_AUTO_TEST_SUITE(supports_commands)
		// оманда var
		BOOST_AUTO_TEST_SUITE(var)
			//объ€вл€ет переменную со значение NAN
			BOOST_AUTO_TEST_CASE(declares_variable_with_value_NAN)
			{
				expr = R"(
				var x
				print x)";
				BOOST_CHECK(VerifyExpectedOutput({ "nan" }));
			}
			//идентификатор должен быть уникальным
			BOOST_AUTO_TEST_CASE(identifier_must_be_unique)
			{
				expr = R"(
				var x
				var x
				print x)";
				BOOST_CHECK(VerifyExpectedOutput({ "x already exists", "nan" }));
		
				expr = R"(
				var y
				fn func=x+y
				print func)";
				BOOST_CHECK(VerifyExpectedOutput({ "nan" }));

				expr = R"(
				var func
				print func)";
				BOOST_CHECK(VerifyExpectedOutput({ "func already exists", "nan" }));

				expr = R"(
				let x=1
				let y=2
				print func)";
				BOOST_CHECK(VerifyExpectedOutput({ "3.00" }));
			}
		BOOST_AUTO_TEST_SUITE_END()
		// оманда let
		BOOST_AUTO_TEST_SUITE(let)
			//присваивает переменной числовое значение
			BOOST_AUTO_TEST_CASE(assigns_variable_numeric_value)
			{
				expr = R"(
				var x
				let x=0
				print x)";
				BOOST_CHECK(VerifyExpectedOutput({ "0.00" }));

				expr = R"(
				let x=1
				print x)";
				BOOST_CHECK(VerifyExpectedOutput({ "1.00" }));

				expr = R"(
				let x=12.34
				print x)";
				BOOST_CHECK(VerifyExpectedOutput({ "12.34" }));

				expr = R"(
				let x=1234567890.12
				print x)";
				BOOST_CHECK(VerifyExpectedOutput({ "1234567890.12" }));

				expr = R"(
				let x=-12.34
				print x)";
				BOOST_CHECK(VerifyExpectedOutput({ "-12.34" }));

				expr = R"(
				let x=-1234567890.12
				print x)";
				BOOST_CHECK(VerifyExpectedOutput({ "-1234567890.12" }));

				expr = R"(
				let x=0.01
				print x)";
				BOOST_CHECK(VerifyExpectedOutput({ "0.01" }));

				expr = R"(
				let x=.12
				print x)";
				BOOST_CHECK(VerifyExpectedOutput({ "0.12" }));

				expr = R"(
				let x=-.12
				print x)";
				BOOST_CHECK(VerifyExpectedOutput({ "-0.12" }));
			}
			//присваивает переменной текущее значение ранее объ€вленного идентификатора
			BOOST_AUTO_TEST_CASE(assigns_variable_current_value_of_previously_declared_identifier)
			{
				expr = R"(
				var x
				var y
				let y=x
				print y)";
				BOOST_CHECK(VerifyExpectedOutput({ "nan" }));

				expr = R"(
				let y=1
				print y)";
				BOOST_CHECK(VerifyExpectedOutput({ "1.00" }));

				expr = R"(
				let y=x
				print y)";
				BOOST_CHECK(VerifyExpectedOutput({ "nan" }));

				expr = R"(
				let x=7
				let y=x
				print y
				print x)";
				BOOST_CHECK(VerifyExpectedOutput({ "7.00", "7.00" }));

				expr = R"(
				let x=3
				print y
				print x)";
				BOOST_CHECK(VerifyExpectedOutput({ "7.00", "3.00" }));

				expr = R"(
				let z=15
				let y=z
				print y
				print x)";
				BOOST_CHECK(VerifyExpectedOutput({ "15.00", "3.00" }));
				//присвоение z значени€ функции
				expr = R"(
				fn func=x+y
				let z=func
				print z
				print func)";
				BOOST_CHECK(VerifyExpectedOutput({ "18.00", "18.00" }));
				//при изменении значени€ функции, z не мен€етс€
				expr = R"(
				let x=5
				let y=5
				print z
				print func)";
				BOOST_CHECK(VerifyExpectedOutput({ "18.00", "10.00" }));
				
				expr = R"(
				let x=wrong
				print x)";
				BOOST_CHECK(VerifyExpectedOutput({ "invalid identifier", "5.00" }));
			}
			//присвоение функции невозможно
			BOOST_AUTO_TEST_CASE(assigns_functions_is_impossible)
			{
				expr = R"(
				let x=5
				let y=5
				fn func=x+y
				print func)";
				BOOST_CHECK(VerifyExpectedOutput({ "10.00" }));

				expr = R"(
				let func=x
				print func)";
				BOOST_CHECK(VerifyExpectedOutput({ "invalid identifier", "10.00" }));

				expr = R"(
				let func=123.45
				print func)";
				BOOST_CHECK(VerifyExpectedOutput({ "invalid identifier", "10.00" }));

				expr = R"(
				let x=10
				let y=10
				print func)";
				BOOST_CHECK(VerifyExpectedOutput({ "20.00" }));
			}
			//если переменна€ не объ€влена, то она объ€вл€етс€
			BOOST_AUTO_TEST_CASE(if_variable_is_not_declared_it_is_declared)
			{
				expr = R"(
				let x=5
				let y=x
				print x
				print y)";
				BOOST_CHECK(VerifyExpectedOutput({ "5.00", "5.00" }));
			}
		BOOST_AUTO_TEST_SUITE_END()
		// оманда fn
		BOOST_AUTO_TEST_SUITE(fn)
			//объ€вл€ет функцию, значением которой будет значение ранее объ€вленного идентификатора в момент вычислени€
			BOOST_AUTO_TEST_CASE(declares_function_whose_value_is_value_of_previously_declared_identifier_in_moment_of_calculation)
			{	
				//f=x
				expr = R"(
				var x
				fn f=x
				print f)";
				BOOST_CHECK(VerifyExpectedOutput({ "nan" }));

				expr = R"(
				let x=5
				print f)";
				BOOST_CHECK(VerifyExpectedOutput({ "5.00" }));
				
				expr = R"(
				let x=-0.01
				print f)";
				BOOST_CHECK(VerifyExpectedOutput({ "-0.01" }));

				//f1=f
				expr = R"(
				fn f1=f
				print f1
				print f)";
				BOOST_CHECK(VerifyExpectedOutput({ "-0.01", "-0.01" }));

				expr = R"(
				let x=123.45
				print f1
				print f)";
				BOOST_CHECK(VerifyExpectedOutput({ "123.45", "123.45" }));

				expr = R"(
				fn f2=wrong)";
				BOOST_CHECK(VerifyExpectedOutput({ "invalid identifier" }));
			}
			//объ€вл€ет функцию, значением которой будет результат применени€ бинарного оператора к значени€м ранее объ€вленных идентификаторов в момент вычислени€
			BOOST_AUTO_TEST_SUITE(declares_function_whose_value_is_result_of_applying_binary_operator_to_values_of_previously_declared_identifiers_at_time_of_calculation)
				//оператор плюс
				BOOST_AUTO_TEST_CASE(operator_plus)
				{
					//f=x+y
					expr = R"(
					var x
					var y
					fn f=x+y
					print x
					print y
					print f)";
					BOOST_CHECK(VerifyExpectedOutput({ "nan", "nan", "nan" }));

					expr = R"(
					let x=5
					print x
					print y
					print f)";
					BOOST_CHECK(VerifyExpectedOutput({ "5.00", "nan", "nan" }));

					expr = R"(
					let y=12.5
					print x
					print y
					print f)";
					BOOST_CHECK(VerifyExpectedOutput({ "5.00", "12.50", "17.50" }));

					expr = R"(
					let x=10
					print x
					print y
					print f)";
					BOOST_CHECK(VerifyExpectedOutput({ "10.00", "12.50", "22.50" }));

					expr = R"(
					let y=-10
					print x
					print y
					print f)";
					BOOST_CHECK(VerifyExpectedOutput({ "10.00", "-10.00", "0.00" }));

					expr = R"(
					fn f1=wrong+wrong1)";
					BOOST_CHECK(VerifyExpectedOutput({ "invalid identifier" }));
				}
				//оператор минус
				BOOST_AUTO_TEST_CASE(operator_minus)
				{
					//f2=f1-z
					expr = R"(
					var x
					var y
					var z
					fn f1=x+y
					fn f2=f1-z
					print f1
					print z
					print f2)";
					BOOST_CHECK(VerifyExpectedOutput({ "nan", "nan", "nan" }));

					expr = R"(
					let x=5
					let y=15.5
					print f1
					print z
					print f2)";
					BOOST_CHECK(VerifyExpectedOutput({ "20.50", "nan", "nan"}));

					expr = R"(
					let z=7
					print f1
					print z
					print f2)";
					BOOST_CHECK(VerifyExpectedOutput({ "20.50", "7.00", "13.50" }));

					expr = R"(
					let z=-3.5
					print f1
					print z
					print f2)";
					BOOST_CHECK(VerifyExpectedOutput({ "20.50", "-3.50", "24.00" }));

					expr = R"(
					let y=-8.5
					print f1
					print z
					print f2)";
					BOOST_CHECK(VerifyExpectedOutput({ "-3.50", "-3.50", "0.00" }));
				}
				//оператор умножить
				BOOST_AUTO_TEST_CASE(operator_multiplies)
				{
					//f2=z*f1
					expr = R"(
					var x
					var y
					var z
					fn f1=x+y
					fn f2=z*f1
					print z
					print f1
					print f2)";
					BOOST_CHECK(VerifyExpectedOutput({ "nan", "nan", "nan" }));

					expr = R"(
					let x=5
					let y=7
					print z
					print f1
					print f2)";
					BOOST_CHECK(VerifyExpectedOutput({ "nan", "12.00", "nan"}));

					expr = R"(
					let z=10
					print z
					print f1
					print f2)";
					BOOST_CHECK(VerifyExpectedOutput({ "10.00", "12.00", "120.00" }));

					expr = R"(
					let z=-0.1
					print z
					print f1
					print f2)";
					BOOST_CHECK(VerifyExpectedOutput({ "-0.10", "12.00", "-1.20" }));

					expr = R"(
					let y=-5
					print z
					print f1
					print f2)";
					BOOST_CHECK(VerifyExpectedOutput({ "-0.10", "0.00", "-0.00" }));
				}
				//оператор делени€
				BOOST_AUTO_TEST_CASE(operator_divides)
				{
					//f3=f1/f2
					expr = R"(
					var x1
					var y1
					var x2
					var y2
					fn f1=x1+y1
					fn f2=x2+y2
					fn f3=f1/f2
					print f1
					print f2
					print f3)";
					BOOST_CHECK(VerifyExpectedOutput({ "nan", "nan", "nan" }));

					expr = R"(
					let x1=30
					let y1=20
					print f1
					print f2
					print f3)";
					BOOST_CHECK(VerifyExpectedOutput({ "50.00", "nan", "nan" }));

					expr = R"(
					let x2=5
					let y2=3
					print f1
					print f2
					print f3)";
					BOOST_CHECK(VerifyExpectedOutput({ "50.00", "8.00", "6.25" }));

					expr = R"(
					let x1=10
					let y1=70
					print f1
					print f2
					print f3)";
					BOOST_CHECK(VerifyExpectedOutput({ "80.00", "8.00", "10.00" }));

					expr = R"(
					let x1=1
					let y1=-1
					print f1
					print f2
					print f3)";
					BOOST_CHECK(VerifyExpectedOutput({ "0.00", "8.00", "0.00" }));
				}
			BOOST_AUTO_TEST_SUITE_END()
			//идентификатор функции должен быть уникальным
			BOOST_AUTO_TEST_CASE(function_identifier_must_be_unique)
			{
				expr = R"(
				let x=5
				let y=7
				fn f=x+y
				print f)";
				BOOST_CHECK(VerifyExpectedOutput({ "12.00"}));
				
				expr = R"(
				fn f=y)";
				BOOST_CHECK(VerifyExpectedOutput({ "invalid identifier" }));

				expr = R"(
				fn f1=y
				print f1)";
				BOOST_CHECK(VerifyExpectedOutput({ "7.00" }));
			}
		BOOST_AUTO_TEST_SUITE_END()
		// оманда print
		BOOST_AUTO_TEST_SUITE(print)
			//выводит значение ранее объ€вленного идентификатора
			BOOST_AUTO_TEST_CASE(displays_value_of_previously_declared_identifier)
			{
				expr = R"(
				var x
				print x)";
				BOOST_CHECK(VerifyExpectedOutput({ "nan" }));

				expr = R"(
				let x=5
				print x)";
				BOOST_CHECK(VerifyExpectedOutput({ "5.00" }));

				expr = R"(
				let x=123.45
				print x)";
				BOOST_CHECK(VerifyExpectedOutput({ "123.45" }));

				expr = R"(
				var y
				fn f=x+y
				print f)";
				BOOST_CHECK(VerifyExpectedOutput({ "nan" }));

				expr = R"(
				let y=10
				print f)";
				BOOST_CHECK(VerifyExpectedOutput({ "133.45" }));

				expr = R"(
				let x=-15
				let y=15
				print f)";
				BOOST_CHECK(VerifyExpectedOutput({ "0.00" }));

				expr = R"(
				print wrong)";
				BOOST_CHECK(VerifyExpectedOutput({ "wrong is not declared" }));
			}
		BOOST_AUTO_TEST_SUITE_END()
		// оманда printvars
		BOOST_AUTO_TEST_SUITE(printvars)
			//выводит имена и значени€ всех ранее объ€вленных переменных
			BOOST_AUTO_TEST_CASE(displays_names_and_values_of_all_previously_declared_variables)
			{
				expr = R"(
				printvars
				var z
				printvars)";
				BOOST_CHECK(VerifyExpectedOutput({ "z:nan" }));

				expr = R"(
				var a
				printvars)";
				BOOST_CHECK(VerifyExpectedOutput({ "a:nan", "z:nan" }));

				expr = R"(
				let a=5
				printvars)";
				BOOST_CHECK(VerifyExpectedOutput({ "a:5.00", "z:nan" }));

				expr = R"(
				let z=-0.99
				printvars)";
				BOOST_CHECK(VerifyExpectedOutput({ "a:5.00", "z:-0.99" }));

				expr = R"(
				fn f=a+z
				printvars)";
				BOOST_CHECK(VerifyExpectedOutput({ "a:5.00", "z:-0.99" }));

				expr = R"(
				let b=12.34
				printvars)";
				BOOST_CHECK(VerifyExpectedOutput({ "a:5.00", "b:12.34", "z:-0.99" }));
			}
		BOOST_AUTO_TEST_SUITE_END()
		// оманда printfns
		BOOST_AUTO_TEST_SUITE(printfns)
			//выводит имена и значени€ всех ранее объ€вленных функций
			BOOST_AUTO_TEST_CASE(displays_names_and_values_of_all_previously_declared_functions)
			{
				expr = R"(
				printfns
				var x
				var y
				fn z=x+y
				printfns)";
				BOOST_CHECK(VerifyExpectedOutput({ "z:nan" }));
				
				expr = R"(
				let x=5
				let y=7
				printfns)";
				BOOST_CHECK(VerifyExpectedOutput({ "z:12.00" }));

				expr = R"(
				fn a=x*y
				printfns)";
				BOOST_CHECK(VerifyExpectedOutput({ "a:35.00", "z:12.00" }));

				expr = R"(
				fn b=a-z
				printfns)";
				BOOST_CHECK(VerifyExpectedOutput({ "a:35.00", "b:23.00", "z:12.00" }));
			}
		BOOST_AUTO_TEST_SUITE_END()
	BOOST_AUTO_TEST_SUITE_END()
	//
	BOOST_AUTO_TEST_SUITE(MalovTests)
		//
		BOOST_AUTO_TEST_CASE(test1)
		{
			expr = R"(
			var x
			print x)";
			BOOST_CHECK(VerifyExpectedOutput({ "nan" }));

			expr = R"(
			let x=42
			print x)";
			BOOST_CHECK(VerifyExpectedOutput({ "42.00" }));

			expr = R"(
			let x=1.234
			print x)";
			BOOST_CHECK(VerifyExpectedOutput({ "1.23" }));

			expr = R"(
			let y=x
			let x=99
			printvars)";
			BOOST_CHECK(VerifyExpectedOutput({ "x:99.00", "y:1.23" }));
		}
		//
		BOOST_AUTO_TEST_CASE(test2)
		{
			expr = R"(
			var x
			var y
			fn XPlusY=x+y
			print XPlusY)";
			BOOST_CHECK(VerifyExpectedOutput({ "nan" }));

			expr = R"(
			let x=3
			let y=4
			print XPlusY)";
			BOOST_CHECK(VerifyExpectedOutput({ "7.00" }));

			expr = R"(
			let x=10
			print XPlusY)";
			BOOST_CHECK(VerifyExpectedOutput({ "14.00" }));

			expr = R"(
			let z=3.5
			fn XPlusYDivZ=XPlusY/z
			printfns)";
			BOOST_CHECK(VerifyExpectedOutput({ "XPlusY:14.00", "XPlusYDivZ:4.00" }));
		}
		//
		BOOST_AUTO_TEST_CASE(test3)
		{
			expr = R"(
			let v=42
			let variable=v
			fn function=v
			let v=43
			print variable)";
			BOOST_CHECK(VerifyExpectedOutput({ "42.00" }));

			expr = R"(
			print function)";
			BOOST_CHECK(VerifyExpectedOutput({ "43.00" }));
		}
		//
		BOOST_AUTO_TEST_CASE(test4)
		{
			expr = R"(
			var radius
			let pi=3.14159265
			fn radiusSquared=radius*radius
			fn circleArea=pi*radiusSquared
			let radius=10
			print circleArea)";
			BOOST_CHECK(VerifyExpectedOutput({ "314.16" }));

			expr = R"(
			let circle10Area=circleArea
			let radius=20
			let circle20Area=circleArea
			printfns)";
			BOOST_CHECK(VerifyExpectedOutput({ "circleArea:1256.64", "radiusSquared:400.00" }));

			expr = R"(
			printvars)";
			BOOST_CHECK(VerifyExpectedOutput({ "circle10Area:314.16", "circle20Area:1256.64", "pi:3.14", "radius:20.00"}));
		}
		//
		BOOST_AUTO_TEST_CASE(test5)
		{
			expr = R"(
			let v0=0
			let v1=1		
			fn fib0=v0
			fn fib1=v1
			fn fib2=fib1+fib0
			fn fib3=fib2+fib1
			fn fib4=fib3+fib2
			fn fib5=fib4+fib3
			fn fib6=fib5+fib4
			printfns)";
			BOOST_CHECK(VerifyExpectedOutput({ "fib0:0.00", "fib1:1.00", "fib2:1.00", "fib3:2.00", "fib4:3.00", "fib5:5.00", "fib6:8.00" }));

			expr = R"(
			let v0=1
			printfns)";
			BOOST_CHECK(VerifyExpectedOutput({ "fib0:1.00", "fib1:1.00", "fib2:2.00", "fib3:3.00", "fib4:5.00", "fib5:8.00", "fib6:13.00" }));
		}
	BOOST_AUTO_TEST_SUITE_END()
	//
	BOOST_AUTO_TEST_CASE(simple_test)
	{
		expr = R"(
		let x=1
		let y=2
		fn b=x+y
		fn c=x+y
		fn a=b+c
		fn z=y
		fn d=b+z
		fn e=b+c
		printfns
		printvars)";
		BOOST_CHECK(VerifyExpectedOutput({ "a:6.00", "b:3.00", "c:3.00", "d:5.00", "e:6.00", "z:2.00", "x:1.00", "y:2.00" }));
	}
BOOST_AUTO_TEST_SUITE_END()
