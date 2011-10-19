#ifndef icomp_IRealComponentStaticInfo_included
#define icomp_IRealComponentStaticInfo_included


// ACF includes
#include "istd/CClassInfo.h"
#include "icomp/IComponentStaticInfo.h"


namespace icomp
{


/**
	Interface adding to component static info functionality existing only for real components.
	Real components are components which can be created in memory.
*/
class IRealComponentStaticInfo: virtual public IComponentStaticInfo
{
public:
	/**
		Create component instance.
		\return				pointer to created component or NULL if this component cannot be created.
	 */
	virtual IComponent* CreateComponent() const = 0;

	/**
		Extract instance implemented specified interface from some component.
	*/
	virtual void* GetComponentInterface(const istd::CClassInfo& interfaceType, IComponent& component) const = 0;
};


}//namespace icomp


#endif // !icomp_IRealComponentStaticInfo_included


