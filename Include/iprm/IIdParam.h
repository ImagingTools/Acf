#ifndef iprm_IIdParam_included
#define iprm_IIdParam_included


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include <iser/ISerializable.h>


namespace iprm
{


/**
	\brief Interface for an ID parameter.
	
	IIdParam stores a unique identifier as a QByteArray. It's typically used for
	internal identification, serialization keys, or database IDs where a binary-safe
	identifier is needed.
	
	\section iidparam_usage Usage Example
	
	\code{.cpp}
	#include <iprm/CIdParam.h>
	
	// Create an ID parameter
	iprm::CIdParam idParam;
	
	// Set ID
	idParam.SetId("user_123");
	
	// Get ID
	QByteArray id = idParam.GetId();
	qDebug() << "ID:" << id;
	
	// IDs can contain binary data
	QByteArray binaryId = QByteArray::fromHex("deadbeef");
	idParam.SetId(binaryId);
	\endcode
	
	\section iidparam_comparison Comparison with Other Parameter Types
	
	- **IIdParam**: Uses QByteArray, suitable for binary-safe identifiers
	- **INameParam**: Uses QString, suitable for human-readable names
	- **ITextParam**: Uses QString, suitable for general text content
	
	\note Inherits from iser::ISerializable for persistence support.
	\note IDs are typically used as stable references that don't change over time.
	
	\see CIdParam, INameParam, ITextParam
*/
class IIdParam: virtual public iser::ISerializable
{
public:
	/**
		\brief Get the object ID.
		
		\return The current ID as QByteArray.
		
		\code{.cpp}
		QByteArray id = idParam.GetId();
		if (!id.isEmpty())
		{
		    qDebug() << "ID:" << id;
		}
		\endcode
		
		\see SetId
	*/
	virtual QByteArray GetId() const = 0;

	/**
		\brief Set the object ID.
		
		Changes the identifier value.
		
		\param id New ID value. Can be empty or contain binary data.
		
		\code{.cpp}
		// Set text-based ID
		idParam.SetId("entity_42");
		
		// Set binary ID
		QByteArray binaryId = QByteArray::fromHex("1234abcd");
		idParam.SetId(binaryId);
		
		// Clear ID
		idParam.SetId(QByteArray());
		\endcode
		
		\see GetId
	*/
	virtual void SetId(const QByteArray& id) = 0;
};


} // namespace iprm


#endif // !iprm_IIdParam_included


