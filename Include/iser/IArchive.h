#ifndef iser_IArchive_included
#define iser_IArchive_included


// Qt includes
#include <QtCore/QByteArray>

// Qt includes
#include <QtCore/QString>


// ACF includes
#include <istd/IPolymorphic.h>


namespace iser
{


class IVersionInfo;
class CArchiveTag;


/**
	Represents an input/output persistence archive for object serialization.
	
	\section ArchivePurpose Purpose
	IArchive provides an abstract interface for serializing and deserializing objects
	to and from various storage media. It acts as a bridge between the logical structure
	of your data and the physical representation (binary file, XML, JSON, memory buffer, etc.).
	This abstraction allows the same serialization code to work with different archive types.

	\section ArchiveTypes Archive Types
	ACF provides several archive implementations:
	- **Binary Archives**: CFileReadArchive, CFileWriteArchive (fast, compact)
	- **XML Archives**: CSimpleXmlFileReadArchive, CCompactXmlFileReadArchive
	- **JSON Archives**: CJsonFileReadArchive, CJsonFileWriteArchive
	- **Memory Archives**: CMemoryReadArchive, CMemoryWriteArchive
	- **Network Archives**: CNetworkWriteArchive (for data transmission)

	\section ArchiveConcepts Key Concepts
	
	**Tags**: Archives organize data using tags (similar to XML elements). Each tag
	has a name and describes a logical unit of data. Tags can be nested to create
	hierarchical structures.
	
	**Bidirectional**: The same Process() methods are used for both reading and writing.
	Use IsStoring() to determine the direction.
	
	**Type Safety**: Archive provides Process() overloads for common types (int, double,
	QString, etc.), ensuring type-safe serialization.

	\section ArchiveUsageExample Usage Example
	\code
	// Example serializable class
	class CPerson: public iser::ISerializable
	{
	public:
		QString m_name;
		int m_age;
		QList<QString> m_hobbies;

		// Implement Serialize method
		virtual bool Serialize(iser::IArchive& archive) override
		{
			bool result = true;
			
			// Serialize name
			static iser::CArchiveTag nameTag("Name", "Person's name");
			result = result && archive.BeginTag(nameTag);
			result = result && archive.Process(m_name);
			result = result && archive.EndTag(nameTag);
			
			// Serialize age
			static iser::CArchiveTag ageTag("Age", "Person's age");
			result = result && archive.BeginTag(ageTag);
			result = result && archive.Process(m_age);
			result = result && archive.EndTag(ageTag);
			
			// Serialize hobbies list
			static iser::CArchiveTag hobbiesTag("Hobbies", "List of hobbies");
			static iser::CArchiveTag hobbyTag("Hobby", "Single hobby");
			int count = m_hobbies.count();
			result = result && archive.BeginMultiTag(hobbiesTag, hobbyTag, count);
			
			if (archive.IsStoring()) {
				// Writing: iterate and save each hobby
				for (const QString& hobby : m_hobbies) {
					QString temp = hobby;
					result = result && archive.BeginTag(hobbyTag);
					result = result && archive.Process(temp);
					result = result && archive.EndTag(hobbyTag);
				}
			} else {
				// Reading: count is set by BeginMultiTag
				m_hobbies.clear();
				for (int i = 0; i < count; ++i) {
					QString hobby;
					result = result && archive.BeginTag(hobbyTag);
					result = result && archive.Process(hobby);
					result = result && archive.EndTag(hobbyTag);
					m_hobbies.append(hobby);
				}
			}
			
			result = result && archive.EndTag(hobbiesTag);
			return result;
		}
	};

	// Usage: Save to file
	{
		CPerson person;
		person.m_name = "Alice";
		person.m_age = 30;
		person.m_hobbies << "Reading" << "Hiking" << "Coding";
		
		ifile::CFileWriteArchive archive("person.dat");
		if (archive.IsOpen()) {
			person.Serialize(archive);
		}
	}

	// Usage: Load from file
	{
		CPerson person;
		ifile::CFileReadArchive archive("person.dat");
		if (archive.IsOpen()) {
			person.Serialize(archive);
			// person now contains loaded data
		}
	}

	// Usage: Save to XML
	{
		CPerson person;
		person.m_name = "Bob";
		person.m_age = 25;
		
		ifile::CSimpleXmlFileWriteArchive xmlArchive("person.xml");
		if (xmlArchive.IsOpen()) {
			person.Serialize(xmlArchive);
		}
	}
	\endcode

	\section ArchiveBestPractices Best Practices
	- Always check return values from BeginTag(), Process(), and EndTag()
	- Use static CArchiveTag objects to avoid repeated construction
	- Match each BeginTag() with an EndTag() call
	- Use BeginMultiTag() for collections and arrays
	- Check IsStoring() to differentiate read/write logic
	- Provide meaningful tag names and descriptions for debugging
	- For optional data, check version info before reading
	- Always validate data after reading (ranges, null checks, etc.)

	\sa iser::ISerializable, iser::CArchiveTag, ifile::CFileReadArchive, 
	    ifile::CFileWriteArchive, ifile::CJsonFileReadArchive

	\ingroup Persistence
	\ingroup Main
*/
class IArchive: virtual public istd::IPolymorphic
{
public:
	enum MessageId
	{
		MI_TAG_ERROR = 0x3f320a0,
		MI_TAG_SKIPPED
	};

	/**
		Checks if this archive is in storing (writing) or loading (reading) mode.
		
		This method is fundamental for implementing bidirectional serialization.
		The same Serialize() method is used for both reading and writing, and
		IsStoring() tells you which operation is being performed.
		
		\return true if the archive is writing/storing data, false if reading/loading data
		
		\code
		virtual bool Serialize(iser::IArchive& archive)
		{
			if (archive.IsStoring()) {
				// Prepare data for writing
				int count = m_items.size();
				archive.Process(count);
				for (const Item& item : m_items) {
					// Write items
				}
			} else {
				// Read data
				int count;
				archive.Process(count);
				m_items.clear();
				for (int i = 0; i < count; ++i) {
					// Read items
				}
			}
		}
		\endcode
		
		\sa Process()
	 */
	virtual bool IsStoring() const = 0;

	/**
		Checks if skipping to the end of a tag on EndTag() is supported.
		
		Some archive types (like XML) support skipping unread content within a tag,
		allowing forward compatibility. When reading an archive created by a newer
		version that added fields, this feature lets you skip unknown data.
		
		\return true if the archive supports tag skipping, false otherwise
		
		\note Binary archives typically don't support skipping, requiring all data
		      to be read sequentially.
		\note XML and JSON archives usually support skipping.
		
		\code
		if (archive.IsTagSkippingSupported()) {
			// Can safely skip new fields added in future versions
		} else {
			// Must read all data in exact order
		}
		\endcode
		
		\sa EndTag()
	*/
	virtual bool IsTagSkippingSupported() const = 0;

	/**
		Gets version information for the archived stream.
		
		Version information allows handling compatibility between different versions
		of your data format. You can query the version to decide whether to read
		optional or newly added fields.
		
		\return Reference to version information object
		
		\code
		const IVersionInfo& version = archive.GetVersionInfo();
		if (version.GetVersion() >= CMyClass::VERSION_WITH_NEW_FIELD) {
			// New field exists, read it
			archive.Process(m_newField);
		}
		\endcode
		
		\sa iser::IVersionInfo, iser::CMinimalVersionInfo
	 */
	virtual const IVersionInfo& GetVersionInfo() const = 0;

	/**
		Begins a tagged section in the archive.
		
		Tags organize data into logical units with names and descriptions. Every
		BeginTag() must be matched with an EndTag() call. Tags can be nested to
		create hierarchical data structures.
		
		\param tag The tag object describing this section. Use static CArchiveTag
		          instances to avoid repeated construction overhead.
		
		\return true if the tag was successfully opened, false on error
		
		\note Always match BeginTag() with EndTag()
		\note Check the return value and propagate errors
		\note For collections, use BeginMultiTag() instead
		
		\code
		static iser::CArchiveTag personTag("Person", "Person data");
		if (archive.BeginTag(personTag)) {
			archive.Process(name);
			archive.Process(age);
			archive.EndTag(personTag);
		}
		\endcode
		
		\sa EndTag(), BeginMultiTag(), CArchiveTag
	*/
	virtual bool BeginTag(const CArchiveTag& tag) = 0;

	/**
		Begins a tagged section containing multiple elements of the same type.
		
		BeginMultiTag is used for serializing collections, arrays, or lists where
		you have multiple items of the same structure. The count parameter works
		differently for reading vs. writing:
		- **Writing**: Pass the number of elements you will serialize
		- **Reading**: The archive sets count to the number of elements stored
		
		\param tag Main container tag for the entire collection
		\param subTag Tag type for each individual element in the collection
		\param count [in/out] For writing: number of elements to serialize
		                     For reading: set by archive to number of stored elements
		
		\return true if successful, false on error
		
		\code
		// Writing a list
		if (archive.IsStoring()) {
			int count = m_items.count();
			archive.BeginMultiTag(listTag, itemTag, count);
			for (const Item& item : m_items) {
				archive.BeginTag(itemTag);
				item.Serialize(archive);
				archive.EndTag(itemTag);
			}
			archive.EndTag(listTag);
		}
		// Reading a list
		else {
			int count;
			archive.BeginMultiTag(listTag, itemTag, count);
			m_items.clear();
			for (int i = 0; i < count; ++i) {
				archive.BeginTag(itemTag);
				Item item;
				item.Serialize(archive);
				m_items.append(item);
				archive.EndTag(itemTag);
			}
			archive.EndTag(listTag);
		}
		\endcode
		
		\sa BeginTag(), EndTag()
	*/
	virtual bool BeginMultiTag(const CArchiveTag& tag, const CArchiveTag& subTag, int& count) = 0;

	/**
		Ends a tagged section in the archive.
		
		Must be called after BeginTag() or BeginMultiTag() to close the section.
		If tag skipping is supported and not all data was read, this will skip
		to the end of the tag section, enabling forward compatibility.
		
		\param tag The same tag object passed to BeginTag() or BeginMultiTag()
		
		\return true if successful, false on error
		
		\note Always call EndTag() even if errors occurred within the tag
		\note The tag parameter must match the one used in BeginTag()
		\note If IsTagSkippingSupported() returns false, you must read all
		      data before calling EndTag()
		
		\code
		static iser::CArchiveTag dataTag("Data", "My data");
		if (archive.BeginTag(dataTag)) {
			bool success = archive.Process(myData);
			archive.EndTag(dataTag);  // Always call, even if Process failed
			if (!success) return false;
		}
		\endcode
		
		\sa BeginTag(), BeginMultiTag(), IsTagSkippingSupported()
	*/
	virtual bool EndTag(const CArchiveTag& tag) = 0;

	/**
		Processes (reads or writes) a boolean value.
		
		\param value [in/out] For writing: the value to store
		                     For reading: receives the loaded value
		
		\return true if successful, false on error
		
		\sa IsStoring(), Process() overloads for other types
	 */
	virtual bool Process(bool& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(char& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(quint8& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(qint8& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(quint16& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(qint16& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(quint32& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(qint32& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(quint64& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(qint64& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(float& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(double& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(QByteArray& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(QString& value) = 0;

	template<typename Primitive>
	bool TagAndProcess(const CArchiveTag& tag, Primitive& value){
		static_assert(!std::is_const_v<Primitive>, "Input value is const-qualified");
		
		return BeginTag(tag) && Process(value) && EndTag(tag);
	}

	/**
		Process binary data block.
		\param	dataPtr	pointer to memory block.
		\param	size	size of memory block in bytes.
	*/
	virtual bool ProcessData(void* dataPtr, int size) = 0;

	/**
		Process binary data block.
		\param	dataPtr	pointer to memory block.
		\param	bitsCount	number of bits.
		\param	bytesCount	size of memory block in bytes.
	*/
	virtual bool ProcessBits(void* dataPtr, int bitsCount, int bytesCount) = 0;
};


} // namespace iser


#endif // !iser_IArchive_included

