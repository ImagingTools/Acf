#ifndef istd_CClassInfo_included
#define istd_CClassInfo_included


// STL includes
#include <string>
#include <typeinfo.h>

#include "istd/IPolymorphic.h"


namespace istd
{


/**
	Represents platform independent type info and provide set of static class manipulation functions.
*/
class CClassInfo
{
public:
	CClassInfo();
	CClassInfo(const type_info& info);

	/**
		Check if this class info object is valid.
		It is valid if stored RTTI type info is set.
	*/
	bool IsValid() const;
	/**
		Get undecorated and platform undependent class name.
		This name has format "namespace::class_name", for example "iser::ISerializable".
	*/
	std::string GetName() const;

	template <class C>
	bool IsType() const;

	CClassInfo& operator=(const type_info& info);
	CClassInfo& operator=(const CClassInfo& info);
	bool operator==(const CClassInfo& info) const;
	bool operator!=(const CClassInfo& info) const;
	bool operator<(const CClassInfo& info) const;
	bool operator>(const CClassInfo& info) const;

	// static methods
	/**
		Get class name defined as template parameter.
	*/
	template <class C>
	static CClassInfo GetInfo();
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

	const type_info* m_infoPtr;
};


// inline methods

inline CClassInfo::CClassInfo()
:	m_infoPtr(NULL)
{
}


inline CClassInfo::CClassInfo(const type_info& info)
:	m_infoPtr(&info)
{
}


inline bool CClassInfo::IsValid() const
{
	return (m_infoPtr != NULL);
}


inline std::string CClassInfo::GetName() const
{
	if (m_infoPtr != NULL){
		return GetName(*m_infoPtr);
	}

	return "<undef_type>";
}


template <class C>
inline bool CClassInfo::IsType() const
{
	if (m_infoPtr != NULL){
		return *m_infoPtr == typeid(C);
	}

	return false;
}


inline CClassInfo& CClassInfo::operator=(const type_info& info)
{
	m_infoPtr = &info;

	return *this;
}


inline CClassInfo& CClassInfo::operator=(const CClassInfo& info)
{
	m_infoPtr = info.m_infoPtr;

	return *this;
}


inline bool CClassInfo::operator==(const CClassInfo& info) const
{
	if ((m_infoPtr != NULL) && (info.m_infoPtr != NULL)){
		return *m_infoPtr == *info.m_infoPtr;
	}
	else{
		return m_infoPtr == m_infoPtr;
	}
}


inline bool CClassInfo::operator!=(const CClassInfo& info) const
{
	if ((m_infoPtr != NULL) && (info.m_infoPtr != NULL)){
		return *m_infoPtr != *info.m_infoPtr;
	}
	else{
		return m_infoPtr != m_infoPtr;
	}
}


inline bool CClassInfo::operator<(const CClassInfo& info) const
{
	if ((m_infoPtr != NULL) && (info.m_infoPtr != NULL)){
		return m_infoPtr->before(*info.m_infoPtr) != 0;
	}
	else{
		return m_infoPtr < m_infoPtr;
	}
}


inline bool CClassInfo::operator>(const CClassInfo& info) const
{
	if ((m_infoPtr != NULL) && (info.m_infoPtr != NULL)){
		return m_infoPtr->before(*info.m_infoPtr) == 0;
	}
	else{
		return m_infoPtr > m_infoPtr;
	}
}


// public static methods

template <class C>
CClassInfo CClassInfo::GetInfo()
{
	return CClassInfo(typeid(C));
}


template <class C>
std::string CClassInfo::GetName()
{
	return GetUndecoratedName(typeid(C).name());
}


} // namespace istd


#endif // !istd_CClassInfo_included


