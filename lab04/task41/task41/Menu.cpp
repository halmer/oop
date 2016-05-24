#include "stdafx.h"
#include "Menu.h"

using namespace std;

void CMenu::AddItem(string const & shortcut, string const & description, Command const & command)
{
	m_items.emplace_back(shortcut, description, command);
}

void CMenu::Run()
{
	ShowInstructions();
	string command;
	while ((cout << ">") && (cin >> command) && ExecuteCommand(command))
	{}
}

void CMenu::ShowInstructions() const
{
	cout << "Commands list:\n";
	for (auto const & item : m_items)
	{
		cout << " " << item.shortcut << ": " << item.description << "\n";
	}
}

void CMenu::Exit()
{
	m_exit = true;
}

bool CMenu::ExecuteCommand(string const & command)
{
	m_exit = false;
	auto it = find_if(m_items.begin(), m_items.end(), [&]
		(Item const & item) { return item.shortcut == command; }
	);

	if (it != m_items.end())
	{
		it->command();
	}
	else
	{
		while (cin.get() != '\n')
		{}
		cout << "Unknown command\n";
	}

	return !m_exit;
}
