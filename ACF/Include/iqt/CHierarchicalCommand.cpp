#include "iqt/CHierarchicalCommand.h"


namespace iqt
{


CHierarchicalCommand::CHierarchicalCommand(const QString& text, QObject* parentPtr, int priority, int staticFlags)
:	BaseClass(text, parentPtr),
	m_priority(priority),
	m_staticFlags(staticFlags)
{
}


CHierarchicalCommand::CHierarchicalCommand(const QIcon& icon, const QString& text, QObject* parentPtr, int priority, int staticFlags)
:	BaseClass(icon, text, parentPtr),
	m_priority(priority),
	m_staticFlags(staticFlags)
{
}


void CHierarchicalCommand::ResetChilds()
{
	m_childs.Reset();
}


void CHierarchicalCommand::InsertChild(CHierarchicalCommand* commandPtr, bool releaseFlag)
{
	m_childs.PushBack(commandPtr, releaseFlag);
}


// reimplemented (idoc::ICommand)

int CHierarchicalCommand::GetPriority() const
{
	return m_priority;
}


int CHierarchicalCommand::GetStaticFlags() const
{
	return m_staticFlags;
}


bool CHierarchicalCommand::Execute(istd::IPolymorphic* /*contextPtr*/)
{
	if (IsEnabled()){
		emit trigger();
		return true;
	}
	else{
		return false;
	}
}


// reimplemented (istd::TIHierarchical<Interface>)

int CHierarchicalCommand::GetChildsCount() const
{
	return m_childs.GetCount();
}


idoc::ICommand* CHierarchicalCommand::GetChild(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < m_childs.GetCount());

	return m_childs.GetAt(index);
}


} // namespace iqt


