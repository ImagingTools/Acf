// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <imod/CModelUpdateBridge.h>


namespace imod
{


// public methods

CModelUpdateBridge::CModelUpdateBridge(istd::IChangeable* changeablePtr, int updateFlags)
:	m_changeablePtr(changeablePtr),
	m_updateFlags(updateFlags),
	m_modelListMutex(QReadWriteLock::Recursive)
{
}


CModelUpdateBridge::~CModelUpdateBridge()
{
	EnsureModelsDetached();
}


IModel* CModelUpdateBridge::GetObservedModel(int modelIndex) const
{
	QReadLocker lock(&m_modelListMutex);

	if (modelIndex < 0 || modelIndex >= m_models.count()){
		return nullptr;
	}

	return m_models.at(modelIndex);
}


int CModelUpdateBridge::GetModelCount() const
{
	QReadLocker lock(&m_modelListMutex);

	return m_models.count();
}


void CModelUpdateBridge::EnsureModelsDetached()
{
	Models models;
	{
		QReadLocker lock(&m_modelListMutex);
		models = m_models;
	}

	for (Models::iterator it = models.begin(); it != models.end(); ++it){
		(*it)->DetachObserver(this);
	}
}


// reimplemented (imod::IObserver)

bool CModelUpdateBridge::IsModelAttached(const imod::IModel* modelPtr) const
{
	QReadLocker lock(&m_modelListMutex);

	return IsAttached(modelPtr);
}


bool CModelUpdateBridge::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	Q_ASSERT(modelPtr != NULL);

	QWriteLocker lock(&m_modelListMutex);

	if (!IsAttached(modelPtr)){
		m_models.push_back(modelPtr);

		changeMask = istd::IChangeable::GetAllChanges();

		return true;
	}

	return false;
}


bool CModelUpdateBridge::OnModelDetached(IModel* modelPtr)
{
	QWriteLocker lock(&m_modelListMutex);

	// If BeginChanges was called for this model but EndChanges hasn't been called yet,
	// we must call EndChanges to maintain the symmetry on the target changeable.
	bool wasPending = m_pendingModels.remove(modelPtr);

	Models::iterator iter = std::find(m_models.begin(), m_models.end(), modelPtr);
	if (iter != m_models.end()){
		m_models.erase(iter);

		if (wasPending && m_changeablePtr != nullptr){
			istd::IChangeable::ChangeSet changes = CreateEndChangeSet(istd::IChangeable::GetNoChanges());

			lock.unlock();

			m_changeablePtr->EndChanges(changes);
		}

		return true;
	}

	return false;
}


void CModelUpdateBridge::BeforeUpdate(IModel* modelPtr)
{
	QWriteLocker lock(&m_modelListMutex);

	if (m_changeablePtr == nullptr){
		return;
	}

	if (IsAttached(modelPtr)){
		istd::IChangeable::ChangeSet changeSet = CreateBeginChangeSet();

		m_pendingModels.insert(modelPtr);

		lock.unlock();

		m_changeablePtr->BeginChanges(changeSet);
	}
}


void CModelUpdateBridge::AfterUpdate(IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet)
{
	QWriteLocker lock(&m_modelListMutex);

	if (m_changeablePtr == nullptr){
		m_pendingModels.remove(modelPtr);
		return;
	}

	if (m_pendingModels.remove(modelPtr)){
		istd::IChangeable::ChangeSet changes = CreateEndChangeSet(changeSet);

		lock.unlock();

		m_changeablePtr->EndChanges(changes);
	}
}


// private methods

bool CModelUpdateBridge::IsAttached(const imod::IModel* modelPtr) const
{
	if (modelPtr != NULL){
		Models::const_iterator foundIter = std::find(m_models.begin(), m_models.end(), modelPtr);
		return (foundIter != m_models.end());
	}

	return !m_models.isEmpty();
}


istd::IChangeable::ChangeSet CModelUpdateBridge::CreateBeginChangeSet() const
{
	if (m_updateFlags & UF_DELEGATED){
		return istd::IChangeable::GetDelegatedChanges();
	}

	return istd::IChangeable::GetAnyChange();
}


istd::IChangeable::ChangeSet CModelUpdateBridge::CreateEndChangeSet(const istd::IChangeable::ChangeSet& sourceChangeSet) const
{
	istd::IChangeable::ChangeSet changes(sourceChangeSet.GetDescription());

	if (m_updateFlags & UF_DELEGATED){
		changes += istd::IChangeable::GetDelegatedChanges();
	}

	if (m_updateFlags & UF_SOURCE){
		changes += sourceChangeSet;
	}

	return changes;
}


} // namespace imod


