#include "stdafx.h"
#include "../task26/ExpandTemplate.h"

using namespace std;

struct ExpandTemplateFixture
{
	CExpandTemplate expTpl;
	map<string, string> replaceMap;
	void CheckSuccessfulReplacement(string const & origStr, string const & expectedStr)
	{
		expTpl.AddReplaceMap(replaceMap);
		BOOST_CHECK_EQUAL(expTpl.Replace(origStr), expectedStr);
	}
};

BOOST_FIXTURE_TEST_SUITE(ExpandTemplate, ExpandTemplateFixture)
	//при пустом шаблоне строка не меняется
	BOOST_AUTO_TEST_CASE(when_empty_template_string_does_not_change)
	{
		replaceMap = {};
		CheckSuccessfulReplacement("abcde", "abcde");

		replaceMap = { { "", "a" } };
		CheckSuccessfulReplacement("abcdefg", "abcdefg");
	}
	//пустая строка не меняется
	BOOST_AUTO_TEST_CASE(empty_string_does_not_change)
	{
		replaceMap = { { "a", "b" } };
		CheckSuccessfulReplacement("", "");
	}
	//при неподходящем шаблоне строка не меняется
	BOOST_AUTO_TEST_CASE(when_template_not_found_string_does_not_change)
	{
		replaceMap = { { "z", "_" } };
		CheckSuccessfulReplacement("abcdefg", "abcdefg");

		replaceMap = { { "abcdf", "_" } };
		CheckSuccessfulReplacement("abcdefg", "abcdefg");
	}
	//можно искать шаблон в разных строках
	BOOST_AUTO_TEST_CASE(can_search_template_in_different_strings)
	{
		replaceMap = { { "be", "[be]" },{ "abc", "[abc]" } };
		CheckSuccessfulReplacement("abcbefg", "[abc][be]fg");

		CheckSuccessfulReplacement("bebeabc", "[be][be][abc]");
	}
	//шаблоны поиска можно менять
	BOOST_AUTO_TEST_CASE(template_can_be_changed)
	{
		replaceMap = { { "a", "" } };
		CheckSuccessfulReplacement("aabaa", "b");

		replaceMap = { { "a", "[a]" } };
		CheckSuccessfulReplacement("abcde", "[a]bcde");

		replaceMap = { { "bc", "[bc]" },{ "de", "[de]" } };
		CheckSuccessfulReplacement("abcde", "a[bc][de]");
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(Examples, ExpandTemplateFixture)
	//примеры разных замен
	BOOST_AUTO_TEST_CASE(_)
	{
		replaceMap = { { "abc", "aa" } ,{ "b", "bc" } };
		CheckSuccessfulReplacement("ab", "abc");
		
		replaceMap = { {"abc", "aa"} , {"aaaa", "b"}};
		CheckSuccessfulReplacement("abcaa", "aaaa");
		
		replaceMap = { 
			{ "abc", "aa" },{ "ab", "[ab]" },{ "aa", "b" },{ "bc", "[bc]" } };
		CheckSuccessfulReplacement("abcaac", "aabc");
		
		replaceMap = { 
			{ "abc", "[abc]" },{ "bca", "[bca]" },{ "cab", "[cab]" },
			{ "a", "[a]" },{ "acb", "[acb]" },{ "bc", "[bc]" }};
		CheckSuccessfulReplacement(
			"abcabcacbbcacbaacbabc",
			"[abc][abc][acb][bca]cb[a][acb][abc]");
	
		replaceMap = {
			{ "A", "[a]" },{ "AA", "[aa]" },{ "B", "[b]" },
			{ "BB", "[bb]" },{ "C", "[c]" },{ "CC", "[cc]" }};
		CheckSuccessfulReplacement(
			"-AABBCCCCCABC+",
			"-[aa][bb][cc][cc][c][a][b][c]+");
	
		replaceMap = {
			{ "%USER_NAME%", "Ivan Petrov" },
			{ "{WEEK_DAY}", "Friday" }};
		CheckSuccessfulReplacement(
			"Hello, %USER_NAME%. Today is {WEEK_DAY}.",
			"Hello, Ivan Petrov. Today is Friday.");

		replaceMap = {
			{ "%USER_NAME%", "Super %USER_NAME% {WEEK_DAY}" },
			{ "{WEEK_DAY}", "Friday. {WEEK_DAY}" }};
		CheckSuccessfulReplacement(
			"Hello, %USER_NAME%. Today is {WEEK_DAY}.",
			"Hello, Super %USER_NAME% {WEEK_DAY}. Today is Friday. {WEEK_DAY}.");
	}
	
BOOST_AUTO_TEST_SUITE_END()
