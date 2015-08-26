#pragma once


// ACF includes
#include "istd/IPolymorphic.h"


namespace iprm
{


class IParamsSet;


/**
	Interface for consistency checking of a parameter set.
*/
class IParamsSetValidator: virtual public istd::IPolymorphic
{
public:
	typedef QSet<QByteArray> Ids;

	/**
		Get list of parameter type IDs which can be checked by the validator.
	*/
	virtual Ids	GetSupportedTypeIds() const = 0;

	/**
		Return \c true if the parameter set is consistent or \c false otherwise.
	*/
	virtual bool IsParamsSetConsistent(const IParamsSet& paramsSet) const = 0;
};


} // namespace iprm


