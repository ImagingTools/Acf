#ifndef ibase_TSingleFactoryComp_included
#define ibase_TSingleFactoryComp_included


#include "istd/TSingleFactory.h"

#include "icomp/CComponentBase.h"


namespace ibase
{


template <class Interface, class Implementation>
class TSingleFactoryComp:	public icomp::CComponentBase, 
							public istd::TSingleFactory<Interface, Implementation>
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef istd::TSingleFactory<Interface, Implementation> BaseClass2;

	I_BEGIN_COMPONENT(TSingleFactoryComp)
		I_REGISTER_INTERFACE(InterfaceType)
		I_ASSIGN(m_keyAttrPtr, "FactoryKey", "Factory key", false, "")
	I_END_COMPONENT

	TSingleFactoryComp();

	// reimplemented (istd::IFactoryInfo)
	IFactoryInfo::KeyList GetFactoryKeys() const;

	// reimplemented (istd::TIFactory)
	Interface* CreateInstance(const std::string& keyId) const;

private:
	I_ATTR(istd::CString, m_keyAttrPtr);
};


template <class Interface, class Implementation>
TSingleFactoryComp<Interface, Implementation>::TSingleFactoryComp()
	:BaseClass2(std::string())
{
}


// reimplemented (istd::IFactoryInfo)

template <class Interface, class Implementation>
IFactoryInfo::KeyList TSingleFactoryComp<Interface, Implementation>::GetFactoryKeys() const
{
	KeyList retVal;

	if (m_keyAttrPtr.IsValid()){
		retVal.push_back(m_keyAttrPtr->GetValue().ToString());
	}

	return retVal;
}


// reimplemented (istd::TIFactory)

template <class Interface, class Implementation>
Interface* TSingleFactoryComp<Interface, Implementation>::CreateInstance(const std::string& keyId) const
{
	if (keyId.empty() || (m_keyAttrPtr.IsValid() && keyId == m_keyAttrPtr->GetValue().ToString())){
		return new Implementation;
	}
	
	return NULL;
}


} // namespace ibase


#endif // !ibase_TSingleFactoryComp_included
