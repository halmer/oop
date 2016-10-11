#pragma once
#include "IHistory.h"

class CHistory : public IHistory
{
public:
	void AddCommand(ExecUnexecCommand const & command) override;
	void AddCommandAndExecute(ExecUnexecCommand const & command) override;
	void Undo() override;
	void Redo() override;
	bool CanUndo() const override;
	bool CanRedo() const override;
	void Reset() override;

private:
	std::vector<ExecUnexecCommand> m_undo;
	std::vector<ExecUnexecCommand> m_redo;
};
