// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <iser/CMemoryWriteArchive.h>
#include <iser/ISerializable.h>
#include <imod/TSingleModelObserverBase.h>
#include <idoc/IDocumentStateComparator.h>


namespace idoc
{


/**
	Implementation of IDocumentStateComparator for serializable document models.
*/
class CDocumentStateComparator:
			public imod::TSingleModelObserverBase<iser::ISerializable>,
			virtual public IDocumentStateComparator
{
public:
	typedef imod::TSingleModelObserverBase<iser::ISerializable> BaseClass;

	CDocumentStateComparator();

	// reimplemented (idoc::IDocumentStateComparator)
	virtual bool HasStoredDocumentState() const override;
	virtual bool StoreDocumentState() override;
	virtual bool RestoreDocumentState() override;
	virtual DocumentChangeFlag GetDocumentChangeFlag() const override;

	// reimplemented (imod::IObserver)
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask) override;
	virtual bool OnModelDetached(imod::IModel* modelPtr) override;

protected:
	virtual void AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet) override;

	// reimplemented (imod::TSingleModelObserverBase<iser::ISerializable>)
	virtual iser::ISerializable* CastFromModel(imod::IModel* modelPtr) const override;

private:
	bool m_hasStoredDocumentState;
	iser::CMemoryWriteArchive m_storedStateArchive;
	mutable DocumentChangeFlag m_stateChangedFlag;
	mutable bool m_isStateChangedFlagValid;
};


} // namespace idoc
