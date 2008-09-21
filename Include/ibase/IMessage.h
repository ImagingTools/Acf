#ifndef ibase_IMessage_included
#define ibase_IMessage_included


#include "ibase/ibase.h"


#include "isys/IDateTime.h"

#include "istd/CString.h"

#include "iser/ISerializable.h"


namespace ibase
{		


/**
	Common interface for an message
*/
class IMessage: virtual public iser::ISerializable
{
public:
	enum MessageCategory
	{
		MC_INFO,
		MC_WARNING,
		MC_ERROR,
		MC_CRITICAL
	};

	/**
		Additionaly message flags.
	*/
	enum MessageFlags
	{
		MF_DEBUG = 0x1,
		MF_SYSTEM = 0x2,
		MF_USER = 0x4
	};

	/**
		Get time stamp of the message.
	*/
	virtual const isys::IDateTime& GetTimeStamp() const = 0;

	/**
		Get category of the message.
		\sa MessageCategory
	*/
	virtual MessageCategory GetCategory() const = 0;

	/**
		Get binary ID of the message using to automatical identification of this message type.
	*/
	virtual int GetId() const = 0;

	/**
		Get the text of the message.
	*/
	virtual istd::CString GetText() const = 0;

	/**
		Get the source of the message.
	*/
	virtual istd::CString GetSource() const = 0;

	/**
		Get flags of the message.
		\sa MessageFlags
	*/
	virtual int GetFlags() const = 0;
};


} // namespace ibase


#endif // !ibase_IMessage_included

