#ifndef istdc_TComposedFactoryComp_included
#define istdc_TComposedFactoryComp_included


#include "istd/TComposedFactory.h"

#include "icomp/CComponentBase.h"


namespace istdc
{


template <class InterfaceClass>
class TComposedFactoryComp: public icomp::CComponentBase, public istd::TComposedFactory<InterfaceClass>
{
public:
	typedef istd::TIFactory<InterfaceClass> FactoryInterface;
	typedef TComposedFactoryComp<InterfaceClass> ComponentClass;
	typedef icomp::Component BaseClass;

	I_BEGIN_COMPONENT(ComponentClass)
		I_REGISTER_INTERFACE(FactoryInterface)
		I_ASSIGN_MULTI_0(m_slaveFactoriesCompPtr, "SlaveFactories", "Slave factories", false)
	I_END_COMPONENT

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

private:
	I_MULTIREF(FactoryInterface> m_slaveFactoriesCompPtr);
};


// reimplemented (icomp::IComponent)

template <class InterfaceClass>
void TComposedFactoryComp<InterfaceClass>::OnComponentCreated()
{
	if (m_slaveFactoriesCompPtr.IsValid(factoryIndex)){
		int salveFactoryCount = m_slaveFactoriesCompPtr.GetCount();

		for (int factoryIndex = 0; factoryIndex < salveFactoryCount; factoryIndex++){
			RegisterFactory(m_slaveFactoriesCompPtr[factoryIndex]);
		}
	}
}



} // namespace istdc


#endif // !istdc_TComposedFactoryComp_included
