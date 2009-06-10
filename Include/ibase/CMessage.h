#ifndef ibase_CMessage_included
#define ibase_CMessage_included


#include "istd/ILogger.h"
#include "istd/CString.h"

#include "isys/CSimpleDateTime.h"

#include "ibase/IMessage.h"


namespace ibase
{


class CMessage: virtual public ibase::IMessage
{
public:
	CMessage();
	CMessage(	istd::ILogger::MessageCategory category,
				int id,
				const istd::CString& text,
				const istd::CString& source,
				int flags = 0);

	virtual void SetCategory(istd::ILogger::MessageCategory category);
	virtual void SetText(const istd::CString& text);
	virtual void SetSource(const istd::CString& source);

	// reimplemented (ibase::IMessage)
	virtual const isys::IDateTime& GetTimeStamp() const;
	virtual istd::ILogger::MessageCategory GetCategory() const;
	virtual int GetId() const;
	virtual istd::CString GetText() const;
	virtual istd::CString GetSource() const;
	virtual int GetFlags() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	istd::ILogger::MessageCategory m_category;
	int m_id;
	istd::CString m_text;
	istd::CString m_source;
	int m_flags;

	isys::CSimpleDateTime m_time;
};


// inline methods

// reimplemented (ibase::IMessage)

inline const isys::IDateTime& CMessage::GetTimeStamp() const
{
	return m_time;
}


inline istd::ILogger::MessageCategory CMessage::GetCategory() const
{
	return m_category;
}


inline int CMessage::GetId() const
{
	return m_id;
}


inline istd::CString CMessage::GetText() const
{
	return m_text;
}


inline istd::CString CMessage::GetSource() const
{
	return m_source;
}


inline int CMessage::GetFlags() const
{
	return m_flags;
}


} // namespace ibase


#endif // !ibase_CMessage_included
