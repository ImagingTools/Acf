#ifndef iqt_CHierarchicalCommand_included
#define iqt_CHierarchicalCommand_included


#include <QAction>

#include "istd/TOptPointerVector.h"

#include "ibase/TNamedBase.h"
#include "ibase/THierarchicalBase.h"
#include "ibase/TEnableableWrap.h"

#include "idoc/ICommand.h"


namespace iqt
{


/**
	Implementation of hierarchical command based on \c QAction from Qt.
*/
class CHierarchicalCommand:
			public QAction,
			public ibase::TEnableableWrap< ibase::THierarchicalBase< ibase::TNamedBase<idoc::IHierarchicalCommand> > >
{
public:
	typedef QAction BaseClass;
	typedef ibase::TEnableableWrap< ibase::THierarchicalBase< ibase::TNamedBase<idoc::IHierarchicalCommand> > > BaseClass2;

	CHierarchicalCommand(const QString& text, QObject* parentPtr, int priority = 100, int staticFlags = CF_ALL_ENABLED);
	CHierarchicalCommand(const QIcon& icon, const QString& text, QObject* parentPtr, int priority = 100, int staticFlags = CF_ALL_ENABLED);

	/**
		Reset list of childs.
	*/
	void ResetChilds();
	/**
		Insert command to child list.
		Please note, this pointer is not owned by this container and will not be removed.
	*/
	void InsertChild(CHierarchicalCommand* commandPtr, bool releaseFlag = false);

	// reimplemented (idoc::ICommand)
	virtual int GetPriority() const;
	virtual int GetStaticFlags() const;
	virtual bool Execute(istd::IPolymorphic* contextPtr);

	// reimplemented (istd::TIHierarchical<idoc::ICommand>)
	virtual int GetChildsCount() const;
	virtual idoc::ICommand* GetChild(int index) const;

private:
	int m_priority;
	int m_staticFlags;

	istd::TOptPointerVector<CHierarchicalCommand> m_childs;
};


} // namespace iqt


#endif // !iqt_CHierarchicalCommand_included


