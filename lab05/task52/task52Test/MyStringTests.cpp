#include "stdafx.h"
#include "../task52/MyString.h"

using namespace std;

bool VerifyProperties(CMyString const & myStr, char const * expectedStr, size_t expectedSize)
{
	if (expectedSize == myStr.Size())
	{
		return (memcmp(myStr.Data(), expectedStr, expectedSize + 1) == 0);
	}

	return false;
}

//MyString
BOOST_AUTO_TEST_SUITE(MyString)
	//можно создать
	BOOST_AUTO_TEST_SUITE(can_create)
		//конструктором по умолчанию
		BOOST_AUTO_TEST_CASE(default_constructor)
		{
			CMyString myStr;
			BOOST_CHECK(VerifyProperties(myStr, "", 0));
		}
		//данными строки с завершающим нулевым символом
		BOOST_AUTO_TEST_CASE(string_with_terminating_null_character)
		{
			{
				char const * str = "";
				CMyString myStr(str);
				BOOST_CHECK(VerifyProperties(myStr, str, strlen(str)));//-V814
			}
			{
				char const * str = "abcde";
				CMyString myStr(str);
				BOOST_CHECK(VerifyProperties(myStr, str, strlen(str)));//-V814
			}
		}
		//данными символьного массива заданной длины
		BOOST_AUTO_TEST_CASE(character_array_of_given_length)
		{
			char const str[] = { 'a', 'b', 'c', 'd', 'e' };
			{
				CMyString myStr(str, 0);
				BOOST_CHECK(VerifyProperties(myStr, "", 0));
			}
			{
				CMyString myStr(str, 2);
				BOOST_CHECK(VerifyProperties(myStr, "ab", 2));
			}
			{
				CMyString myStr(str, 5);
				BOOST_CHECK(VerifyProperties(myStr, "abcde", 5));
			}
			{
				char const str[] = { 'a', '\0', 'c', '\0', 'e' };
				CMyString myStr(str, 5);
				BOOST_CHECK(VerifyProperties(myStr, "a\0c\0e", 5));
			}
			{
				char const str[] = "abcde";
				CMyString myStr(str, 3);
				BOOST_CHECK(VerifyProperties(myStr, "abc", 3));
			}
		}
		//данными строки STL
		BOOST_AUTO_TEST_CASE(STL_string)
		{
			{
				string const str;
				CMyString myStr(str);
				BOOST_CHECK(VerifyProperties(myStr, str.data(), str.size()));
			}
			{
				string const str("abcde");
				CMyString myStr(str);
				BOOST_CHECK(VerifyProperties(myStr, str.data(), str.size()));
			}
			{
				string const str("ab\0de", 5);
				CMyString myStr(str);
				BOOST_CHECK(VerifyProperties(myStr, str.data(), str.size()));
			}
		}
		//конструктором копирования
		BOOST_AUTO_TEST_CASE(copy_constructor)
		{
			{
				CMyString const myStrOrig;
				CMyString myStr(myStrOrig);
				BOOST_CHECK(VerifyProperties(myStr, myStrOrig.Data(), myStrOrig.Size()));
			}
			{
				CMyString const myStrOrig("abcde");
				CMyString myStr(myStrOrig);
				BOOST_CHECK(VerifyProperties(myStr, myStrOrig.Data(), myStrOrig.Size()));
			}
		}
		//перемещающим конструктором
		BOOST_AUTO_TEST_CASE(move_constructor)
		{
			CMyString moved("abcde");
			CMyString myStr(move(moved));
			BOOST_CHECK(VerifyProperties(myStr, "abcde", 5));
			BOOST_CHECK(VerifyProperties(moved, "", 0));
			{
				CMyString myStr(move(moved));
				BOOST_CHECK(VerifyProperties(myStr, "", 0));
				BOOST_CHECK(VerifyProperties(moved, "", 0));
			}
			{
				CMyString myStr(moved);
				BOOST_CHECK(VerifyProperties(myStr, "", 0));
				BOOST_CHECK(VerifyProperties(moved, "", 0));
			}
			{
				CMyString myStr;
				myStr = moved;
				BOOST_CHECK(VerifyProperties(myStr, "", 0));
				BOOST_CHECK(VerifyProperties(moved, "", 0));
			}
			{
				CMyString myStr("abc");
				myStr = moved;
				BOOST_CHECK(VerifyProperties(myStr, "", 0));
				BOOST_CHECK(VerifyProperties(moved, "", 0));
			}
			{
				CMyString myStr;
				myStr += moved;
				BOOST_CHECK(VerifyProperties(myStr, "", 0));
				BOOST_CHECK(VerifyProperties(moved, "", 0));
			}
			{
				CMyString myStr("abc");
				myStr += moved;
				BOOST_CHECK(VerifyProperties(myStr, "abc", 3));
				BOOST_CHECK(VerifyProperties(moved, "", 0));
			}
			{
				CMyString myStr;
				myStr = moved + "abc";
				BOOST_CHECK(VerifyProperties(myStr, "abc", 3));
				BOOST_CHECK(VerifyProperties(moved, "", 0));
			}
			//изменяем moved
			{
				CMyString myStr("abc");
				moved = myStr;
				BOOST_CHECK(VerifyProperties(myStr, "abc", 3));
				BOOST_CHECK(VerifyProperties(moved, "abc", 3));
				CMyString tmp(move(moved));

				moved += myStr;
				BOOST_CHECK(VerifyProperties(myStr, "abc", 3));
				BOOST_CHECK(VerifyProperties(moved, "abc", 3));
				CMyString tmp1(move(moved));

				moved = myStr + myStr;
				BOOST_CHECK(VerifyProperties(myStr, "abc", 3));
				BOOST_CHECK(VerifyProperties(moved, "abcabc", 6));
			}
		}
	BOOST_AUTO_TEST_SUITE_END()
	//имеет длину
	BOOST_AUTO_TEST_CASE(has_length)
	{
		CMyString const myStr;
		BOOST_CHECK_EQUAL(myStr.Size(), 0);

		CMyString const myStr1("abcde");
		BOOST_CHECK_EQUAL(myStr1.Size(), 5);
	}
	//может возвратить указатель на массив символов строки
	BOOST_AUTO_TEST_CASE(can_return_pointer_to_array_of_character)
	{
		CMyString const myStr;
		BOOST_CHECK(memcmp(myStr.Data(), "", 1) == 0);

		CMyString const myStr1("abcde");
		BOOST_CHECK(memcmp(myStr1.Data(), "abcde", 6) == 0);
	}
	//может возвратить подстроку
	BOOST_AUTO_TEST_CASE(can_return_substring)
	{
		{
			CMyString const myStr;
			{
				CMyString mySubStr(myStr.SubStr(0));
				BOOST_CHECK(VerifyProperties(mySubStr, "", 0));
			}
			{
				CMyString mySubStr(myStr.SubStr(5));
				BOOST_CHECK(VerifyProperties(mySubStr, "", 0));
			
				mySubStr = myStr.SubStr(5, 10);
				BOOST_CHECK(VerifyProperties(mySubStr, "", 0));
			}
		}

		CMyString const myStr("abcde");
		{
			CMyString mySubStr(myStr.SubStr(0));
			BOOST_CHECK(VerifyProperties(mySubStr, "abcde", 5));
		}
		{
			CMyString mySubStr(myStr.SubStr(0, 10));
			BOOST_CHECK(VerifyProperties(mySubStr, "abcde", 5));
		}
		{
			CMyString mySubStr(myStr.SubStr(2, 0));
			BOOST_CHECK(VerifyProperties(mySubStr, "", 0));
		}
		{
			CMyString mySubStr(myStr.SubStr(2, 1));
			BOOST_CHECK(VerifyProperties(mySubStr, "c", 1));
		}
		{
			CMyString mySubStr(myStr.SubStr(2, 3));
			BOOST_CHECK(VerifyProperties(mySubStr, "cde", 3));
		
			mySubStr = myStr.SubStr(2, 10);
			BOOST_CHECK(VerifyProperties(mySubStr, "cde", 3));
		
			mySubStr = myStr.SubStr(4);
			BOOST_CHECK(VerifyProperties(mySubStr, "e", 1));
		
			mySubStr = myStr.SubStr(5);
			BOOST_CHECK(VerifyProperties(mySubStr, "", 0));
		}
	}
	//может очистить строку
	BOOST_AUTO_TEST_CASE(can_clear_string)
	{
		{
			CMyString myStr;
			myStr.Clear();
			BOOST_CHECK(VerifyProperties(myStr, "", 0));

			myStr += "abc";
			BOOST_CHECK(VerifyProperties(myStr, "abc", 3));
		}
		{
			CMyString myStr("abcde");
			myStr.Clear();
			BOOST_CHECK(VerifyProperties(myStr, "", 0));

			CMyString myStr1("abc");
			myStr1 = myStr;
			BOOST_CHECK(VerifyProperties(myStr1, "", 0));

			myStr = "abc";
			BOOST_CHECK(VerifyProperties(myStr, "abc", 3));
		}
	}

	struct Operators_
	{
		CMyString const empty;
		CMyString const abc = "abc";
		CMyString const defgh = "defgh";
	};
	//имеет операторы
	BOOST_FIXTURE_TEST_SUITE(has_operators, Operators_)
		//присваивания
		BOOST_AUTO_TEST_CASE(assignment)
		{
			CMyString myStr;

			myStr = empty;
			BOOST_CHECK(VerifyProperties(myStr, empty.Data(), empty.Size()));
			myStr = abc;
			BOOST_CHECK(VerifyProperties(myStr, abc.Data(), abc.Size()));
			myStr = defgh;
			BOOST_CHECK(VerifyProperties(myStr, defgh.Data(), defgh.Size()));
			myStr = abc;
			BOOST_CHECK(VerifyProperties(myStr, abc.Data(), abc.Size()));
			myStr = empty;
			BOOST_CHECK(VerifyProperties(myStr, empty.Data(), empty.Size()));
		
			myStr = defgh;
			BOOST_CHECK(VerifyProperties(myStr, defgh.Data(), defgh.Size()));
			myStr = myStr;//-V570
			BOOST_CHECK(VerifyProperties(myStr, defgh.Data(), defgh.Size()));
			myStr = empty;
			BOOST_CHECK(VerifyProperties(myStr, empty.Data(), empty.Size()));
			myStr = myStr;//-V570
			BOOST_CHECK(VerifyProperties(myStr, empty.Data(), empty.Size()));
		}
		//перемещающего присваивания
		BOOST_AUTO_TEST_CASE(move_assignment)
		{
			CMyString myStr("123");
			{
				CMyString moved;
				myStr = move(moved);
				BOOST_CHECK(VerifyProperties(moved, "", 0));
				BOOST_CHECK(VerifyProperties(myStr, "", 0));

				moved += "abc";
				BOOST_CHECK(VerifyProperties(moved, "abc", 3));
			}
			{
				CMyString moved("abc");
				myStr = move(moved);
				BOOST_CHECK(VerifyProperties(moved, "", 0));
				BOOST_CHECK(VerifyProperties(myStr, "abc", 3));

				moved += myStr;
				BOOST_CHECK(VerifyProperties(moved, "abc", 3));
				BOOST_CHECK(VerifyProperties(myStr, "abc", 3));
			}
		}
		//присваивания сложения
		BOOST_AUTO_TEST_CASE(concatenation_assignment)
		{
			{
				CMyString myStr;
				myStr += empty;
				BOOST_CHECK(VerifyProperties(myStr, empty.Data(), empty.Size()));
			}
			{
				CMyString myStr;
				myStr += abc;
				BOOST_CHECK(VerifyProperties(myStr, abc.Data(), abc.Size()));
				myStr += defgh;
				BOOST_CHECK(VerifyProperties(myStr, "abcdefgh", 8));
			}
			{
				CMyString myStr("xyz");
				myStr += empty;
				BOOST_CHECK(VerifyProperties(myStr, "xyz", 3));
				myStr += abc;
				BOOST_CHECK(VerifyProperties(myStr, "xyzabc", 6));
			}
			{
				string const str("ab\0de", 5);
				CMyString myStr(str);
				myStr += str;
				BOOST_CHECK(VerifyProperties(myStr, "ab\0deab\0de", 10));
			}
		}
		//сложения
		BOOST_AUTO_TEST_CASE(concatenation)
		{
			CMyString myStr;
			
 			myStr = empty + empty;
 			BOOST_CHECK(VerifyProperties(myStr, empty.Data(), empty.Size()));
			myStr = abc + empty;
			BOOST_CHECK(VerifyProperties(myStr, abc.Data(), abc.Size()));
			myStr = empty + defgh;
			BOOST_CHECK(VerifyProperties(myStr, defgh.Data(), defgh.Size()));
			myStr = abc + defgh;
			BOOST_CHECK(VerifyProperties(myStr, "abcdefgh", 8));
			
			string const emptyStl;
			string const abcStl("abc");
			string nullInclude("ab\0de", 5);
			myStr = abc + emptyStl;
			BOOST_CHECK(VerifyProperties(myStr, abc.Data(), abc.Size()));
			myStr = empty + abcStl;
			BOOST_CHECK(VerifyProperties(myStr, abcStl.data(), abcStl.size()));
			myStr = emptyStl + empty;
			BOOST_CHECK(VerifyProperties(myStr, empty.Data(), empty.Size()));
			myStr = abcStl + abc;
			BOOST_CHECK(VerifyProperties(myStr, "abcabc", 6));
			myStr = nullInclude + abc;
			BOOST_CHECK(VerifyProperties(myStr, "ab\0deabc", 8));
			
			char const * emptyC = "";
			char const * abcC = "abc";
			myStr = abc + emptyC;
			BOOST_CHECK(VerifyProperties(myStr, abc.Data(), abc.Size()));
			myStr = empty + abcC;
			BOOST_CHECK(VerifyProperties(myStr, abcC, strlen(abcC)));//-V814
			myStr = emptyC + empty;
			BOOST_CHECK(VerifyProperties(myStr, empty.Data(), empty.Size()));
			myStr = abcC + abc;
			BOOST_CHECK(VerifyProperties(myStr, "abcabc", 6));
		}
		//сравнения
		BOOST_AUTO_TEST_SUITE(comparison)
			//равенство
			BOOST_AUTO_TEST_CASE(equality)
			{
				BOOST_CHECK(empty == "");
				BOOST_CHECK(!(empty == abc));

				BOOST_CHECK(abc == "abc");
				BOOST_CHECK(!(abc == defgh));
			}
			//неравенство
			BOOST_AUTO_TEST_CASE(inequality)
			{
				BOOST_CHECK(!(empty != ""));
				BOOST_CHECK(empty != abc);

				BOOST_CHECK(!(abc != "abc"));
				BOOST_CHECK(abc != defgh);
			}
			//меньше
			BOOST_AUTO_TEST_CASE(less_than)
			{
				BOOST_CHECK(!(empty < ""));
				BOOST_CHECK(empty < abc);

				BOOST_CHECK(!(abc < "abc"));
				BOOST_CHECK(abc < defgh);

				BOOST_CHECK(!(abc < "abba"));
				BOOST_CHECK(abc < "de");

				BOOST_CHECK(!(abc < "ab"));
				BOOST_CHECK(abc < "abca");
			}
			//больше
			BOOST_AUTO_TEST_CASE(greater_than)
			{
				BOOST_CHECK(!(empty > ""));
				BOOST_CHECK(abc > empty);

				BOOST_CHECK(!(abc > "abc"));
				BOOST_CHECK(defgh > abc);

				BOOST_CHECK(!(abc > "de"));
				BOOST_CHECK(abc > "abba");

				BOOST_CHECK(!(abc > "abca"));
				BOOST_CHECK(abc > "ab");
			}
			//меньше или равно
			BOOST_AUTO_TEST_CASE(less_than_or_equal)
			{
				BOOST_CHECK(empty <= "");
				BOOST_CHECK(empty <= abc);

				BOOST_CHECK(abc <= "abc");
				BOOST_CHECK(abc <= defgh);

				BOOST_CHECK(!(abc <= "abba"));
				BOOST_CHECK(abc <= "de");

				BOOST_CHECK(!(abc <= "ab"));
				BOOST_CHECK(abc <= "abca");
			}
			//больше или равно
			BOOST_AUTO_TEST_CASE(greater_than_or_equal)
			{
				BOOST_CHECK(empty >= "");
				BOOST_CHECK(abc >= empty);

				BOOST_CHECK(abc >= "abc");
				BOOST_CHECK(defgh >= abc);

				BOOST_CHECK(!(abc >= "de"));
				BOOST_CHECK(abc >= "abba");

				BOOST_CHECK(!(abc >= "abca"));
				BOOST_CHECK(abc >= "ab");
			}
		BOOST_AUTO_TEST_SUITE_END()
		//индексации[]
		BOOST_AUTO_TEST_CASE(indexing)
		{
			BOOST_CHECK_EQUAL(abc[0], 'a');
			BOOST_CHECK_EQUAL(abc[2], 'c');

			CMyString myStr("abc");
			BOOST_CHECK_EQUAL(myStr[0], 'a');
			BOOST_CHECK_EQUAL(myStr[2], 'c');

			myStr[2] = 'z';
			BOOST_CHECK_EQUAL(myStr[2], 'z');
			BOOST_CHECK_EQUAL(myStr, "abz");
		}
		//может быть выведена в выходной поток
		BOOST_AUTO_TEST_CASE(can_be_output_to_output_stream)
		{
			{
				stringstream strm;
				strm << empty;
				BOOST_CHECK_EQUAL(strm.str(), empty);
			}
			{
				stringstream strm;
				strm << abc;
				BOOST_CHECK_EQUAL(strm.str(), abc);
			}
			{
				stringstream strm;
				string str("ab\0de", 5);
				strm << CMyString(str);
				BOOST_CHECK_EQUAL(strm.str(), str);
			}
		}
		//может быть введена из входного потока
		BOOST_AUTO_TEST_CASE(can_be_entered_from_input_stream)
		{
			CMyString myStr;
			{
				stringstream strm("");
				strm >> myStr;
				BOOST_CHECK_EQUAL(myStr, "");
			}
			{
				stringstream strm("abcde");
				strm >> myStr;
				BOOST_CHECK_EQUAL(myStr, "abcde");
			}
			{
				stringstream strm("  \nab  cd  ");
				strm >> myStr;
				BOOST_CHECK_EQUAL(myStr, "ab");
				strm >> myStr;
				BOOST_CHECK_EQUAL(myStr, "cd");
			}
		}
	BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

struct Iterator_
{
	CMyString empty;
	CMyString myStr = "12345";
	CMyString::iterator begin = myStr.begin();
	CMyString::iterator end = myStr.end();
};
//Итератор
BOOST_FIXTURE_TEST_SUITE(Iterator, Iterator_)
	//имеет операторы
	BOOST_AUTO_TEST_SUITE(has_operators)
		//разыменования
		BOOST_AUTO_TEST_CASE(dereference)
		{
			BOOST_CHECK_EQUAL(*begin, '1');
		}
		//префексный инкремент
		BOOST_AUTO_TEST_CASE(preincrement)
		{
			++begin;
			BOOST_CHECK_EQUAL(*begin, '2');
			BOOST_CHECK_EQUAL(*++begin, '3');

			*begin = '_';
			BOOST_CHECK(myStr == "12_45");
		}
		//постфиксный инкремент
		BOOST_AUTO_TEST_CASE(postincrement)
		{
			begin++;//-V803
			BOOST_CHECK_EQUAL(*begin, '2');
			BOOST_CHECK_EQUAL(*begin++, '2');
			BOOST_CHECK_EQUAL(*begin, '3');

			*begin = '_';
			BOOST_CHECK(myStr == "12_45");
		}
		//префексный декремент
		BOOST_AUTO_TEST_CASE(predecrement)
		{
			--end;
			BOOST_CHECK_EQUAL(*end, '5');
			BOOST_CHECK_EQUAL(*--end, '4');

			*end = '_';
			BOOST_CHECK(myStr == "123_5");
		}
		//постфиксный декремент
		BOOST_AUTO_TEST_CASE(postdecrement)
		{
			end--;//-V803
			BOOST_CHECK_EQUAL(*end, '5');
			BOOST_CHECK_EQUAL(*end--, '5');
			BOOST_CHECK_EQUAL(*end, '4');

			*end = '_';
			BOOST_CHECK(myStr == "123_5");
		}
		//присваивание сложения
		BOOST_AUTO_TEST_CASE(addition_assignment)
		{
			begin += 1;
			BOOST_CHECK_EQUAL(*begin, '2');
			begin += 3;
			BOOST_CHECK_EQUAL(*begin, '5');

			*begin = '_';
			BOOST_CHECK(myStr == "1234_");
		}
		//сложения
		BOOST_AUTO_TEST_CASE(addition)
		{
			BOOST_CHECK_EQUAL(*(begin + 1), '2');
			BOOST_CHECK_EQUAL(*(begin + 4), '5');
		
			BOOST_CHECK_EQUAL(*(0 + begin), '1');
			BOOST_CHECK_EQUAL(*(3 + begin), '4');

			*begin = '_';
			BOOST_CHECK(myStr == "_2345");
		}
		//присваивание вычитания
		BOOST_AUTO_TEST_CASE(subtraction_assignment)
		{
			end -= 2;
			BOOST_CHECK_EQUAL(*end, '4');
			end -= 1;
			BOOST_CHECK_EQUAL(*end, '3');

			*end = '_';
			BOOST_CHECK(myStr == "12_45");
		}
		//вычитания
		BOOST_AUTO_TEST_CASE(subtraction)
		{
			BOOST_CHECK_EQUAL(*(end - 1), '5');
			BOOST_CHECK_EQUAL(*(end - 5), '1');

			*--end = '_';
			BOOST_CHECK(myStr == "1234_");
		}
		//разность итераторов
		BOOST_AUTO_TEST_CASE(difference_of_iterators)
		{
			BOOST_CHECK_EQUAL(begin - begin, 0);//-V501
			BOOST_CHECK_EQUAL(end - begin, 5);
			BOOST_CHECK_EQUAL(--end - ++begin, 3);

			BOOST_CHECK_EQUAL(empty.begin() - empty.end(), 0);
		}
		//индексации[]
		BOOST_AUTO_TEST_CASE(indexing)
		{
			BOOST_CHECK_EQUAL(begin[0], '1');
			BOOST_CHECK_EQUAL(begin[3], '4');
			BOOST_CHECK_EQUAL(begin[2], '3');

			begin[4] = '_';
			*begin = '_';
			BOOST_CHECK(myStr == "_234_");
		}
		//сравнения
		BOOST_AUTO_TEST_CASE(comparison)
		{
			BOOST_CHECK((begin + 2) == (end - 3));
			BOOST_CHECK(begin != end);
			BOOST_CHECK(begin < end);
			BOOST_CHECK(end > begin);
			BOOST_CHECK((end - 2) >= (begin + 2));
			BOOST_CHECK((end - 3) >= (begin + 2));
			BOOST_CHECK((begin + 1) <= (end - 3));
			BOOST_CHECK((begin + 2) <= (end - 3));

			auto it = begin;
			BOOST_CHECK(begin == it);
			BOOST_CHECK(++begin == (it + 1));
			BOOST_CHECK(begin != it);

			BOOST_CHECK(empty.begin() == empty.end());
		}
	BOOST_AUTO_TEST_SUITE_END()
	//совместим с алгоритмами STL
	BOOST_AUTO_TEST_CASE(compatible_with_STL_algorithms)
	{
		random_shuffle(myStr.begin(), myStr.end());
		BOOST_CHECK(myStr != "12345");

		auto res = minmax_element(myStr.begin(), myStr.end());
		BOOST_CHECK_EQUAL(*res.first, '1');
		BOOST_CHECK_EQUAL(*res.second, '5');
		
		sort(myStr.begin(), myStr.end());
		stringstream strm;
		copy(myStr.begin(), myStr.end(), ostream_iterator<char>(strm));
		BOOST_CHECK_EQUAL(strm.str(), "12345");
		
		reverse(myStr.begin(), myStr.end());
		strm.str("");
		for (auto ch : myStr)
		{
			strm << ch;
		}
		BOOST_CHECK_EQUAL(strm.str(), "54321");

		strm.str("");
		for (auto ch : empty)
		{
			strm << ch;
		}
		BOOST_CHECK_EQUAL(strm.str(), "");
	}
BOOST_AUTO_TEST_SUITE_END()

struct ConstIterator_
{
	CMyString const empty;
	CMyString const myStr = "12345";
	CMyString::const_iterator begin = myStr.begin();
	CMyString::const_iterator end = myStr.end();
};
//Константный итератор
BOOST_FIXTURE_TEST_SUITE(Const_iterator, ConstIterator_)
	//имеет операторы
	BOOST_AUTO_TEST_SUITE(has_operators)
		//разыменования
		BOOST_AUTO_TEST_CASE(dereference)
		{
			BOOST_CHECK_EQUAL(*begin, '1');
		}
		//префексный инкремент
		BOOST_AUTO_TEST_CASE(preincrement)
		{
			++begin;
			BOOST_CHECK_EQUAL(*begin, '2');
			BOOST_CHECK_EQUAL(*++begin, '3');
		}
		//постфиксный инкремент
		BOOST_AUTO_TEST_CASE(postincrement)
		{
			begin++;//-V803
			BOOST_CHECK_EQUAL(*begin, '2');
			BOOST_CHECK_EQUAL(*begin++, '2');
			BOOST_CHECK_EQUAL(*begin, '3');
		}
		//префексный декремент
		BOOST_AUTO_TEST_CASE(predecrement)
		{
			--end;
			BOOST_CHECK_EQUAL(*end, '5');
			BOOST_CHECK_EQUAL(*--end, '4');
		}
		//постфиксный декремент
		BOOST_AUTO_TEST_CASE(postdecrement)
		{
			end--;//-V803
			BOOST_CHECK_EQUAL(*end, '5');
			BOOST_CHECK_EQUAL(*end--, '5');
			BOOST_CHECK_EQUAL(*end, '4');
		}
		//присваивание сложения
		BOOST_AUTO_TEST_CASE(addition_assignment)
		{
			begin += 1;
			BOOST_CHECK_EQUAL(*begin, '2');
			begin += 3;
			BOOST_CHECK_EQUAL(*begin, '5');
		}
		//сложения
		BOOST_AUTO_TEST_CASE(addition)
		{
			BOOST_CHECK_EQUAL(*(begin + 1), '2');
			BOOST_CHECK_EQUAL(*(begin + 4), '5');

			BOOST_CHECK_EQUAL(*(0 + begin), '1');
			BOOST_CHECK_EQUAL(*(3 + begin), '4');
		}
		//присваивание вычитания
		BOOST_AUTO_TEST_CASE(subtraction_assignment)
		{
			end -= 2;
			BOOST_CHECK_EQUAL(*end, '4');
			end -= 1;
			BOOST_CHECK_EQUAL(*end, '3');
		}
		//вычитания
		BOOST_AUTO_TEST_CASE(subtraction)
		{
			BOOST_CHECK_EQUAL(*(end - 1), '5');
			BOOST_CHECK_EQUAL(*(end - 5), '1');
		}
		//разность итераторов
		BOOST_AUTO_TEST_CASE(difference_of_iterators)
		{
			BOOST_CHECK_EQUAL(begin - begin, 0);//-V501
			BOOST_CHECK_EQUAL(end - begin, 5);
			BOOST_CHECK_EQUAL(--end - ++begin, 3);

			BOOST_CHECK_EQUAL(empty.begin() - empty.end(), 0);
		}
		//индексации[]
		BOOST_AUTO_TEST_CASE(indexing)
		{
			BOOST_CHECK_EQUAL(begin[0], '1');
			BOOST_CHECK_EQUAL(begin[3], '4');
			BOOST_CHECK_EQUAL(begin[2], '3');
		}
		//сравнения
		BOOST_AUTO_TEST_CASE(comparison)
		{
			BOOST_CHECK((begin + 2) == (end - 3));
			BOOST_CHECK(begin != end);
			BOOST_CHECK(begin < end);
			BOOST_CHECK(end > begin);
			BOOST_CHECK((end - 2) >= (begin + 2));
			BOOST_CHECK((end - 3) >= (begin + 2));
			BOOST_CHECK((begin + 1) <= (end - 3));
			BOOST_CHECK((begin + 2) <= (end - 3));

			auto it = begin;
			BOOST_CHECK(begin == it);
			BOOST_CHECK(++begin == (it + 1));
			BOOST_CHECK(begin != it);

			BOOST_CHECK(empty.begin() == empty.end());
		}
	BOOST_AUTO_TEST_SUITE_END()
	//совместим с алгоритмами STL
	BOOST_AUTO_TEST_CASE(compatible_with_STL_algorithms)
	{
		auto res = minmax_element(myStr.begin(), myStr.end());
		BOOST_CHECK_EQUAL(*res.first, '1');
		BOOST_CHECK_EQUAL(*res.second, '5');

		stringstream strm;
		copy(myStr.begin(), myStr.end(), ostream_iterator<char>(strm));
		BOOST_CHECK_EQUAL(strm.str(), "12345");

		strm.str("");
		for (auto ch : myStr)
		{
			strm << ch;
		}
		BOOST_CHECK_EQUAL(strm.str(), "12345");

		strm.str("");
		for (auto ch : empty)
		{
			strm << ch;
		}
		BOOST_CHECK_EQUAL(strm.str(), "");
	}
BOOST_AUTO_TEST_SUITE_END()

struct ReverseIterator_
{
	CMyString empty;
	CMyString myStr = "12345";
	CMyString::reverse_iterator rbegin = myStr.rbegin();
	CMyString::reverse_iterator rend = myStr.rend();
};
//Реверсированный итератор
BOOST_FIXTURE_TEST_SUITE(Reverse_iterator, ReverseIterator_)
	//имеет операторы
	BOOST_AUTO_TEST_SUITE(has_operators)
		//разыменования
		BOOST_AUTO_TEST_CASE(dereference)
		{
			BOOST_CHECK_EQUAL(*rbegin, '5');
		}
		//префексный инкремент
		BOOST_AUTO_TEST_CASE(preincrement)
		{
			++rbegin;
			BOOST_CHECK_EQUAL(*rbegin, '4');
			BOOST_CHECK_EQUAL(*++rbegin, '3');

			*rbegin = '_';
			BOOST_CHECK(myStr == "12_45");
		}
		//постфиксный инкремент
		BOOST_AUTO_TEST_CASE(postincrement)
		{
			rbegin++;//-V803
			BOOST_CHECK_EQUAL(*rbegin, '4');
			BOOST_CHECK_EQUAL(*rbegin++, '4');
			BOOST_CHECK_EQUAL(*rbegin, '3');

			*rbegin = '_';
			BOOST_CHECK(myStr == "12_45");
		}
		//префексный декремент
		BOOST_AUTO_TEST_CASE(predecrement)
		{
			--rend;
			BOOST_CHECK_EQUAL(*rend, '1');
			BOOST_CHECK_EQUAL(*--rend, '2');

			*rend = '_';
			BOOST_CHECK(myStr == "1_345");
		}
		//постфиксный декремент
		BOOST_AUTO_TEST_CASE(postdecrement)
		{
			rend--;//-V803
			BOOST_CHECK_EQUAL(*rend, '1');
			BOOST_CHECK_EQUAL(*rend--, '1');
			BOOST_CHECK_EQUAL(*rend, '2');

			*rend = '_';
			BOOST_CHECK(myStr == "1_345");
		}
		//присваивание сложения
		BOOST_AUTO_TEST_CASE(addition_assignment)
		{
			rbegin += 1;
			BOOST_CHECK_EQUAL(*rbegin, '4');
			rbegin += 3;
			BOOST_CHECK_EQUAL(*rbegin, '1');

			*rbegin = '_';
			BOOST_CHECK(myStr == "_2345");
		}
		//сложения
		BOOST_AUTO_TEST_CASE(addition)
		{
			BOOST_CHECK_EQUAL(*(rbegin + 1), '4');
			BOOST_CHECK_EQUAL(*(rbegin + 4), '1');

			BOOST_CHECK_EQUAL(*(0 + rbegin), '5');
			BOOST_CHECK_EQUAL(*(3 + rbegin), '2');

			*rbegin = '_';
			BOOST_CHECK(myStr == "1234_");
		}
		//присваивание вычитания
		BOOST_AUTO_TEST_CASE(subtraction_assignment)
		{
			rend -= 2;
			BOOST_CHECK_EQUAL(*rend, '2');
			rend -= 1;
			BOOST_CHECK_EQUAL(*rend, '3');

			*rend = '_';
			BOOST_CHECK(myStr == "12_45");
		}
		//вычитания
		BOOST_AUTO_TEST_CASE(subtraction)
		{
			BOOST_CHECK_EQUAL(*(rend - 1), '1');
			BOOST_CHECK_EQUAL(*(rend - 5), '5');

			*--rend = '_';
			BOOST_CHECK(myStr == "_2345");
		}
		//разность итераторов
		BOOST_AUTO_TEST_CASE(difference_of_iterators)
		{
			BOOST_CHECK_EQUAL(rbegin - rbegin, 0);//-V501
			BOOST_CHECK_EQUAL(rend - rbegin, 5);
			BOOST_CHECK_EQUAL(--rend - ++rbegin, 3);

			BOOST_CHECK_EQUAL(empty.rbegin() - empty.rend(), 0);
		}
		//индексации[]
		BOOST_AUTO_TEST_CASE(indexing)
		{
			BOOST_CHECK_EQUAL(rbegin[0], '5');
			BOOST_CHECK_EQUAL(rbegin[3], '2');
			BOOST_CHECK_EQUAL(rbegin[2], '3');

			rbegin[4] = '_';
			*rbegin = '_';
			BOOST_CHECK(myStr == "_234_");
		}
		//сравнения
		BOOST_AUTO_TEST_CASE(comparison)
		{
			BOOST_CHECK((rbegin + 2) == (rend - 3));
			BOOST_CHECK(rbegin != rend);
			BOOST_CHECK(rbegin < rend);
			BOOST_CHECK(rend > rbegin);
			BOOST_CHECK((rend - 2) >= (rbegin + 2));
			BOOST_CHECK((rend - 3) >= (rbegin + 2));
			BOOST_CHECK((rbegin + 1) <= (rend - 3));
			BOOST_CHECK((rbegin + 2) <= (rend - 3));

			auto it = rbegin;
			BOOST_CHECK(rbegin == it);
			BOOST_CHECK(++rbegin == (it + 1));
			BOOST_CHECK(rbegin != it);

			BOOST_CHECK(empty.rbegin() == empty.rend());
		}
	BOOST_AUTO_TEST_SUITE_END()
	//совместим с алгоритмами STL
	BOOST_AUTO_TEST_CASE(compatible_with_STL_algorithms)
	{
		random_shuffle(myStr.rbegin(), myStr.rend());
		BOOST_CHECK(myStr != "12345");

		auto res = minmax_element(myStr.rbegin(), myStr.rend());
		BOOST_CHECK_EQUAL(*res.first, '1');
		BOOST_CHECK_EQUAL(*res.second, '5');

		sort(myStr.rbegin(), myStr.rend());
		stringstream strm;
		copy(myStr.rbegin(), myStr.rend(), ostream_iterator<char>(strm));
		BOOST_CHECK_EQUAL(strm.str(), "12345");

		reverse(myStr.rbegin(), myStr.rend());
		strm.str("");
		for (auto ch : myStr)
		{
			strm << ch;
		}
		BOOST_CHECK_EQUAL(strm.str(), "12345");
	}
BOOST_AUTO_TEST_SUITE_END()

struct ConstReverseIterator_
{
	CMyString empty;
	CMyString myStr = "12345";
	CMyString::const_reverse_iterator crbegin = myStr.crbegin();
	CMyString::const_reverse_iterator crend = myStr.crend();
};
//Константный реверсированный итератор
BOOST_FIXTURE_TEST_SUITE(Const_reverse_iterator, ConstReverseIterator_)
	//имеет операторы
	BOOST_AUTO_TEST_SUITE(has_operators)
		//разыменования
		BOOST_AUTO_TEST_CASE(dereference)
		{
			BOOST_CHECK_EQUAL(*crbegin, '5');
		}
		//префексный инкремент
		BOOST_AUTO_TEST_CASE(preincrement)
		{
			++crbegin;
			BOOST_CHECK_EQUAL(*crbegin, '4');
			BOOST_CHECK_EQUAL(*++crbegin, '3');
		}
		//постфиксный инкремент
		BOOST_AUTO_TEST_CASE(postincrement)
		{
			crbegin++;//-V803
			BOOST_CHECK_EQUAL(*crbegin, '4');
			BOOST_CHECK_EQUAL(*crbegin++, '4');
			BOOST_CHECK_EQUAL(*crbegin, '3');
		}
		//префексный декремент
		BOOST_AUTO_TEST_CASE(predecrement)
		{
			--crend;
			BOOST_CHECK_EQUAL(*crend, '1');
			BOOST_CHECK_EQUAL(*--crend, '2');
		}
		//постфиксный декремент
		BOOST_AUTO_TEST_CASE(postdecrement)
		{
			crend--;//-V803
			BOOST_CHECK_EQUAL(*crend, '1');
			BOOST_CHECK_EQUAL(*crend--, '1');
			BOOST_CHECK_EQUAL(*crend, '2');
		}
		//присваивание сложения
		BOOST_AUTO_TEST_CASE(addition_assignment)
		{
			crbegin += 1;
			BOOST_CHECK_EQUAL(*crbegin, '4');
			crbegin += 3;
			BOOST_CHECK_EQUAL(*crbegin, '1');
		}
		//сложения
		BOOST_AUTO_TEST_CASE(addition)
		{
			BOOST_CHECK_EQUAL(*(crbegin + 1), '4');
			BOOST_CHECK_EQUAL(*(crbegin + 4), '1');

			BOOST_CHECK_EQUAL(*(0 + crbegin), '5');
			BOOST_CHECK_EQUAL(*(3 + crbegin), '2');
		}
		//присваивание вычитания
		BOOST_AUTO_TEST_CASE(subtraction_assignment)
		{
			crend -= 2;
			BOOST_CHECK_EQUAL(*crend, '2');
			crend -= 1;
			BOOST_CHECK_EQUAL(*crend, '3');
		}
		//вычитания
		BOOST_AUTO_TEST_CASE(subtraction)
		{
			BOOST_CHECK_EQUAL(*(crend - 1), '1');
			BOOST_CHECK_EQUAL(*(crend - 5), '5');
		}
		//разность итераторов
		BOOST_AUTO_TEST_CASE(difference_of_iterators)
		{
			BOOST_CHECK_EQUAL(crbegin - crbegin, 0);//-V501
			BOOST_CHECK_EQUAL(crend - crbegin, 5);
			BOOST_CHECK_EQUAL(--crend - ++crbegin, 3);

			BOOST_CHECK_EQUAL(empty.crbegin() - empty.crend(), 0);
		}
		//индексации[]
		BOOST_AUTO_TEST_CASE(indexing)
		{
			BOOST_CHECK_EQUAL(crbegin[0], '5');
			BOOST_CHECK_EQUAL(crbegin[3], '2');
			BOOST_CHECK_EQUAL(crbegin[2], '3');
		}
		//сравнения
		BOOST_AUTO_TEST_CASE(comparison)
		{
			BOOST_CHECK((crbegin + 2) == (crend - 3));
			BOOST_CHECK(crbegin != crend);
			BOOST_CHECK(crbegin < crend);
			BOOST_CHECK(crend > crbegin);
			BOOST_CHECK((crend - 2) >= (crbegin + 2));
			BOOST_CHECK((crend - 3) >= (crbegin + 2));
			BOOST_CHECK((crbegin + 1) <= (crend - 3));
			BOOST_CHECK((crbegin + 2) <= (crend - 3));

			auto it = crbegin;
			BOOST_CHECK(crbegin == it);
			BOOST_CHECK(++crbegin == (it + 1));
			BOOST_CHECK(crbegin != it);

			BOOST_CHECK(empty.crbegin() == empty.crend());
		}
	BOOST_AUTO_TEST_SUITE_END()
	//совместим с алгоритмами STL
	BOOST_AUTO_TEST_CASE(compatible_with_STL_algorithms)
	{
		auto res = minmax_element(myStr.crbegin(), myStr.crend());
		BOOST_CHECK_EQUAL(*res.first, '1');
		BOOST_CHECK_EQUAL(*res.second, '5');

		stringstream strm;
		copy(myStr.crbegin(), myStr.crend(), ostream_iterator<char>(strm));
		BOOST_CHECK_EQUAL(strm.str(), "54321");
	}
BOOST_AUTO_TEST_SUITE_END()
