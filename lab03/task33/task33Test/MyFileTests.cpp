#include "stdafx.h"
#include "../task33/MyFile.h"

using namespace std;

constexpr int bufOut[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9};

void CheckOpenFile(string const & fileName, string const & mode, bool expectIsOpened)
{
	CMyFile file(fileName, mode);
	BOOST_CHECK(file.IsOpened() == expectIsOpened);
}
//����
BOOST_AUTO_TEST_SUITE(MyFile)
	//����� ������� ������������� ��� ����������
	BOOST_AUTO_TEST_CASE(can_create_constructor_without_parameters)
	{
		CMyFile file;
		BOOST_CHECK(!file.IsOpened());
	}
	//����� ������� ������������� � �����������
	BOOST_AUTO_TEST_CASE(can_create_constructor_with_parameters)
	{
		//�������������� ����, ������������ ����� ��������
		CheckOpenFile("null.txt", "r", false);
		CheckOpenFile("null.txt", "ra", false);
		CheckOpenFile("", "w", false);
		CheckOpenFile("null.txt", "", false);
		CheckOpenFile("", "", false);
		//���������� ��������
		CheckOpenFile("temp.txt", "w", true);
	}
	//����� ��������� � ���������
	BOOST_AUTO_TEST_CASE(can_open_and_close)
	{
		CMyFile file;
		BOOST_CHECK(!file.IsOpened());
		BOOST_CHECK_EQUAL(file.Open("temp.txt", "w"), true);
		//������� � ������� �����
		file.Close();
		BOOST_CHECK(!file.IsOpened());
		BOOST_CHECK_EQUAL(file.Open("ABC.txt"), true);
		//�������� ������, ��� ������ �������� �������
		BOOST_CHECK_EQUAL(file.Open("temp.txt"), true);
	}
	//����� �������� ������ �� �����
	BOOST_AUTO_TEST_CASE(can_get_character_from_file)
	{
		//�������������� ����
		CMyFile file("null.txt");
		BOOST_CHECK_EQUAL(file.GetChar(), CMyFile::FILE_ERROR);
		//���������� ����
		file.Open("ABC.txt");
		BOOST_CHECK_EQUAL(file.GetChar(), 'A');
		BOOST_CHECK_EQUAL(file.GetChar(), 'B');
		BOOST_CHECK(!file.IsEndOfFile());
		BOOST_CHECK_EQUAL(file.GetChar(), 'C');
		BOOST_CHECK_EQUAL(file.GetChar(), EOF);
		BOOST_CHECK(file.IsEndOfFile());
	}
	//����� �������� ������ � ����
	BOOST_AUTO_TEST_CASE(can_write_character_to_file)
	{
		CMyFile file("temp.txt", "w");
		BOOST_CHECK(file.PutChar('1'));
		BOOST_CHECK(file.PutChar('2'));
		BOOST_CHECK(file.PutChar('3'));
		
		file.Open("temp.txt");
		BOOST_CHECK_EQUAL(file.GetChar(), '1');
		BOOST_CHECK_EQUAL(file.GetChar(), '2');
		BOOST_CHECK_EQUAL(file.GetChar(), '3');
		BOOST_CHECK_EQUAL(file.GetChar(), EOF);
	}
	//����� �������� ������� ������� � �����
	BOOST_AUTO_TEST_CASE(can_get_current_position_in_file)
	{
		CMyFile file("ABC.txt");
		BOOST_CHECK_EQUAL(file.GetPosition(), 0);
		BOOST_CHECK_EQUAL(file.GetChar(), 'A');
		BOOST_CHECK_EQUAL(file.GetPosition(), 1);
		BOOST_CHECK_EQUAL(file.GetChar(), 'B');
		BOOST_CHECK_EQUAL(file.GetPosition(), 2);
		
		file.Close();
		BOOST_CHECK_EQUAL(file.GetPosition(), -1);
	}
	//����� ��������� ���������������� � �����
	BOOST_AUTO_TEST_CASE(can_performed_positioning_in_file)
	{
		CMyFile file("ABCDEF.txt");
		BOOST_CHECK_EQUAL(file.Seek(2), true);
		BOOST_CHECK_EQUAL(file.GetChar(), 'C');
		BOOST_CHECK_EQUAL(file.Seek(0), true);
		BOOST_CHECK_EQUAL(file.GetChar(), 'A');
		BOOST_CHECK_EQUAL(file.Seek(-2, SEEK_END), true);
		BOOST_CHECK_EQUAL(file.GetChar(), 'E');
		BOOST_CHECK_EQUAL(file.Seek(-2, SEEK_CUR), true);
		BOOST_CHECK_EQUAL(file.GetChar(), 'D');
		
		file.Close();
		BOOST_CHECK_EQUAL(file.Seek(0), false);
	}
	//����� �������� ����� �����
	BOOST_AUTO_TEST_CASE(can_get_length_of_file)
	{
		//���������� � ���� - ������������� �����
		CMyFile file("temp.txt", "w");
		BOOST_CHECK_EQUAL(file.GetLength(), 0);
		BOOST_CHECK(file.PutChar('1'));
		BOOST_CHECK(file.PutChar('2'));
		BOOST_CHECK_EQUAL(file.GetLength(), 2);
		BOOST_CHECK(file.PutChar('3'));
		BOOST_CHECK(file.PutChar('4'));
		BOOST_CHECK(file.PutChar('5'));
		BOOST_CHECK_EQUAL(file.GetLength(), 5);
		//��� ������ ���������������� �� ���������
		file.Close();
		file.Open("temp.txt");
		BOOST_CHECK_EQUAL(file.GetLength(), 5);
		BOOST_CHECK_EQUAL(file.GetChar(), '1');
		BOOST_CHECK_EQUAL(file.GetLength(), 5);
		BOOST_CHECK_EQUAL(file.GetChar(), '2');
		BOOST_CHECK_EQUAL(file.GetLength(), 5);
	}
	//����� �������� ������ � ����
	BOOST_AUTO_TEST_CASE(can_write_data_to_file)
	{
		CMyFile file("temp.txt", "wb");
		int * ptr = nullptr;
		BOOST_CHECK_EQUAL(file.Write(ptr, 1, 10), -1);

		BOOST_CHECK_EQUAL(file.Write(
									bufOut,
									sizeof(bufOut[0]),
									sizeof(bufOut) / sizeof(bufOut[0])),
									sizeof(bufOut) / sizeof(bufOut[0]));
	}
	//����� ������� ������ �� ����
	BOOST_AUTO_TEST_CASE(can_read_data_from_file)
	{
		CMyFile file("temp.txt", "rb");
		int * ptr = nullptr;
		BOOST_CHECK_EQUAL(file.Read(ptr, 1, 10), -1);

		int bufIn[sizeof(bufOut) / sizeof(bufOut[0])];
		BOOST_CHECK_EQUAL(file.Read(
									bufIn,
									sizeof(bufIn[0]),
									sizeof(bufIn) / sizeof(bufIn[0])),
									sizeof(bufIn) / sizeof(bufIn[0]));
		BOOST_CHECK_EQUAL(memcmp(bufOut, bufIn, sizeof(bufOut)), 0);		
	}
BOOST_AUTO_TEST_SUITE_END()
