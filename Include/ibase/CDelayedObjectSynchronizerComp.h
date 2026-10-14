// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QTimer>

// ACF includes
#include <ibase/CObjectSynchronizerComp.h>


namespace ibase
{


/**
	Object synchronizer that propagates changes of the reference object to its slaves with a delay.
	A burst of changes is coalesced: the slaves are synchronized once, after the reference object
	stayed unchanged for the given delay. The initial synchronization is done immediately.

	\ingroup DataModel
*/
class CDelayedObjectSynchronizerComp: public CObjectSynchronizerComp
{
public:
	typedef CObjectSynchronizerComp BaseClass;

	I_BEGIN_COMPONENT(CDelayedObjectSynchronizerComp);
		I_ASSIGN(m_delayAttrPtr, "Delay", "Time in ms the reference object must stay unchanged before the slaves are synchronized", true, 500);
	I_END_COMPONENT;

protected:
	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet) override;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

private:
	I_ATTR(int, m_delayAttrPtr);

	QTimer m_delayTimer;
	bool m_isInitialized = false;
};


} // namespace ibase


