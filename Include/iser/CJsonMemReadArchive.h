#pragma once


// ACF includes
#include <iser/CJsonReadArchiveBase.h>


namespace iser
{


/**
	Implementation of an ACF archive deserializing from a JSON string

	\note	As to simplify decoding of some more complicated data structures support
			for special annotation tags was added. They are used for guiding the 
			deserialization algorithm in some special cases.
*/
class CJsonMemReadArchive : public iser::CJsonReadArchiveBase
{
public:
	typedef CTextReadArchiveBase BaseClass;

	CJsonMemReadArchive(const QByteArray& inputString);
};


} // namespace iser


