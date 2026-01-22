// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/IChangeable.h>
#include <imod/TModelWrap.h>
#include <icomp/TComponentCloneWrap.h>
#include <icomp/CComponentBase.h>


namespace icomp
{


/**
	Makes component and model from some class implementing interface \c istd::IChangeable.
	This model wrapper extends functionality of \c icomp::TModelWrap for components.

	\ingroup ComponentConcept
*/
template <class Base>
class TModelCompWrap: public icomp::TComponentCloneWrap<imod::TModelWrap<Base> >
{
public:
	typedef icomp::TComponentCloneWrap<imod::TModelWrap<Base> > BaseClass;

	I_BEGIN_COMPONENT(TModelCompWrap);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_REGISTER_INTERFACE(imod::IModel);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentDestroyed() override;
};


// protected methods

// reimplemented (icomp::CComponentBase)

template <class Base>
void TModelCompWrap<Base>::OnComponentDestroyed()
{
	imod::CModelBase::DetachAllObservers();

	BaseClass::OnComponentDestroyed();
}


} // namespace icomp


