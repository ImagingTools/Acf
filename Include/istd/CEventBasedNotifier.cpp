// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <istd/CEventBasedNotifier.h>


// Qt includes
#include <QtCore/QCoreApplication>
#include <QtCore/QThread>


namespace istd
{


// public methods

CEventBasedNotifier::CEventBasedNotifier(istd::IChangeable* slavePtr,  const IChangeable::ChangeSet* changeSetPtr)
{
	Q_ASSERT(changeSetPtr != NULL);

	if (slavePtr != NULL){
		m_asyncNotifierPtr = new CAsyncNotifier(slavePtr, *changeSetPtr);
	}
	else{
		m_asyncNotifierPtr = NULL;
	}
}


CEventBasedNotifier::~CEventBasedNotifier()
{
	if (m_asyncNotifierPtr != NULL){
		m_asyncNotifierPtr->deleteLater();
	}
}


// public methods of the internal class CAsyncNotifier

CAsyncNotifier::CAsyncNotifier(istd::IChangeable* slavePtr, const IChangeable::ChangeSet& changeSet)
:	m_slavePtr(slavePtr),
	m_changeIds(changeSet),
 	m_isBeginCalled(false)
{
	Q_ASSERT(slavePtr != NULL);

	if (QCoreApplication::instance() != NULL){
		moveToThread(QCoreApplication::instance()->thread());
	}

	connect(this, SIGNAL(EmitBeginChanges()), this, SLOT(DoBeginChanges()));

	Q_EMIT EmitBeginChanges();
}


CAsyncNotifier::~CAsyncNotifier()
{
	Q_ASSERT(m_isBeginCalled);

	if (m_isBeginCalled){
		m_slavePtr->EndChanges(m_changeIds);
	}
}


// protected slots

void CAsyncNotifier::DoBeginChanges()
{
	Q_ASSERT(!m_isBeginCalled);

	m_isBeginCalled = true;

	m_slavePtr->BeginChanges(m_changeIds);
}


} // namespace istd


