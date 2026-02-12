// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/CChangeNotifier.h>
#include <istd/CClassInfo.h>
#include <imod/TModelWrap.h>
#include <ilog/CMessage.h>


namespace ilog
{


/**
	Template for creating extended messages with custom embedded data.
	
	TExtMessage combines CMessage with a user-defined Element type to create
	specialized message types that carry both standard message properties and
	custom structured data in a single object.
	
	This is an alternative to CExtMessage's attached objects approach. Instead of
	attaching separate objects, the custom data is embedded directly in the message
	type through multiple inheritance.
	
	The Element type must implement iser::ISerializable and istd::IChangeable for
	proper serialization and cloning support.
	
	\tparam Element	User-defined class to embed in the message. Must implement
	                iser::ISerializable and istd::IChangeable interfaces.
	
	\ingroup Logging
	
	\par Advantages over CExtMessage
	- Type-safe access to custom data (no casting needed)
	- More efficient (no indirection through attached object list)
	- Simpler usage for single custom data type
	- Direct member access to Element's properties
	
	\par Usage Example
	\code{.cpp}
	// Define custom data structure
	struct DiagnosticData : public iser::ISerializable, public istd::IChangeable
	{
	    double cpuUsage;
	    qint64 memoryUsed;
	    QString componentName;
	    
	    virtual bool Serialize(iser::IArchive& archive) override {
	        return archive.Process(cpuUsage, "cpu") &&
	               archive.Process(memoryUsed, "memory") &&
	               archive.Process(componentName, "component");
	    }
	    
	    virtual bool CopyFrom(const istd::IChangeable& obj, CompatibilityMode mode) {
	        const DiagnosticData* other = dynamic_cast<const DiagnosticData*>(&obj);
	        if (!other) return false;
	        cpuUsage = other->cpuUsage;
	        memoryUsed = other->memoryUsed;
	        componentName = other->componentName;
	        return true;
	    }
	};
	
	// Create message type
	typedef ilog::TExtMessage<DiagnosticData> DiagnosticMessage;
	
	// Register for serialization
	I_REGISTER_MESSAGE_TYPE(DiagnosticMessage, "com.myapp.DiagnosticMessage");
	
	// Use the message
	istd::TSharedInterfacePtr<DiagnosticMessage> msg(
	    new DiagnosticMessage(
	        istd::IInformationProvider::IC_INFO,
	        1001,
	        "System diagnostics",
	        "Monitor"
	    )
	);
	
	// Set custom data directly
	msg->cpuUsage = 65.5;
	msg->memoryUsed = 1024*1024*800; // 800 MB
	msg->componentName = "MainProcessor";
	
	// Send to logger
	logger->AddMessage(msg);
	\endcode
	
	\see ilog::CExtMessage, ilog::CMessage, ilog::TExtMessageModel
*/
template<class Element>
class TExtMessage:
			public CMessage,
			public Element
{
public:
	/// Base message class typedef
	typedef CMessage BaseClass;
	
	/// Base element class typedef
	typedef Element BaseClass2;

	/**
		Default constructor.
		
		Initializes both the message and element parts with default values.
	*/
	TExtMessage();
	
	/**
		Construct message with all standard properties.
		
		Creates a message with the specified properties. The Element part
		is default-constructed and can be set afterwards.
		
		\param	category	Message severity level
		\param	id			Numeric message identifier
		\param	text		Human-readable message description
		\param	source		Message origin
		\param	flags		Optional application-defined flags (default: 0)
		\param	timeStampPtr Optional pointer to timestamp, or NULL for current time
		
		\par Example
		\code{.cpp}
		TExtMessage<DiagnosticData> msg(
		    istd::IInformationProvider::IC_INFO,
		    2001,
		    "Performance metrics",
		    "Monitor"
		);
		msg.cpuUsage = 45.2;
		msg.memoryUsed = 512*1024*1024;
		\endcode
	*/
	TExtMessage(istd::IInformationProvider::InformationCategory category,
				int id,
				const QString& text,
				const QString& source,
				int flags = 0,
				const QDateTime* timeStampPtr = NULL);

	// reimplemented (iser::IObject)
	
	/**
		Get the factory identifier for serialization.
		
		\return Type identifier combining message and element types
		
		\see GetTypeName()
	*/
	virtual QByteArray GetFactoryId() const override;

	// reimplemented (iser::ISerializable)
	
	/**
		Serialize or deserialize the message and element data.
		
		Stores/loads both the base message properties and the Element data
		to/from the archive. The Element part is serialized in a tagged section.
		
		\param	archive	Archive for reading or writing
		\return true if serialization succeeded, false on error
		
		\note Handles version compatibility for old ACF formats (version < 4279)
		
		\see iser::IArchive
	*/
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (iser::IChangeable)
	
	/**
		Copy message and element data from another object.
		
		Copies both message properties and element data from a compatible object.
		
		\param	object	Source object to copy from
		\param	mode	Compatibility mode for copying
		\return true if copy succeeded, false if incompatible
	*/
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_STRICT) override;
	
	/**
		Create a deep copy of this extended message.
		
		Creates a new message with both message and element data copied.
		
		\param	mode	Compatibility mode for cloning
		\return New extended message with copied data, or NULL on error
		
		\note Caller takes ownership of returned pointer
	*/
	virtual istd::TUniqueInterfacePtr<istd::IChangeable> CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;

	// static methods
	
	/**
		Get the extended message type identifier.
		
		Returns a unique type name combining the template parameter's type.
		
		\return Type identifier string
	*/
	static QByteArray GetTypeName();
};


// public methods

template<class Element>
TExtMessage<Element>::TExtMessage()
:	BaseClass(),
	Element()
{
}


template<class Element>
TExtMessage<Element>::TExtMessage(
			istd::IInformationProvider::InformationCategory category,
			int id,
			const QString& text,
			const QString& source,
			int flags,
			const QDateTime* timeStampPtr)
:	CMessage(category, id, text, source, flags, timeStampPtr),
	Element()
{
}


// reimplemented (iser::IObject)

template<class Element>
QByteArray TExtMessage<Element>::GetFactoryId() const
{
	return GetTypeName();
}


// reimplemented (iser::ISerializable)

template<class Element>
bool TExtMessage<Element>::Serialize(iser::IArchive& archive)
{
	static const iser::CArchiveTag elementTag("Element", "Element part", iser::CArchiveTag::TT_GROUP);

	quint32 versionNumber = 0;
	bool isOldFormat = archive.GetVersionInfo().GetVersionNumber(iser::IVersionInfo::AcfVersionId, versionNumber) && (versionNumber < 4279);

	bool retVal = BaseClass::Serialize(archive);

	if (!isOldFormat){
		retVal = retVal && archive.BeginTag(elementTag);
	}
	retVal = retVal && BaseClass2::Serialize(archive);
	if (!isOldFormat){
		retVal = retVal && archive.EndTag(elementTag);
	}

	return retVal;
}


// reimplemented (iser::IChangeable)

template<class Element>
bool TExtMessage<Element>::CopyFrom(const istd::IChangeable& object, CompatibilityMode mode)
{
	istd::CChangeNotifier notifier(this);

	return BaseClass::CopyFrom(object, mode) && BaseClass2::CopyFrom(object, mode);
}


template<class Element>
istd::IChangeable* TExtMessage<Element>::CloneMe(CompatibilityMode mode) const
{
	istd::TDelPtr<TExtMessage<Element> > clonedPtr(new TExtMessage<Element>);
	if (clonedPtr->CopyFrom(*this, mode)){
		return clonedPtr.PopRootPtr();
	}

	return NULL;
}


// static methods

template<class Element>
QByteArray TExtMessage<Element>::GetTypeName()
{
	return istd::CClassInfo::GetName<TExtMessage<Element> >();
}


/**
	Model wrapper for TExtMessage.
	
	TExtMessageModel is a convenience wrapper that combines TExtMessage with
	imod::TModelWrap to create messages that automatically integrate with
	the ACF model-observer pattern.
	
	This allows messages to participate in data binding and automatic UI updates
	when used with model-aware components.
	
	\tparam Element	User-defined class to embed in the message. Must implement
	                iser::ISerializable and istd::IChangeable interfaces.
	
	\ingroup Logging
	
	\par Usage Example
	\code{.cpp}
	// Create model-aware diagnostic message
	typedef ilog::TExtMessageModel<DiagnosticData> DiagnosticMessageModel;
	
	istd::TSharedInterfacePtr<DiagnosticMessageModel> msg(
	    new DiagnosticMessageModel(
	        istd::IInformationProvider::IC_INFO,
	        3001,
	        "System metrics",
	        "Monitor"
	    )
	);
	
	// Set data
	msg->cpuUsage = 55.0;
	
	// Observers will be notified of changes
	msg->NotifyChange();
	\endcode
	
	\see ilog::TExtMessage, imod::TModelWrap
*/
template<class Element>
class TExtMessageModel: public imod::TModelWrap<TExtMessage<Element> >
{
public:
	/// Base class typedef
	typedef imod::TModelWrap<TExtMessage<Element> > BaseClass;

	/**
		Construct message model with all properties.
		
		\param	category	Message severity level
		\param	id			Numeric message identifier
		\param	text		Human-readable message description
		\param	source		Message origin
		\param	flags		Optional application-defined flags (default: 0)
		\param	timeStampPtr Optional pointer to timestamp, or NULL for current time
	*/
	TExtMessageModel(
				istd::IInformationProvider::InformationCategory category,
				int id,
				const QString& text,
				const QString& source,
				int flags = 0,
				const QDateTime* timeStampPtr = NULL);
};


template<class Element>
TExtMessageModel<Element>::TExtMessageModel(
				istd::IInformationProvider::InformationCategory category,
				int id,
				const QString& text,
				const QString& source,
				int flags,
				const QDateTime* timeStampPtr)
{
	BaseClass::SetMessageValues(category, id, text, source, flags, timeStampPtr);
}


} // namespace ilog




