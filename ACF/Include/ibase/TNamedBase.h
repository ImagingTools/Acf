#ifndef istd_TNamedBase_included
#define istd_TNamedBase_included


#include "ibase/ibase.h"


#include "istd/INamed.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace ibase
{


/**
	Template based implementation of a named object. 
	This class is a pseudo-implementation of istd::INamed interface.
*/
template <class BaseClass>
class TNamedBase: virtual public BaseClass
{
public:
	// peudeo-reimplemented (istd::INamed)
	virtual const istd::CString& GetName() const;
	virtual void SetName(const istd::CString& name);

protected:
	virtual bool SerializeName(iser::IArchive& archive);

protected:
	istd::CString m_name;
};



// peudeo-reimplemented (istd::INamed)

template <class BaseClass>
const istd::CString& TNamedBase<BaseClass>::name() const
{
	return m_name;
}


template <class BaseClass>
void TNamedBase<BaseClass>::setName(const istd::CString& newName)
{
	m_name = newName;
}


// protected members

template <class BaseClass>
bool TNamedBase<BaseClass>::SerializeName(iser::IArchive& archive)
{
	static iser::CArchiveTag nameTag("Name", "Object name");

	bool retVal = archive.BeginTag(nameTag);
	retVal = retVal && archive.Process(m_name);
	retVal = retVal && archive.EndTag(nameTag);

	return retVal;
}


} // namespace ibase


#endif // ibase_TNamedBase_included
