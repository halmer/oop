#include "stdafx.h"
#include "History.h"

CHistory::CHistory()
	: m_state(0)
{
}

void CHistory::AddCommand(ExecUnexecCommand const & command)
{
	m_redo.clear();
	m_undo.push_back(command);

	if (m_state && *m_state >= m_undo.size())
	{
		m_state = boost::none;
	}
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
		m_redo.push_back(m_undo.back());
		m_undo.pop_back();
		m_redo.back().unexecute();
	}
}

void CHistory::Redo()
{
	if (CanRedo())
	{
		m_undo.push_back(m_redo.back());
		m_redo.pop_back();
		m_undo.back().execute();
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

void CHistory::SaveState()
{
	m_state = m_undo.size();
}

bool CHistory::IsModified() const
{
	if (m_state)
	{
		return *m_state != m_undo.size();
	}

	return true;
}
