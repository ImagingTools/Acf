#ifndef iqt_CXslTransformationReadArchive_included
#define iqt_CXslTransformationReadArchive_included


// Qt includes
#include <QDomDocument>
#include <QDomNode>
#include <QXmlQuery>
#include <QAbstractMessageHandler>


// ACF includes
#include "istd/ILogger.h"

#include "iser/CXmlDocumentInfoBase.h"
#include "iser/CReadArchiveBase.h"

#include "iqt/iqt.h"
#include "istd/itr.h"


namespace iqt
{


/**
	Qt-based implementation of archive reading from XML file.

	\ingroup Persistence
*/
class CXslTransformationReadArchive: public iser::CReadArchiveBase, public iser::CXmlDocumentInfoBase
{
public:
	CXslTransformationReadArchive(
				const istd::CString& filePath = "",
				const istd::CString& xslFilePath = "",
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);

	bool OpenDocument(const istd::CString& filePath, const istd::CString& xslFilePath);

	// reimplemented (iser::IArchive)
	virtual bool IsTagSkippingSupported() const;
	virtual bool BeginTag(const iser::CArchiveTag& tag);
	virtual bool BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count);
	virtual bool EndTag(const iser::CArchiveTag& tag);
	virtual bool Process(bool& value);
	virtual bool Process(char& value);
	virtual bool Process(I_BYTE& value);
	virtual bool Process(I_SBYTE& value);
	virtual bool Process(I_WORD& value);
	virtual bool Process(I_SWORD& value);
	virtual bool Process(I_DWORD& value);
	virtual bool Process(I_SDWORD& value);
	virtual bool Process(I_QWORD& value);
	virtual bool Process(I_SQWORD& value);
	virtual bool Process(float& value);
	virtual bool Process(double& value);
	virtual bool Process(std::string& value);
	virtual bool Process(istd::CString& value);
	virtual bool ProcessData(void* dataPtr, int size);

protected:
	/**
		Find the next text node and move the current node to the next sibling.
	*/
	QString PullTextNode();

	//virtual bool SendLogMessage(
	//	MessageCategory category,
	//	int id,
	//	const istd::CString& message,
	//	const istd::CString& messageSource,
	//	int flags = 0) const
	//{
	//	return iser::CReadArchiveBase::SendLogMessage(category, id, message, messageSource, flags);
	//}

private:
	class MessageHandler:
		virtual public QAbstractMessageHandler
	{
		I_DECLARE_TR_FUNCTION(MessageHandler);
	public:
		MessageHandler(CXslTransformationReadArchive* logger);
	protected:

		void handleMessage(QtMsgType type, const QString &description, const QUrl &identifier, const QSourceLocation &sourceLocation);

		CXslTransformationReadArchive* m_loggerPtr;
	};

	friend class MessageHandler;

	QDomDocument m_document;
	QDomNode m_currentNode;

	bool m_serializeHeader;
	iser::CArchiveTag m_rootTag;
};


} // namespace iqt


#endif // !iqt_CXslTransformationReadArchive_included


