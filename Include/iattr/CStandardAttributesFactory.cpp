// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iattr/CStandardAttributesFactory.h>


// ACF includes
#include <iattr/TAttribute.h>
#include <iattr/TMultiAttribute.h>


namespace iattr
{


static QByteArray integerAttrTypeName = CIntegerAttribute::GetTypeName();
static QByteArray realAttrTypeName = CRealAttribute::GetTypeName();
static QByteArray booleanAttrTypeName = CBooleanAttribute::GetTypeName();
static QByteArray stringAttrTypeName = CStringAttribute::GetTypeName();
static QByteArray idAttrTypeName = CIdAttribute::GetTypeName();

static QByteArray boolListAttrTypeName = CBooleanListAttribute::GetTypeName();
static QByteArray realListAttrTypeName = CRealListAttribute::GetTypeName();
static QByteArray integerListAttrTypeName = CIntegerListAttribute::GetTypeName();
static QByteArray stringListAttrTypeName = CStringListAttribute::GetTypeName();
static QByteArray idListAttrTypeName = CIdListAttribute::GetTypeName();


// reimplemented (istd::TIFactory<iser::IObject>)

istd::TUniqueInterfacePtr<iser::IObject> CStandardAttributesFactory::CreateInstance(const QByteArray& keyId) const
{
	if (keyId == integerAttrTypeName){
		return istd::TUniqueInterfacePtr<iser::IObject>(new CIntegerAttribute());
	}
	else if (keyId == realAttrTypeName){
		return istd::TUniqueInterfacePtr<iser::IObject>(new CRealAttribute());
	}
	else if (keyId == booleanAttrTypeName){
		return istd::TUniqueInterfacePtr<iser::IObject>(new CBooleanAttribute());
	}
	else if (keyId == stringAttrTypeName){
		return istd::TUniqueInterfacePtr<iser::IObject>(new CStringAttribute());
	}
	else if (keyId == idAttrTypeName){
		return istd::TUniqueInterfacePtr<iser::IObject>(new CIdAttribute());
	}
	else if (keyId == integerListAttrTypeName){
		return istd::TUniqueInterfacePtr<iser::IObject>(new CIntegerListAttribute());
	}
	else if (keyId == realListAttrTypeName){
		return istd::TUniqueInterfacePtr<iser::IObject>(new CRealListAttribute());
	}
	else if (keyId == boolListAttrTypeName){
		return istd::TUniqueInterfacePtr<iser::IObject>(new CBooleanListAttribute());
	}
	else if (keyId == stringListAttrTypeName){
		return istd::TUniqueInterfacePtr<iser::IObject>(new CStringListAttribute());
	}
	else if (keyId == idListAttrTypeName){
		return istd::TUniqueInterfacePtr<iser::IObject>(new CIdListAttribute());
	}
	else{
		return istd::TUniqueInterfacePtr<iser::IObject>();
	}
}


// reimplemented (istd::IFactoryInfo)

CStandardAttributesFactory::KeyList CStandardAttributesFactory::GetFactoryKeys() const
{
	return s_factoryKeys;
}


// static methods

const CStandardAttributesFactory& CStandardAttributesFactory::GetInstance()
{
	return s_instance;
}


// private static methods

CStandardAttributesFactory::KeyList CStandardAttributesFactory::GetInitialFactoryKeys()
{
	KeyList retVal;

	retVal.insert(integerAttrTypeName);
	retVal.insert(realAttrTypeName);
	retVal.insert(booleanAttrTypeName);
	retVal.insert(stringAttrTypeName);
	retVal.insert(idAttrTypeName);
	retVal.insert(integerListAttrTypeName);
	retVal.insert(realListAttrTypeName);
	retVal.insert(boolListAttrTypeName);
	retVal.insert(stringListAttrTypeName);
	retVal.insert(idListAttrTypeName);

	return retVal;
}


// static attributes
CStandardAttributesFactory::KeyList CStandardAttributesFactory::s_factoryKeys = CStandardAttributesFactory::GetInitialFactoryKeys();
CStandardAttributesFactory CStandardAttributesFactory::s_instance;


} // namespace iattr


