#include "stdafx.h"
#include "../task62/MyStringList.h"

using namespace std;

bool VerifyProperties(CMyStringList const & myLst, vector<string> const & expectedValue, size_t expectedSize)
{
	if (myLst.Size() == expectedSize && expectedValue.size() == expectedSize)
	{
		return equal(expectedValue.begin(), expectedValue.end(), myLst.begin());
	}

	return false;
}

//MyStringList
BOOST_AUTO_TEST_SUITE(MyStringList)
	//можно создать
	BOOST_AUTO_TEST_SUITE(can_create)
		//конструктором по умолчанию
		BOOST_AUTO_TEST_CASE(default_constructor)
		{
			CMyStringList myLst;
			BOOST_CHECK(VerifyProperties(myLst, {}, 0));
		}
		//данными из диапазона [first,last)
		BOOST_AUTO_TEST_CASE(range_first_last)
		{
			{
				vector<string> vec;
				CMyStringList myLst(vec.begin(), vec.end());
				BOOST_CHECK(VerifyProperties(myLst, {}, 0));

				list<string> lst;
				CMyStringList myLst1(lst.begin(), lst.end());
				BOOST_CHECK(VerifyProperties(myLst1, {}, 0));
			}
			{
				vector<string> vec{ "1", "22", "333" };
				CMyStringList myLst(vec.begin(), vec.end());
				BOOST_CHECK(VerifyProperties(myLst, { "1", "22", "333" }, 3));

				list<string> lst{ "1", "22", "333", "4444", "55555" };
				CMyStringList myLst1(lst.begin(), lst.end());
				BOOST_CHECK(VerifyProperties(myLst1, { "1", "22", "333", "4444", "55555" }, 5));
			}
		}
		//данными initializer_list
		BOOST_AUTO_TEST_CASE(initializer_list)
		{
			{
				CMyStringList myLst{};
				BOOST_CHECK(VerifyProperties(myLst, {}, 0));
			}
			{
				CMyStringList myLst{ "" };
				BOOST_CHECK(VerifyProperties(myLst, { "" }, 1));
			}
			{
				CMyStringList myLst{ "1" };
				BOOST_CHECK(VerifyProperties(myLst, { "1" }, 1));
			}
			{
				CMyStringList myLst{ "1", "22", "333", "4444", "55555" };
				BOOST_CHECK(VerifyProperties(myLst, { "1", "22", "333", "4444", "55555" }, 5));
			}
		}
		//конструктором копирования
		BOOST_AUTO_TEST_CASE(copy_constructor)
		{
			{
				CMyStringList const myLstOrig;
				CMyStringList myLst(myLstOrig);
				BOOST_CHECK(VerifyProperties(myLst, {}, myLstOrig.Size()));
			}
			{
				CMyStringList const myLstOrig{ "1", "22", "333", "4444", "55555" };
				CMyStringList myLst(myLstOrig);
				BOOST_CHECK(VerifyProperties(myLst, { "1", "22", "333", "4444", "55555" }, myLstOrig.Size()));
			}
		}
	BOOST_AUTO_TEST_SUITE_END()
	//имеет оператор присваивания
	BOOST_AUTO_TEST_CASE(has_operator_assignment)
	{
		CMyStringList myLst;
		CMyStringList const empty;
		CMyStringList const lst{ "1", "22", "333" };

		myLst = empty;
		BOOST_CHECK(VerifyProperties(myLst, {}, 0));
		myLst = lst;
		BOOST_CHECK(VerifyProperties(myLst, { "1", "22", "333" }, 3));
		myLst = empty;
		BOOST_CHECK(VerifyProperties(myLst, {}, 0));

		myLst = lst;
		BOOST_CHECK(VerifyProperties(myLst, { "1", "22", "333" }, 3));
		myLst = myLst;//-V570
		BOOST_CHECK(VerifyProperties(myLst, { "1", "22", "333" }, 3));
		myLst = empty;
		BOOST_CHECK(VerifyProperties(myLst, {}, 0));
		myLst = myLst;//-V570
		BOOST_CHECK(VerifyProperties(myLst, {}, 0));
	}
	//можно вставить элемент в начало
	BOOST_AUTO_TEST_CASE(can_insert_element_at_beginning)
	{
		{
			CMyStringList myLst;
			myLst.PushFront("1");
			BOOST_CHECK(VerifyProperties(myLst, { "1" }, 1));

			myLst.PushFront("22");
			BOOST_CHECK(VerifyProperties(myLst, { "22", "1" }, 2));
		}
		{
			CMyStringList myLst{ "1" };
			myLst.PushFront("22");
			BOOST_CHECK(VerifyProperties(myLst, { "22", "1" }, 2));

			myLst.PushFront("333");
			BOOST_CHECK(VerifyProperties(myLst, { "333", "22", "1" }, 3));
		}
	}
	//можно добавить элемент в конец
	BOOST_AUTO_TEST_CASE(can_add_element_at_end)
	{
		{
			CMyStringList myLst;
			myLst.PushBack("1");
			BOOST_CHECK(VerifyProperties(myLst, { "1" }, 1));

			myLst.PushBack("22");
			BOOST_CHECK(VerifyProperties(myLst, { "1", "22" }, 2));
		}
		{
			CMyStringList myLst{ "1" };
			myLst.PushBack("22");
			BOOST_CHECK(VerifyProperties(myLst, { "1", "22" }, 2));

			myLst.PushBack("333");
			BOOST_CHECK(VerifyProperties(myLst, { "1", "22", "333" }, 3));
		}
	}
	//можно вставить элемент в указанную позицию
	BOOST_AUTO_TEST_CASE(can_insert_element_at_specified_position)
	{
		{
			CMyStringList myLst;

			auto begin = myLst.begin();
			auto end = myLst.end();
			auto it = myLst.Insert(begin, "1");
			BOOST_CHECK(VerifyProperties(myLst, { "1" }, 1));
			BOOST_CHECK(*it == "1");
			BOOST_CHECK(begin == ++myLst.begin());
			BOOST_CHECK(end == myLst.end());

			begin = myLst.begin();
			end = myLst.end();
			it = myLst.Insert(end, "22");
			BOOST_CHECK(VerifyProperties(myLst, { "1", "22" }, 2));
			BOOST_CHECK(*it == "22");
			BOOST_CHECK(begin == myLst.begin());
			BOOST_CHECK(end == myLst.end());

			begin = myLst.begin();
			end = --myLst.end();
			it = myLst.Insert(it, "333");
			BOOST_CHECK(VerifyProperties(myLst, { "1", "333", "22" }, 3));
			BOOST_CHECK(*it == "333");
			BOOST_CHECK(begin == myLst.begin());
			BOOST_CHECK(end == --myLst.end());
		}
		{
			CMyStringList myLst{ "1" };

			auto begin = myLst.begin();
			auto end = myLst.end();
			auto it = myLst.Insert(begin, "22");
			BOOST_CHECK(VerifyProperties(myLst, { "22", "1" }, 2));
			BOOST_CHECK(*it == "22");
			BOOST_CHECK(begin == ++myLst.begin());
			BOOST_CHECK(end == myLst.end());

			begin = myLst.begin();
			end = myLst.end();
			it = myLst.Insert(end, "333");
			BOOST_CHECK(VerifyProperties(myLst, { "22", "1", "333" }, 3));
			BOOST_CHECK(*it == "333");
			BOOST_CHECK(begin == myLst.begin());
			BOOST_CHECK(end == myLst.end());

			begin = myLst.begin();
			end = --myLst.end();
			it = myLst.Insert(it, "4444");
			BOOST_CHECK(VerifyProperties(myLst, { "22", "1", "4444", "333" }, 4));
			BOOST_CHECK(*it == "4444");
			BOOST_CHECK(begin == myLst.begin());
			BOOST_CHECK(end == --myLst.end());
		}
	}
	//можно удалить элемент в указанной позиции
	BOOST_AUTO_TEST_CASE(can_delete_element_at_specified_position)
	{
		{
			CMyStringList myLst{ "1" };

			auto begin = myLst.begin();
			auto end = myLst.end();
			auto it = myLst.Erase(begin);
			BOOST_CHECK(VerifyProperties(myLst, {}, 0));
			BOOST_CHECK(it == end);
			BOOST_CHECK(end == myLst.end());
		}
		{
			CMyStringList myLst{ "1" , "22" };

			auto begin = myLst.begin();
			auto end = myLst.end();
			auto it = myLst.Erase(--myLst.end());
			BOOST_CHECK(VerifyProperties(myLst, { "1" }, 1));
			BOOST_CHECK(it == end);
			BOOST_CHECK(begin == myLst.begin());
			BOOST_CHECK(end == myLst.end());
		}
		{
			CMyStringList myLst{ "1" , "22", "333" };

			auto begin = myLst.begin();
			auto end = --myLst.end();
			auto it = myLst.Erase(++myLst.begin());
			BOOST_CHECK(VerifyProperties(myLst, { "1", "333" }, 2));
			BOOST_CHECK(*it == "333");
			BOOST_CHECK(begin == myLst.begin());
			BOOST_CHECK(end == --myLst.end());
		}
	}
	//можно узнать количество элементов
	BOOST_AUTO_TEST_CASE(can_learn_number_of_elements)
	{
		CMyStringList const myLst;
		BOOST_CHECK_EQUAL(myLst.Size(), 0);

		CMyStringList myLst1{ "1", "22", "333" };
		BOOST_CHECK_EQUAL(myLst1.Size(), 3);
		myLst1.PushBack("4444");
		BOOST_CHECK_EQUAL(myLst1.Size(), 4);
		myLst1.Erase(myLst1.begin());
		BOOST_CHECK_EQUAL(myLst1.Size(), 3);
	}
	//можно узнать пуст список или нет
	BOOST_AUTO_TEST_CASE(can_learn_whether_list_empty)
	{
		CMyStringList const myLst;
		BOOST_CHECK_EQUAL(myLst.Empty(), true);

		CMyStringList myLst1{ "1", "22", "333" };
		BOOST_CHECK_EQUAL(myLst1.Empty(), false);
		myLst1.Clear();
		BOOST_CHECK_EQUAL(myLst1.Empty(), true);

	}
	//можно очистить список
	BOOST_AUTO_TEST_CASE(can_clear_list)
	{
		CMyStringList myLst;
		myLst.Clear();
		BOOST_CHECK(VerifyProperties(myLst, {}, 0));
		{
			CMyStringList myLst{"123"};
			myLst.Clear();
			BOOST_CHECK(VerifyProperties(myLst, {}, 0));

			myLst.PushFront("1");
			BOOST_CHECK(VerifyProperties(myLst, { "1" }, 1));
		}
		{
			myLst.Clear();
			myLst.PushBack("22");
			BOOST_CHECK(VerifyProperties(myLst, { "22" }, 1));
		}
		{
			myLst.Clear();
			myLst.Insert(myLst.begin(), "333");
			BOOST_CHECK(VerifyProperties(myLst, { "333" }, 1));
		}
		{
			myLst.Clear();
			myLst.Insert(myLst.end(), "4444");
			BOOST_CHECK(VerifyProperties(myLst, { "4444" }, 1));
		}
		{
			myLst.Clear();
			myLst.PushFront("1");
			myLst.PushBack("22");
			myLst.Insert(myLst.begin(), "333");
			myLst.Insert(myLst.end(), "4444");
			BOOST_CHECK(VerifyProperties(myLst, { "333", "1", "22", "4444" }, 4));
		}
		{
			myLst.Clear();
			CMyStringList const newLst{ "1", "22", "333" };
			myLst = newLst;
			BOOST_CHECK(VerifyProperties(myLst, { "1", "22", "333" }, 3));
		}
		{
			myLst.Clear();
			CMyStringList const newLst(myLst);
			BOOST_CHECK(VerifyProperties(newLst, {}, 0));
		}
		{
			myLst.Clear();
			CMyStringList newLst{ "1", "22", "333" };
			newLst = myLst;//-V519
			BOOST_CHECK(VerifyProperties(newLst, {}, 0));
		}
	}
BOOST_AUTO_TEST_SUITE_END()

struct Iterator_
{
	CMyStringList empty;
	CMyStringList myLst{ "1", "22", "333", "4444", "55555" };
	CMyStringList::iterator begin = myLst.begin();
	CMyStringList::iterator end = myLst.end();
};
//Итератор
BOOST_FIXTURE_TEST_SUITE(Iterator, Iterator_)
	//имеет операторы
	BOOST_AUTO_TEST_SUITE(has_operators)
		//разыменования
		BOOST_AUTO_TEST_CASE(dereference)
		{
			BOOST_CHECK_EQUAL(*begin, "1");
			BOOST_CHECK_EQUAL(*--end, "55555");
		}
		//стрелка
		BOOST_AUTO_TEST_CASE(member_access)
		{
			BOOST_CHECK_EQUAL(begin->size(), 1);
			BOOST_CHECK_EQUAL((--end)->data(), "55555");
		}
		//префексный инкремент
		BOOST_AUTO_TEST_CASE(preincrement)
		{
			++begin;
			BOOST_CHECK_EQUAL(*begin, "22");
			BOOST_CHECK_EQUAL(*++begin, "333");

			*begin = "_";
			BOOST_CHECK(VerifyProperties(myLst, { "1", "22", "_", "4444", "55555" }, 5));
		}
		//постфиксный инкремент
		BOOST_AUTO_TEST_CASE(postincrement)
		{
			begin++;//-V803
			BOOST_CHECK_EQUAL(*begin, "22");
			BOOST_CHECK_EQUAL(*begin++, "22");
			BOOST_CHECK_EQUAL(*begin, "333");

			*begin = "_";
			BOOST_CHECK(VerifyProperties(myLst, { "1", "22", "_", "4444", "55555" }, 5));
		}
		//префексный декремент
		BOOST_AUTO_TEST_CASE(predecrement)
		{
			--end;
			BOOST_CHECK_EQUAL(*end, "55555");
			BOOST_CHECK_EQUAL(*--end, "4444");

			*end = "_";
			BOOST_CHECK(VerifyProperties(myLst, { "1", "22", "333", "_", "55555" }, 5));
		}
		//постфиксный декремент
		BOOST_AUTO_TEST_CASE(postdecrement)
		{
			end--;//-V803
			BOOST_CHECK_EQUAL(*end, "55555");
			BOOST_CHECK_EQUAL(*end--, "55555");
			BOOST_CHECK_EQUAL(*end, "4444");

			*end = "_";
			BOOST_CHECK(VerifyProperties(myLst, { "1", "22", "333", "_", "55555" }, 5));
		}
		//сравнения
		BOOST_AUTO_TEST_CASE(comparison)
		{
			BOOST_CHECK(begin != end);
			BOOST_CHECK(++(++(++begin)) == --(--end));
			
			auto it = begin = myLst.begin();
			BOOST_CHECK(++begin != it);
			BOOST_CHECK(begin == ++it);

			BOOST_CHECK(empty.begin() == empty.end());
		}
		//приведения типа
		BOOST_AUTO_TEST_CASE(type_cast)
		{
			BOOST_CHECK_EQUAL(typeid(begin).name(),
				"class CMyStringList::iterator");

			CMyStringList::const_iterator cbegin = begin;
			BOOST_CHECK_EQUAL(typeid(cbegin).name(),
				"class CMyStringList::const_iterator");

			BOOST_CHECK_EQUAL(typeid(static_cast<CMyStringList::const_iterator>(begin)).name(),
				"class CMyStringList::const_iterator");
		}
	BOOST_AUTO_TEST_SUITE_END()
	//совместим с алгоритмами STL
	BOOST_AUTO_TEST_CASE(compatible_with_STL_algorithms)
	{
		auto res = minmax_element(myLst.begin(), myLst.end());
		BOOST_CHECK_EQUAL(*res.first, "1");
		BOOST_CHECK_EQUAL(*res.second, "55555");
	
		stringstream strm;
		copy(myLst.begin(), myLst.end(), ostream_iterator<string>(strm, " "));
		BOOST_CHECK_EQUAL(strm.str(), "1 22 333 4444 55555 ");

		reverse(myLst.begin(), myLst.end());
		strm.str("");
		for (auto const & str : myLst)
		{
			strm << str << " ";
		}
		BOOST_CHECK_EQUAL(strm.str(), "55555 4444 333 22 1 ");

		auto it = stable_partition(myLst.begin(), myLst.end(),
			[](string const & str) { return (str.size() % 2 == 1); });
		auto odd = { "55555", "333", "1" };
		auto even = { "4444", "22" };
		BOOST_CHECK(equal(myLst.begin(), it, odd.begin()));
		BOOST_CHECK(equal(it, myLst.end(), even.begin()));

		strm.str("");
		for (auto const & str : empty)
		{
			strm << str;
		}
		BOOST_CHECK_EQUAL(strm.str(), "");
	}
BOOST_AUTO_TEST_SUITE_END()

struct ConstIterator_
{
	CMyStringList const empty;
	CMyStringList const myLst{ "1", "22", "333", "4444", "55555" };
	CMyStringList::const_iterator begin = myLst.begin();
	CMyStringList::const_iterator end = myLst.end();
};
//Константный итератор
BOOST_FIXTURE_TEST_SUITE(Const_iterator, ConstIterator_)
	//имеет операторы
	BOOST_AUTO_TEST_SUITE(has_operators)
		//разыменования
		BOOST_AUTO_TEST_CASE(dereference)
		{
			BOOST_CHECK_EQUAL(*begin, "1");
			BOOST_CHECK_EQUAL(*--end, "55555");
		}
		//стрелка
		BOOST_AUTO_TEST_CASE(member_access)
		{
			BOOST_CHECK_EQUAL(begin->size(), 1);
			BOOST_CHECK_EQUAL((--end)->data(), "55555");
		}
		//префексный инкремент
		BOOST_AUTO_TEST_CASE(preincrement)
		{
			++begin;
			BOOST_CHECK_EQUAL(*begin, "22");
			BOOST_CHECK_EQUAL(*++begin, "333");
		}
		//постфиксный инкремент
		BOOST_AUTO_TEST_CASE(postincrement)
		{
			begin++;//-V803
			BOOST_CHECK_EQUAL(*begin, "22");
			BOOST_CHECK_EQUAL(*begin++, "22");
			BOOST_CHECK_EQUAL(*begin, "333");
		}
		//префексный декремент
		BOOST_AUTO_TEST_CASE(predecrement)
		{
			--end;
			BOOST_CHECK_EQUAL(*end, "55555");
			BOOST_CHECK_EQUAL(*--end, "4444");
		}
		//постфиксный декремент
		BOOST_AUTO_TEST_CASE(postdecrement)
		{
			end--;//-V803
			BOOST_CHECK_EQUAL(*end, "55555");
			BOOST_CHECK_EQUAL(*end--, "55555");
			BOOST_CHECK_EQUAL(*end, "4444");
		}
		//сравнения
		BOOST_AUTO_TEST_CASE(comparison)
		{
			BOOST_CHECK(begin != end);
			BOOST_CHECK(++(++(++begin)) == --(--end));

			auto it = begin = myLst.begin();
			BOOST_CHECK(++begin != it);
			BOOST_CHECK(begin == ++it);

			BOOST_CHECK(empty.begin() == empty.end());
		}
	BOOST_AUTO_TEST_SUITE_END()
	//совместим с алгоритмами STL
	BOOST_AUTO_TEST_CASE(compatible_with_STL_algorithms)
	{
		auto res = minmax_element(myLst.begin(), myLst.end());
		BOOST_CHECK_EQUAL(*res.first, "1");
		BOOST_CHECK_EQUAL(*res.second, "55555");

		stringstream strm;
		copy(myLst.begin(), myLst.end(), ostream_iterator<string>(strm, " "));
		BOOST_CHECK_EQUAL(strm.str(), "1 22 333 4444 55555 ");

		strm.str("");
		for (auto const & str : myLst)
		{
			strm << str << " ";
		}
		BOOST_CHECK_EQUAL(strm.str(), "1 22 333 4444 55555 ");

		strm.str("");
		for (auto const & str : empty)
		{
			strm << str;
		}
		BOOST_CHECK_EQUAL(strm.str(), "");
	}
BOOST_AUTO_TEST_SUITE_END()

struct ReverseIterator_
{
	CMyStringList empty;
	CMyStringList myLst{ "1", "22", "333", "4444", "55555" };
	CMyStringList::reverse_iterator rbegin = myLst.rbegin();
	CMyStringList::reverse_iterator rend = myLst.rend();
};
//Реверсированный итератор
BOOST_FIXTURE_TEST_SUITE(Reverse_iterator, ReverseIterator_)
	//имеет операторы
	BOOST_AUTO_TEST_SUITE(has_operators)
		//разыменования
		BOOST_AUTO_TEST_CASE(dereference)
		{
			BOOST_CHECK_EQUAL(*rbegin, "55555");
			BOOST_CHECK_EQUAL(*--rend, "1");
		}
		//стрелка
		BOOST_AUTO_TEST_CASE(member_access)
		{
			BOOST_CHECK_EQUAL(rbegin->size(), 5);
			BOOST_CHECK_EQUAL((--rend)->data(), "1");
		}
		//префексный инкремент
		BOOST_AUTO_TEST_CASE(preincrement)
		{
			++rbegin;
			BOOST_CHECK_EQUAL(*rbegin, "4444");
			BOOST_CHECK_EQUAL(*++rbegin, "333");

			*rbegin = "_";
			BOOST_CHECK(VerifyProperties(myLst, { "1", "22", "_", "4444", "55555" }, 5));
		}
		//постфиксный инкремент
		BOOST_AUTO_TEST_CASE(postincrement)
		{
			rbegin++;//-V803
			BOOST_CHECK_EQUAL(*rbegin, "4444");
			BOOST_CHECK_EQUAL(*rbegin++, "4444");
			BOOST_CHECK_EQUAL(*rbegin, "333");

			*rbegin = "_";
			BOOST_CHECK(VerifyProperties(myLst, { "1", "22", "_", "4444", "55555" }, 5));
		}
		//префексный декремент
		BOOST_AUTO_TEST_CASE(predecrement)
		{
			--rend;
			BOOST_CHECK_EQUAL(*rend, "1");
			BOOST_CHECK_EQUAL(*--rend, "22");

			*rend = "_";
			BOOST_CHECK(VerifyProperties(myLst, { "1", "_", "333", "4444", "55555" }, 5));
		}
		//постфиксный декремент
		BOOST_AUTO_TEST_CASE(postdecrement)
		{
			rend--;//-V803
			BOOST_CHECK_EQUAL(*rend, "1");
			BOOST_CHECK_EQUAL(*rend--, "1");
			BOOST_CHECK_EQUAL(*rend, "22");

			*rend = "_";
			BOOST_CHECK(VerifyProperties(myLst, { "1", "_", "333", "4444", "55555" }, 5));
		}
		//сравнения
		BOOST_AUTO_TEST_CASE(comparison)
		{
			BOOST_CHECK(rbegin != rend);
			BOOST_CHECK(++(++(++rbegin)) == --(--rend));

			auto it = rbegin = myLst.rbegin();
			BOOST_CHECK(++rbegin != it);
			BOOST_CHECK(rbegin == ++it);

			BOOST_CHECK(empty.rbegin() == empty.rend());
		}
		//приведения типа
		BOOST_AUTO_TEST_CASE(type_cast)
		{
			BOOST_CHECK_EQUAL(typeid(rbegin).name(),
				"class std::reverse_iterator<class CMyStringList::iterator>");

			CMyStringList::const_reverse_iterator crbegin = rbegin;
			BOOST_CHECK_EQUAL(typeid(crbegin).name(),
				"class std::reverse_iterator<class CMyStringList::const_iterator>");

			BOOST_CHECK_EQUAL(typeid(static_cast<reverse_iterator<class CMyStringList::const_iterator>>(rbegin)).name(),
				"class std::reverse_iterator<class CMyStringList::const_iterator>");
		}
	BOOST_AUTO_TEST_SUITE_END()
	//совместим с алгоритмами STL
	BOOST_AUTO_TEST_CASE(compatible_with_STL_algorithms)
	{
		auto res = minmax_element(myLst.rbegin(), myLst.rend());
		BOOST_CHECK_EQUAL(*res.first, "1");
		BOOST_CHECK_EQUAL(*res.second, "55555");

		stringstream strm;
		copy(myLst.rbegin(), myLst.rend(), ostream_iterator<string>(strm, " "));
		BOOST_CHECK_EQUAL(strm.str(), "55555 4444 333 22 1 ");

		reverse(myLst.rbegin(), myLst.rend());
		strm.str("");
		for (auto const & str : myLst)
		{
			strm << str << " ";
		}
		BOOST_CHECK_EQUAL(strm.str(), "55555 4444 333 22 1 ");

		auto it = stable_partition(myLst.rbegin(), myLst.rend(),
			[](string const & str) { return (str.size() % 2 == 1); });
		vector<string> odd{ "55555", "333", "1" };
		vector<string> even{ "4444", "22" };
		BOOST_CHECK(equal(myLst.rbegin(), it, odd.rbegin()));
		BOOST_CHECK(equal(it, myLst.rend(), even.rbegin()));
	}
BOOST_AUTO_TEST_SUITE_END()

struct ConstReverseIterator_
{
	CMyStringList empty;
	CMyStringList myLst{ "1", "22", "333", "4444", "55555" };
	CMyStringList::const_reverse_iterator crbegin = myLst.crbegin();
	CMyStringList::const_reverse_iterator crend = myLst.crend();
};
//Константный реверсированный итератор
BOOST_FIXTURE_TEST_SUITE(Const_reverse_iterator, ConstReverseIterator_)
	//имеет операторы
	BOOST_AUTO_TEST_SUITE(has_operators)
		//разыменования
		BOOST_AUTO_TEST_CASE(dereference)
		{
			BOOST_CHECK_EQUAL(*crbegin, "55555");
			BOOST_CHECK_EQUAL(*--crend, "1");
		}
		//стрелка
		BOOST_AUTO_TEST_CASE(member_access)
		{
			BOOST_CHECK_EQUAL(crbegin->size(), 5);
			BOOST_CHECK_EQUAL((--crend)->data(), "1");
		}
		//префексный инкремент
		BOOST_AUTO_TEST_CASE(preincrement)
		{
			++crbegin;
			BOOST_CHECK_EQUAL(*crbegin, "4444");
			BOOST_CHECK_EQUAL(*++crbegin, "333");
		}
		//постфиксный инкремент
		BOOST_AUTO_TEST_CASE(postincrement)
		{
			crbegin++;//-V803
			BOOST_CHECK_EQUAL(*crbegin, "4444");
			BOOST_CHECK_EQUAL(*crbegin++, "4444");
			BOOST_CHECK_EQUAL(*crbegin, "333");
		}
		//префексный декремент
		BOOST_AUTO_TEST_CASE(predecrement)
		{
			--crend;
			BOOST_CHECK_EQUAL(*crend, "1");
			BOOST_CHECK_EQUAL(*--crend, "22");
		}
		//постфиксный декремент
		BOOST_AUTO_TEST_CASE(postdecrement)
		{
			crend--;//-V803
			BOOST_CHECK_EQUAL(*crend, "1");
			BOOST_CHECK_EQUAL(*crend--, "1");
			BOOST_CHECK_EQUAL(*crend, "22");
		}
		//сравнения
		BOOST_AUTO_TEST_CASE(comparison)
		{
			BOOST_CHECK(crbegin != crend);
			BOOST_CHECK(++(++(++crbegin)) == --(--crend));

			auto it = crbegin = myLst.crbegin();
			BOOST_CHECK(++crbegin != it);
			BOOST_CHECK(crbegin == ++it);

			BOOST_CHECK(empty.crbegin() == empty.crend());
		}
	BOOST_AUTO_TEST_SUITE_END()
	//совместим с алгоритмами STL
	BOOST_AUTO_TEST_CASE(compatible_with_STL_algorithms)
	{
		auto res = minmax_element(myLst.crbegin(), myLst.crend());
		BOOST_CHECK_EQUAL(*res.first, "1");
		BOOST_CHECK_EQUAL(*res.second, "55555");

		stringstream strm;
		copy(myLst.crbegin(), myLst.crend(), ostream_iterator<string>(strm, " "));
		BOOST_CHECK_EQUAL(strm.str(), "55555 4444 333 22 1 ");

		strm.str("");
		for (auto const & str : myLst)
		{
			strm << str << " ";
		}
		BOOST_CHECK_EQUAL(strm.str(), "1 22 333 4444 55555 ");
	}
BOOST_AUTO_TEST_SUITE_END()
