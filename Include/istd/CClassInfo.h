#ifndef istd_CClassInfo_included
#define istd_CClassInfo_included


// STL includes
#include <string>
#include <typeinfo.h>

#include "istd/IPolymorphic.h"


namespace istd
{


/**
	Provide set of static class manipulation functions.
*/
class CClassInfo
{
public:
	/**
		Get class name defined as template parameter.
	*/
	template <class C>
	static std::string GetName();
	/**
		Get class name from RTTI.
	*/
	static std::string GetName(const type_info& info);
	/**
		Get class name of polymorphic object.
	*/
	static std::string GetName(const istd::IPolymorphic& object);

	/**
		Get undecorated class name from raw name.
	*/
	static std::string GetUndecoratedName(const std::string& rawName);

private:
	static int ParseToNumber(const char* buffer, int maxLength, int& nextPosition);
};


// public static methods

template <class C>
std::string CClassInfo::GetName()
{
	return GetUndecoratedName(typeid(C).name());
}


} // namespace istd


#endif // !istd_CClassInfo_included


