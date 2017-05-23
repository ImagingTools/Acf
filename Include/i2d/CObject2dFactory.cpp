#include <i2d/CObject2dFactory.h>


// ACF includes
#include <imod/TModelWrap.h>
#include <i2d/CLine2d.h>


namespace i2d
{


static QByteArray s_line2dTypeId = CLine2d::GetTypeName();


// reimplemented (istd::TIFactory<iser::IObject>)

iser::IObject* CObject2dFactory::CreateInstance(const QByteArray& keyId) const
{
	if (keyId == s_line2dTypeId){
		return new imod::TModelWrap<CLine2d>();
	}
	else{
		return NULL;
	}
}


// reimplemented (istd::IFactoryInfo)

CObject2dFactory::KeyList CObject2dFactory::GetFactoryKeys() const
{
	return s_factoryKeys;
}


// static methods

const CObject2dFactory& CObject2dFactory::GetInstance()
{
	return s_instance;
}


// private static methods

CObject2dFactory::KeyList CObject2dFactory::GetInitialFactoryKeys()
{
	KeyList retVal;

	retVal.insert(s_line2dTypeId);

	return retVal;
}


// static attributes

CObject2dFactory::KeyList CObject2dFactory::s_factoryKeys = CObject2dFactory::GetInitialFactoryKeys();

CObject2dFactory CObject2dFactory::s_instance;


} // namespace i2d


