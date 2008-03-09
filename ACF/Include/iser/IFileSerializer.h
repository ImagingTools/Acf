#ifndef iser_IFileSerializer_included
#define iser_IFileSerializer_included


#include "istd/IPolymorphic.h"


namespace iser
{


class ISerializable;


class IFileSerializer: virtual public istd::IPolymorphic
{
public:

	enum SerializationState
	{
		StateOk,
		StateAborted,
		StateFailed
	};

	/**
		This function loads data \c data from file \c fileName
		\returns serialization state. \sa SerializationState
	*/
	virtual int LoadFromFile(ISerializable& data, const istd::CString& fileName) const = 0;

	/**
		This function saves data \c data to file \c fileName
		\returns serialization state. \sa SerializationState
	*/
	virtual int SaveToFile(ISerializable& data, const istd::CString& fileName) const = 0;
};



} // namespace iser


#endif // iser_IFileSerializer_included