/********************************************************************************
**
**	Copyright (C) 2007-2017 Witold Gantzke & Kirill Lepskiy
**
**	This file is part of the ACF Toolkit.
**
**	This file may be used under the terms of the GNU Lesser
**	General Public License version 2.1 as published by the Free Software
**	Foundation and appearing in the file LicenseLGPL.txt included in the
**	packaging of this file.  Please review the following information to
**	ensure the GNU Lesser General Public License version 2.1 requirements
**	will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**	If you are unsure which license is appropriate for your use, please
**	contact us at info@imagingtools.de.
**
** 	See http://www.ilena.org or write info@imagingtools.de for further
** 	information about the ACF.
**
********************************************************************************/


#ifndef ibase_TComposedFactoryComp_included
#define ibase_TComposedFactoryComp_included


// ACF includes
#include <istd/TComposedFactory.h>
#include <icomp/CComponentBase.h>


namespace ibase
{

// TODO: check if this class should be removed from ACF project
template <class Interface>
class TComposedFactoryComp: public icomp::CComponentBase, public istd::TComposedFactory<Interface>
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef istd::TComposedFactory<Interface> BaseClass2;

	I_BEGIN_COMPONENT(TComposedFactoryComp);
		I_REGISTER_INTERFACE(FactoryInterface);
		I_ASSIGN_MULTI_0(m_slaveFactoriesCompPtr, "SlaveFactories", "Slave factories", true);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_MULTIREF(FactoryInterface, m_slaveFactoriesCompPtr);
};


// protected methods

// reimplemented (icomp::CComponentBase)

template <class Interface>
void TComposedFactoryComp<Interface>::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	int slaveFactoriesCount = m_slaveFactoriesCompPtr.GetCount();

	for (int factoryIndex = 0; factoryIndex < slaveFactoriesCount; ++factoryIndex){
		Q_ASSERT(m_slaveFactoriesCompPtr.IsValid(factoryIndex));	// isObligatory was set to true

		RegisterFactory(m_slaveFactoriesCompPtr[factoryIndex]);
	}
}


} // namespace ibase


#endif // !ibase_TComposedFactoryComp_included


