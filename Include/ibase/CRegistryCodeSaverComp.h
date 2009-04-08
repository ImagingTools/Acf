#ifndef istdc_CRegistryCodeSaverComp_included
#define istdc_CRegistryCodeSaverComp_included


// STL includes
#include <list>

#include "iser/IFileLoader.h"

#include "icomp/IRegistry.h"
#include "icomp/CComponentBase.h"


namespace ibase
{


class CRegistryCodeSaverComp: public icomp::CComponentBase, public iser::IFileLoader
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CRegistryCodeSaverComp);
		I_REGISTER_INTERFACE(iser::IFileLoader);
	I_END_COMPONENT;

	CRegistryCodeSaverComp();

	// reimplemented (iser::IFileLoader)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const istd::CString* filePathPtr = NULL,
				int flags = 0,
				bool beQuiet = true) const;
	virtual int LoadFromFile(istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;
	virtual int SaveToFile(const istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;
	virtual bool GetFileExtensions(istd::CStringList& result, bool doAppend = false) const;
	virtual istd::CString GetTypeDescription(const istd::CString* extensionPtr = NULL) const;

protected:
	bool WriteRegistryIncludes(
				const icomp::IRegistry& registry,
				std::ofstream& stream);
	bool WriteRegistryInfo(
				const icomp::IRegistry& registry,
				std::ofstream& stream);
	bool WriteComponentInfo(
				const icomp::IRegistry& registry,
				const std::string& componentId,
				const icomp::IRegistry::ElementInfo& componentInfo,
				std::ofstream& stream);
	bool WriteAttribute(
				const std::string& attributeId,
				const std::string& attributeName,
				const iser::ISerializable& attribute,
				std::ofstream& stream);

	bool GetSingleAttributeValue(
				const iser::ISerializable& attribute,
				std::string& valueString,
				std::string& typeName) const;
	bool GetMultipleAttributeValue(
				const iser::ISerializable& attribute,
				std::list<std::string>& valueStrings,
				std::string& typeName) const;

	bool NextLine(std::ofstream& stream);
	int ChangeIndent(int difference);

private:
	int m_indentCount;
};


} // namespace ibase


#endif // !istdc_CRegistryCodeSaverComp_included


