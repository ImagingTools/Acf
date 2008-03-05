#ifndef icomp_TSimpleAttribute_included
#define icomp_TSimpleAttribute_included


#include "iser/IArchive.h"
#include "iser/ISerializable.h"

#include "icomp/icomp.h"


namespace icomp
{


/**
	Template implementation of single component attribute.
*/
template <typename ValueType> 
class TSimpleAttribute: virtual public iser::ISerializable
{
public:
	virtual const ValueType& GetValue() const;
	virtual void SetValue(const ValueType& value);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	ValueType m_value;
};


// public methods

template <typename ValueType> 
const ValueType& TSimpleAttribute<ValueType>::GetValue() const
{
	return m_value;
}


template <typename ValueType> 
void TSimpleAttribute<ValueType>::SetValue(const ValueType& value)
{
	m_value = value;
}


// reimplemented (ISerializable)

template <typename ValueType> 
bool TSimpleAttribute<ValueType>::Serialize(iser::IArchive& archive)
{
	bool result = true;

	static iser::CArchiveTag valueTag("Value", "Value of attribute", iser::IArchive::VT_FRAMEWORK);
	result = result && archive.BeginTag(valueTag);

	result = result && archive.Process(m_value);

	result = result && archive.EndTag(valueTag);

	return result;
}


typedef TSimpleAttribute<int> IntAttribute;
typedef TSimpleAttribute<bool> BoolAttribute;
typedef TSimpleAttribute<double> DoubleAttribute;
typedef TSimpleAttribute<CString> StringAttribute;


} // namespace icomp


#endif // icomp_TSimpleAttribute_included


