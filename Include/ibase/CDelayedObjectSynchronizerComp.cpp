// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ibase/CDelayedObjectSynchronizerComp.h>


// ACF includes
#include <imod/IModel.h>


namespace ibase
{


// protected methods

// reimplemented (imod::CSingleModelObserverBase)

void CDelayedObjectSynchronizerComp::OnUpdate(const istd::IChangeable::ChangeSet& changeSet)
{
	// Attaching to the reference object initializes the slaves without delay
	if (!m_isInitialized){
		BaseClass::OnUpdate(changeSet);

		return;
	}

	m_delayTimer.start();
}


// reimplemented (icomp::CComponentBase)

void CDelayedObjectSynchronizerComp::OnComponentCreated()
{
	m_delayTimer.setSingleShot(true);
	m_delayTimer.setInterval(*m_delayAttrPtr);

	QObject::connect(&m_delayTimer, &QTimer::timeout, [this](){
		BaseClass::OnUpdate(istd::IChangeable::GetAnyChange());
	});

	BaseClass::OnComponentCreated();

	m_isInitialized = true;
}


void CDelayedObjectSynchronizerComp::OnComponentDestroyed()
{
	m_delayTimer.stop();

	BaseClass::OnComponentDestroyed();
}


} // namespace ibase


