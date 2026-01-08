#pragma once


// Qt includes
#include <QtCore/QList>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <istd/CClassInfo.h>
#include <imod/TModelWrap.h>
#include <iser/IObject.h>
#include <ilog/CMessage.h>


namespace ilog
{


/**
	Extended message supporting attached list of serializable objects.
	
	CExtMessage extends CMessage to allow attachment of custom objects that provide
	additional context or data related to the message. Each attached object can have
	an associated description.
	
	This is particularly useful for error messages where you want to include the
	problematic data, validation results, or other context objects for detailed
	analysis or debugging.
	
	The class uses an object factory for deserialization, allowing attached objects
	to be properly reconstructed when loading from archives.
	
	\ingroup Logging
	
	\par Attached Objects
	Attached objects must implement iser::IObject interface for serialization.
	The message takes ownership of attached objects and manages their lifetime.
	
	\par Thread Safety
	Like CMessage, CExtMessage objects should be treated as immutable after creation
	and attachment of objects. Modifications are not thread-safe.
	
	\par Usage Example
	\code{.cpp}
	// Create extended message
	istd::TSharedInterfacePtr<ilog::CExtMessage> msg(
	    new ilog::CExtMessage(
	        istd::IInformationProvider::IC_ERROR,
	        3001,
	        "Data validation failed",
	        "Validator",
	        0,
	        nullptr,
	        myObjectFactory
	    )
	);
	
	// Attach invalid data object
	istd::TSharedInterfacePtr<MyDataObject> dataObj(new MyDataObject);
	dataObj->SetValue(invalidValue);
	msg->InsertAttachedObject(dataObj.PopPtr(), "Invalid data sample");
	
	// Attach validation context
	istd::TSharedInterfacePtr<ValidationContext> context(new ValidationContext);
	msg->InsertAttachedObject(context.PopPtr(), "Validation context");
	
	// Send to logger
	logger->AddMessage(msg);
	
	// Later, retrieve attached objects
	int count = msg->GetAttachedObjectsCount();
	for (int i = 0; i < count; ++i) {
	    const iser::IObject* obj = msg->GetAttachedObject(i);
	    const QString& desc = msg->GetAttachedObjectDescription(i);
	    // Analyze object...
	}
	\endcode
	
	\see ilog::CMessage, ilog::TExtMessage, iser::IObject, iser::IObjectFactory
*/
class CExtMessage: public CMessage
{
public:
	/// Base class typedef
	typedef CMessage BaseClass;

	/**
		Default constructor.
		
		Creates an empty extended message with no object factory.
		
		\param	factoryPtr	Optional object factory for deserialization of attached objects
	*/
	explicit CExtMessage(const iser::IObjectFactory* factoryPtr = NULL);
	
	/**
		Construct extended message with all properties.
		
		\param	category	Message severity level
		\param	id			Numeric message identifier
		\param	text		Human-readable message description
		\param	source		Message origin
		\param	flags		Optional application-defined flags (default: 0)
		\param	timeStampPtr Optional pointer to timestamp, or NULL for current time
		\param	factoryPtr	Optional object factory for deserializing attached objects
		
		\par Example
		\code{.cpp}
		ilog::CExtMessage msg(
		    istd::IInformationProvider::IC_ERROR,
		    5001,
		    "Processing failed",
		    "Processor",
		    0,
		    nullptr,
		    GetObjectFactory()
		);
		\endcode
	*/
	CExtMessage(istd::IInformationProvider::InformationCategory category,
				int id,
				const QString& text,
				const QString& source,
				int flags = 0,
				const QDateTime* timeStampPtr = NULL,
				const iser::IObjectFactory* factoryPtr = NULL);

	/**
		Get the object factory used for deserialization.
		
		\return Pointer to object factory, or NULL if none set
	*/
	const iser::IObjectFactory* GetObjectFactory() const;
	
	/**
		Set the object factory for deserialization.
		
		\param	factoryPtr	Object factory to use when deserializing attached objects
	*/
	void SetObjectFactory(const iser::IObjectFactory* factoryPtr);

	/**
		Remove all attached objects from the message.
		
		Clears the attached objects list, releasing all owned objects.
		
		\par Example
		\code{.cpp}
		msg->ResetAttachedObjects();
		// msg now has no attached objects
		\endcode
	*/
	void ResetAttachedObjects();
	
	/**
		Get the number of attached objects.
		
		\return Count of objects currently attached to this message
		
		\par Example
		\code{.cpp}
		int count = msg->GetAttachedObjectsCount();
		for (int i = 0; i < count; ++i) {
		    // Process object i
		}
		\endcode
		
		\see GetAttachedObject()
	*/
	int GetAttachedObjectsCount() const;
	
	/**
		Get a specific attached object by index.
		
		\param	index	Zero-based index of object to retrieve
		\return Pointer to the attached object, or NULL if index invalid
		
		\note The returned pointer is owned by the message. Don't delete it.
		
		\par Example
		\code{.cpp}
		const iser::IObject* obj = msg->GetAttachedObject(0);
		if (obj) {
		    // Try to cast to expected type
		    const MyDataObject* dataObj = 
		        dynamic_cast<const MyDataObject*>(obj);
		    if (dataObj) {
		        // Use dataObj
		    }
		}
		\endcode
		
		\see GetAttachedObjectsCount(), GetAttachedObjectDescription()
	*/
	const iser::IObject* GetAttachedObject(int index) const;
	
	/**
		Get the description of an attached object.
		
		\param	index	Zero-based index of object
		\return Description string provided when object was attached
		
		\par Example
		\code{.cpp}
		QString desc = msg->GetAttachedObjectDescription(0);
		qDebug() << "Object 0:" << desc;
		\endcode
		
		\see GetAttachedObject(), InsertAttachedObject()
	*/
	const QString& GetAttachedObjectDescription(int index) const;
	
	/**
		Add an object to this message.
		
		Attaches a serializable object to the message with an optional description.
		The message takes ownership of the object and will delete it when the
		message is destroyed.
		
		\param	objectPtr	Pointer to object to attach. Message takes ownership.
		                    Must implement iser::IObject interface.
		\param	description	Optional human-readable description of the object
		
		\note After calling this method, don't delete or modify objectPtr. The
		      message owns it now.
		      
		\par Example
		\code{.cpp}
		// Create object to attach
		MyDataObject* dataObj = new MyDataObject;
		dataObj->Initialize();
		
		// Attach to message (message takes ownership)
		msg->InsertAttachedObject(dataObj, "Problematic data");
		
		// Don't use dataObj after this point
		\endcode
		
		\see GetAttachedObject(), ResetAttachedObjects()
	*/
	void InsertAttachedObject(iser::IObject* objectPtr, const QString& description = "");

	// reimplemented (iser::IObject)
	
	/**
		Get the factory identifier for serialization.
		
		\return Type identifier for object factory
		
		\see GetTypeName()
	*/
	virtual QByteArray GetFactoryId() const override;

	// reimplemented (iser::ISerializable)
	
	/**
		Serialize or deserialize the message and attached objects.
		
		Stores/loads all message properties plus all attached objects to/from the archive.
		Uses the object factory to recreate attached objects during deserialization.
		
		\param	archive	Archive for reading or writing
		\return true if serialization succeeded, false on error
		
		\see iser::IArchive, GetObjectFactory()
	*/
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (iser::IChangeable)
	
	/**
		Copy message and attached objects from another message.
		
		\param	object	Source object to copy from
		\param	mode	Compatibility mode for copying
		\return true if copy succeeded, false if incompatible
	*/
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_STRICT) override;
	
	/**
		Create a deep copy of this extended message.
		
		Creates a new message with all properties and attached objects copied.
		
		\param	mode	Compatibility mode for cloning
		\return New extended message with copied data, or NULL on error
		
		\note Caller takes ownership of returned pointer
	*/
	virtual istd::TUniqueInterfacePtr<istd::IChangeable> CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;

	/**
		Get the extended message type identifier.
		
		\return Type identifier string
	*/
	static QByteArray GetTypeName();

protected:
	/**
		Structure holding an attached object and its description.
	*/
	struct AttachedObject
	{
		/// Shared pointer to the attached object
		istd::TSharedInterfacePtr<iser::IObject> objectPtr;
		
		/// Human-readable description of the object
		QString description;
	};

protected:
	/// Object factory for deserializing attached objects
	const iser::IObjectFactory* m_factoryPtr;
	
	/// List of attached objects with descriptions
	QList<AttachedObject> m_attachedObjects;
};


} // namespace ilog




