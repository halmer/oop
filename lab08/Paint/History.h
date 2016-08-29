#pragma once

class CHistory
{
	struct CommandExecUnexec;
public:
	void AddCommand(CommandExecUnexec const & command);
	void AddCommandAndExecute(CommandExecUnexec const & command);
	void Undo();
	void Redo();
	bool CanUndo() const;
	bool CanRedo() const;
	void Reset();
	
private:
	struct CommandExecUnexec
	{
		using Command = std::function<void()>;
		CommandExecUnexec(Command const & execute, Command const & unexecute)
			: execute(execute)
			, unexecute(unexecute)
		{}
		Command execute;
		Command unexecute;
	};

	std::vector<CommandExecUnexec> m_undo;
	std::vector<CommandExecUnexec> m_redo;
};
