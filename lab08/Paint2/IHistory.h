#pragma once

struct ExecUnexecCommand
{
	using Command = std::function<void()>;
	ExecUnexecCommand(Command const & execute, Command const & unexecute)
		: execute(execute)
		, unexecute(unexecute)
	{}
	Command execute;
	Command unexecute;
};

class IHistory
{
public:
	virtual void AddCommand(ExecUnexecCommand const & command) = 0;
	virtual void AddCommandAndExecute(ExecUnexecCommand const & command) = 0;
	virtual void Undo() = 0;
	virtual void Redo() = 0;
	virtual bool CanUndo() const = 0;
	virtual bool CanRedo() const = 0;
	virtual void Reset() = 0;

	virtual ~IHistory() = default;
};
