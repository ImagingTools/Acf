#ifndef iser_IFileSerializer_included
#define iser_IFileSerializer_included


#include "istd/IPolymorphic.h"


namespace iser
{


class ISerializable;


class IFileSerializer: public acf::IPolymorphic
{
public:
	/**
		This function loads data \c data from file \c fileName
	*/
	virtual bool LoadFromFile(ISerializable& data, const istd::CString& fileName) = 0;

	/**
		This function saves data \c data to file \c fileName
	*/
	virtual bool SaveToFile(ISerializable& data, const istd::CString& fileName) = 0;
};



} // namespace iser


#endif // iser_IFileSerializer_included