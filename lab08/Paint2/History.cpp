#include "stdafx.h"
#include "History.h"

void CHistory::AddCommand(ExecUnexecCommand const & command)
{
	m_redo.clear();
	m_undo.push_back(command);
}

void CHistory::AddCommandAndExecute(ExecUnexecCommand const & command)
{
	AddCommand(command);
	command.execute();
}

void CHistory::Undo()
{
	if (CanUndo())
	{
		m_undo.back().unexecute();
		m_redo.push_back(m_undo.back());
		m_undo.pop_back();
	}
}

void CHistory::Redo()
{
	if (CanRedo())
	{
		m_redo.back().execute();
		m_undo.push_back(m_redo.back());
		m_redo.pop_back();
	}
}

bool CHistory::CanUndo() const
{
	return !m_undo.empty();
}

bool CHistory::CanRedo() const
{
	return !m_redo.empty();
}

void CHistory::Reset()
{
	m_undo.clear();
	m_redo.clear();
}
