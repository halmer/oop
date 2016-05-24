#pragma once

class CMenu
{
public:
	using Command = std::function<void()>;

	void AddItem(std::string const & shortcut, std::string const & description, Command const & command);
	void Run();
	void ShowInstructions() const;
	void Exit();

private:
	struct Item
	{
		Item(std::string const & shortcut, std::string const & description, Command const & command)
			: shortcut(shortcut)
			, description(description)
			, command(command)
		{}

		std::string shortcut;
		std::string description;
		Command command;
	};

	bool ExecuteCommand(std::string const & command);

	std::vector<Item> m_items;
	bool m_exit = false;
};
