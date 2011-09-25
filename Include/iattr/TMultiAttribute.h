#ifndef iattr_TMultiAttribute_included
#define iattr_TMultiAttribute_included


// STL includes
#include <vector>


// ACF includes
#include "istd/CClassInfo.h"

#include "iser/IArchive.h"
#include "iser/IObject.h"
#include "iser/CArchiveTag.h"

#include "iattr/CAttributeBase.h"


namespace iattr
{


/**
	Template implementation of multiple attribute.
	\internal
*/
template <typename Value>
class TMultiAttribute: public iattr::CAttributeBase
{
public:
	typedef Value ValueType;
	typedef iattr::CAttributeBase BaseClass;
	typedef std::vector<ValueType> ValueList;
	typedef typename ValueList::iterator iterator;
	typedef typename ValueList::const_iterator const_iterator;

	/**
		Default constructor.
	*/
	TMultiAttribute();

	/**
		Constructor.
		\param	attributeOwnerPtr	Owner of the attrbiute.
		\param	attributeId		Unique ID of the attrbiute.
		\param	attributeId		Attribute description.
		\param	changeFlags		Change flags for model update notification.
		\param	elementsCount	Number of elements stored at \c valuesPtr.
		\param	valuesPtr		Pointer to array of elements.
	*/
	TMultiAttribute(
				iattr::IAttributesManager* attributeOwnerPtr,
				const std::string& attributeId,
				const std::string& attributeDescription,
				int attributeFlags,
				int changeFlags = 0,
				int elementsCount = 0,
				Value* valuesPtr = NULL);

	void SetValues(const ValueList& valueList);

	/**
		Set value list to the attribute.
	*/
	template <class ContainerImpl>
	void SetValues(typename ContainerImpl::const_iterator begin, typename ContainerImpl::const_iterator end);

	/**
		Get all values.
	*/
	const ValueList& GetValues() const;

	/**
		Get number of values in the multi attribute.
	*/
	virtual int GetValuesCount() const;

	/**
		Get value at the index \c index
	*/
	virtual Value GetValueAt(int index) const;

	/**
		Set value at the index \c index to new value \c value.
	*/
	virtual void SetValueAt(int index, const Value& value);

	/**
		Insert new value into container.
	*/
	virtual void InsertValue(const Value& value);

	/**
		Remove all values from the container.
	*/
	virtual void ResetValues();

	// STL compatibility
	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;

	// reimplemented (iser::IObject)
	const std::string& GetFactoryId() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// static methods
	static const std::string& GetTypeName();

protected:
	std::vector<Value> m_values;

private:
	static const std::string s_typeName;
};


// public methods

template <typename Value>
TMultiAttribute<Value>::TMultiAttribute()
{
}


template <typename Value>
TMultiAttribute<Value>::TMultiAttribute(
				iattr::IAttributesManager* attributeOwnerPtr,
				const std::string& attributeId,
				const std::string& attributeDescription,
				int attributeFlags,
				int changeFlags,
				int elementsCount,
				Value* valuesPtr)
	:BaseClass(attributeOwnerPtr, attributeId, attributeDescription, attributeFlags, changeFlags)
{
	for (int i = 0; i < elementsCount; ++i){
		m_values.push_back(valuesPtr[i]);
	}
}


template <typename Value>
void TMultiAttribute<Value>::SetValues(const ValueList& valueList)
{
	SetValues<ValueList>(valueList.begin(), valueList.end());
}


template <typename Value>
template <class ContainerImpl>
void TMultiAttribute<Value>::SetValues(typename ContainerImpl::const_iterator beginIter, typename ContainerImpl::const_iterator endIter)
{
	istd::CChangeNotifier changePtr(m_attributeOwnerPtr, m_changeFlags);

	ResetValues();

	while (beginIter != endIter){
		InsertValue(*beginIter);

		++beginIter;
	}
}


template <typename Value>
const typename TMultiAttribute<Value>::ValueList& TMultiAttribute<Value>::GetValues() const
{
	return m_values;
}


template <typename Value>
int TMultiAttribute<Value>::GetValuesCount() const
{
	return int(m_values.size());
}


template <typename Value>
Value TMultiAttribute<Value>::GetValueAt(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < GetValuesCount());

	return m_values[index];
}


template <typename Value>
void TMultiAttribute<Value>::SetValueAt(int index, const Value& value)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < GetValuesCount());

	if (m_values[index] != value){
		istd::CChangeNotifier changePtr(m_attributeOwnerPtr, m_changeFlags);

		m_values[index] = value;
	}
}


template <typename Value>
void TMultiAttribute<Value>::InsertValue(const Value& value)
{
	m_values.push_back(value);
}


template <typename Value>
void TMultiAttribute<Value>::ResetValues()
{
	istd::CChangeNotifier changePtr(m_attributeOwnerPtr, m_changeFlags);
	
	m_values.clear();
}


// STL compatibility

template <typename Value>
typename TMultiAttribute<Value>::iterator TMultiAttribute<Value>::begin()
{
	return m_values.begin();
}


template <typename Value>
typename TMultiAttribute<Value>::const_iterator TMultiAttribute<Value>::begin() const
{
	return m_values.begin();
}


template <typename Value>
typename TMultiAttribute<Value>::iterator TMultiAttribute<Value>::end()
{
	return m_values.end();
}
	

template <typename Value>
typename TMultiAttribute<Value>::const_iterator TMultiAttribute<Value>::end() const
{
	return m_values.end();
}


// reimplemented (iser::IObject)

template <typename Value>
const std::string& TMultiAttribute<Value>::GetFactoryId() const
{
	return s_typeName;
}


// reimplemented (ISerializable)

template <typename Value>
bool TMultiAttribute<Value>::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	bool isStoring = archive.IsStoring();

	static iser::CArchiveTag valuesTag("Values", "List of attribute values");
	static iser::CArchiveTag valueTag("Value", "Single Value");

	int valuesCount = 0;

	if (isStoring){
		valuesCount = int(m_values.size());
	}

	retVal = retVal && archive.BeginMultiTag(valuesTag, valueTag, valuesCount);

	if (!isStoring){
		if (!retVal){
			return false;
		}

		m_values.resize(valuesCount);
	}

	for (int i = 0; i < valuesCount; ++i){
		retVal = retVal && archive.BeginTag(valueTag);
		Value value = m_values[i];
		retVal = retVal && archive.Process(value);
		m_values[i] = value;
		retVal = retVal && archive.EndTag(valueTag);
	}

	retVal = retVal && archive.EndTag(valuesTag);

	return retVal;
}


// static methods

template <typename Value>
const std::string& TMultiAttribute<Value>::GetTypeName()
{
	return s_typeName;
}


// private static attributes

template <typename Value>
const std::string TMultiAttribute<Value>::s_typeName(istd::CClassInfo::GetName<TMultiAttribute<Value> >());


// typedefs

typedef TMultiAttribute<double> CMultiDoubleAttribute;
typedef TMultiAttribute<bool> CMultiBoolAttribute;
typedef TMultiAttribute<int> CMultiIntAttribute;
typedef TMultiAttribute<istd::CString> CMultiStringAttribute;


} // namespace iattr


#endif // !iattr_TMultiAttribute_included


