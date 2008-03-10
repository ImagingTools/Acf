#ifndef icomp_CCompositeComponent_included
#define icomp_CCompositeComponent_included


#include "icomp/IComponent.h"
#include "icomp/CCompositeComponentContext.h"


namespace icomp
{


class CCompositeComponent: virtual public IComponent
{
public:
	CCompositeComponent(const CCompositeComponentContext* contextPtr);

	// reimplemented (icomp::IComponent)
	virtual void* GetInterface(const type_info& interfaceType, const ::std::string& subId = "");
	virtual const IComponentContext& GetComponentContext() const;

protected:
	/**
		Split complete component ID to real component ID and subcomponent ID.
	*/
	static void SplitComponentId(const ::std::string& fullId, ::std::string& componentId, ::std::string& restId);

private:
	const CCompositeComponentContext& m_context;
};


// inline methods

inline const IComponentContext& CCompositeComponent::GetComponentContext() const
{
	return m_context;
}


}//namespace icomp


#endif // !icomp_CCompositeComponent_included


