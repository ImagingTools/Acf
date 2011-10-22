#ifndef iser_IObject_included
#define iser_IObject_included


// ACF includes
#include "iser/ISerializable.h"


namespace iser
{


/**
	Common interface for factorisable model objects.

	\ingroup Persistence
*/
class IObject: virtual public iser::ISerializable
{
public:
	virtual std::string GetFactoryId() const;
};


// public methods 

inline std::string IObject::GetFactoryId() const
{
	static std::string emptyId;

	return emptyId;
}


} // namespace iser


#endif // !iser_IObject_included

