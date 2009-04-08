#include "ibase/CRegistryCodeSaverComp.h"


// STL includes
#include <fstream>


namespace ibase
{


CRegistryCodeSaverComp::CRegistryCodeSaverComp()
:	m_indentCount(0)
{
}


// reimplemented (iser::IFileLoader)

bool CRegistryCodeSaverComp::IsOperationSupported(
		const istd::IChangeable* dataObjectPtr,
		const istd::CString* /*filePathPtr*/,
		int flags,
		bool /*beQuiet*/) const
{
	if (dynamic_cast<const icomp::IRegistry*>(dataObjectPtr) == NULL){
		return false;
	}

	return (flags & QF_NO_SAVING) == 0;
}


int CRegistryCodeSaverComp::LoadFromFile(istd::IChangeable& /*data*/, const istd::CString& /*filePath*/) const
{
	return StateFailed;
}


int CRegistryCodeSaverComp::SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const
{
	const icomp::IRegistry* registryPtr = dynamic_cast<const icomp::IRegistry*>(&data);
	if (registryPtr != NULL){
		std::ofstream stream(filePath.ToString().c_str());

		CRegistryCodeSaverComp& me = const_cast<CRegistryCodeSaverComp&>(*this);
		if (me.WriteRegistryIncludes(*registryPtr, stream) && me.WriteRegistryInfo(*registryPtr, stream)){
			return StateOk;
		}
	}

	return StateFailed;
}


bool CRegistryCodeSaverComp::GetFileExtensions(istd::CStringList& result, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	result.push_back("h");
	result.push_back("hpp");
	result.push_back("c");
	result.push_back("cpp");

	return true;
}


istd::CString CRegistryCodeSaverComp::GetTypeDescription(const istd::CString* extensionPtr) const
{
	if (		(extensionPtr == NULL) ||
				extensionPtr->IsEqualNoCase("h") ||
				extensionPtr->IsEqualNoCase("hpp") ||
				extensionPtr->IsEqualNoCase("c") ||
				extensionPtr->IsEqualNoCase("cpp")){
		return "QSF registry file";
	}

	return "";
}


// protected methods

bool CRegistryCodeSaverComp::WriteRegistryIncludes(
			const icomp::IRegistry& registry,
			std::ofstream& stream)
{
	typedef std::set<std::string> PackageIds;

	PackageIds packageIds;

	icomp::IRegistry::Ids ids = registry.GetElementIds();
	for (		icomp::IRegistry::Ids::const_iterator elementIter = ids.begin();
				elementIter != ids.end();
				++elementIter){
		const std::string& componentName = *elementIter;

		const icomp::IRegistry::ElementInfo* infoPtr = registry.GetElementInfo(componentName);
		I_ASSERT(infoPtr != NULL);	// used element ID was returned by registry, info must exist.

		const std::string& packageId = infoPtr->address.GetPackageId();

		packageIds.insert(packageId);
	}
	
	for (		PackageIds::const_iterator packageIter = packageIds.begin();
				packageIter != packageIds.end();
				++packageIter){
		const std::string& packageId = *packageIter;

		stream << "#include \"" << packageId << "/" << packageId << ".h\"" << std::endl;
	}

	stream << std::endl;

	return !stream.fail();
}


bool CRegistryCodeSaverComp::WriteRegistryInfo(
			const icomp::IRegistry& registry,
			std::ofstream& stream)
{
	NextLine(stream);
	stream << "bool SetupRegistry(icomp::CRegistry& registry)";
	NextLine(stream);
	stream << "{";
	ChangeIndent(1);

	icomp::IRegistry::Ids ids = registry.GetElementIds();
	for (		icomp::IRegistry::Ids::const_iterator elementIter = ids.begin();
				elementIter != ids.end();
				++elementIter){
		const std::string& componentName = *elementIter;

		const icomp::IRegistry::ElementInfo* infoPtr = registry.GetElementInfo(componentName);
		I_ASSERT(infoPtr != NULL);	// used element ID was returned by registry, info must exist.

		if (elementIter != ids.begin()){
			stream << std::endl;
		}

		NextLine(stream);
		stream << "// Element '" << componentName << "' of type " << infoPtr->address.GetPackageId() << "::" << infoPtr->address.GetComponentId();

		WriteComponentInfo(registry, componentName, *infoPtr, stream);
	}

	stream << std::endl;

	NextLine(stream);
	stream << "// Interface export";
	const icomp::IRegistry::ExportedInterfacesMap& interfacesMap = registry.GetExportedInterfacesMap();
	for (		icomp::IRegistry::ExportedInterfacesMap::const_iterator interfaceIter = interfacesMap.begin();
				interfaceIter != interfacesMap.end();
				++interfaceIter){
		const istd::CClassInfo& info = interfaceIter->first;
		const std::string& componentName = interfaceIter->second;

		NextLine(stream);
		stream << "registry.SetElementExported(";
		NextLine(stream);
		stream << "\t\t\t\"" << componentName << "\",";
		NextLine(stream);
		stream << "\t\t\tistd::CClassInfo(\"" << info.GetName() << "\"),";
		NextLine(stream);
		stream << "\t\t\ttrue);";
	}

	stream << std::endl;

	NextLine(stream);
	stream << "// Setting of meta info";
	NextLine(stream);
	stream << "registry.SetDescription(\"" << registry.GetDescription().ToString() << "\");";
	NextLine(stream);
	stream << "registry.SetKeywords(\"" << registry.GetKeywords().ToString() << "\");";
	stream << std::endl;
	NextLine(stream);
	stream << "return true;";
	ChangeIndent(-1);
	NextLine(stream);
	stream << "}" << std::endl;

	return !stream.fail();
}


bool CRegistryCodeSaverComp::WriteComponentInfo(
			const icomp::IRegistry& registry,
			const std::string& componentName,
			const icomp::IRegistry::ElementInfo& componentInfo,
			std::ofstream& stream)
{
	std::string elementInfoName = "info" + componentName + "Ptr";
	NextLine(stream);
	stream << "icomp::IRegistry::ElementInfo* " << elementInfoName << " = registry.InsertElementInfo(";
	NextLine(stream);
	stream << "\t\t\t\"" << componentName << "\",";
	NextLine(stream);
	stream << "\t\t\ticomp::CComponentAddress(\"" << componentInfo.address.GetPackageId();
	stream << "\", \"" << componentInfo.address.GetComponentId() << "\"),";
	NextLine(stream);
	stream << "\t\t\tfalse);";

	NextLine(stream);
	stream << "if (" << elementInfoName << " == NULL){";
	ChangeIndent(1);
	NextLine(stream);
	stream << "return false;";
	ChangeIndent(-1);
	NextLine(stream);
	stream << "}" << std::endl;

	NextLine(stream);
	stream << elementInfoName << ".elementPtr.SetPtr(new icomp::CRegistryElement(&";
	stream << componentInfo.address.GetPackageId() << "::" << componentInfo.address.GetComponentId() << "::InitStaticInfo(NULL)));";
	stream << std::endl;

	NextLine(stream);
	stream << "if (" << elementInfoName << ".elementPtr.IsValid()){";
	ChangeIndent(1);
	if (componentInfo.elementPtr.IsValid()){
		icomp::IRegistryElement& component = *componentInfo.elementPtr;
		icomp::IRegistryElement::Ids attributeIds = component.GetAttributeIds();

		for (		icomp::IRegistryElement::Ids::const_iterator attributeIter = attributeIds.begin();
					attributeIter != attributeIds.end();
					++attributeIter){
			const std::string& attributeId = *attributeIter;

			const icomp::IRegistryElement::AttributeInfo* attrInfoPtr = component.GetAttributeInfo(attributeId);

			if (attributeIter != attributeIds.begin()){
				stream << std::endl;
			}

			NextLine(stream);
			stream << "// Create and set attribute value for '" << attributeId << "'";
			if (attrInfoPtr != NULL){
				std::string attributeInfoName = "attrInfo" + attributeId + "Ptr";

				NextLine(stream);
				stream << "icomp::IRegistryElement::AttributeInfo* " << attributeInfoName << " = ";
				stream << elementInfoName << ".elementPtr->InsertAttributeInfo(\"" << attributeId << "\", true);";

				NextLine(stream);
				stream << "if (" << attributeInfoName << " == NULL){";
				ChangeIndent(1);
				NextLine(stream);
				stream << "return false;";
				ChangeIndent(-1);
				NextLine(stream);
				stream << "}";

				if (!attrInfoPtr->exportId.empty()){
					NextLine(stream);
					stream << attributeInfoName << "->exportId = \"" << attrInfoPtr->exportId << "\";";
				}

				if (attrInfoPtr->attributePtr.IsValid()){
					WriteAttribute(attributeId, attributeInfoName, *attrInfoPtr->attributePtr, stream);
				}
			}
		}
	}
	ChangeIndent(-1);
	NextLine(stream);
	stream << "}";

	return !stream.fail();
}


bool CRegistryCodeSaverComp::WriteAttribute(
			const std::string& attributeId,
			const std::string& attributeInfoName,
			const iser::ISerializable& attribute,
			std::ofstream& stream)
{
	NextLine(stream);
	stream << "I_ASSERT(" << attributeInfoName << "->attributePtr.IsValid());";
	stream << std::endl;

	std::string attributeType;
	std::string valueString;
	std::list<std::string> valueStrings;

	std::string attributeName = "attr" + attributeId + "Ptr";

	if (GetSingleAttributeValue(attribute, valueString, attributeType)){
		NextLine(stream);
		stream << attributeType << "* " << attributeName << " = dynamic_cast<" << attributeType << ">(" << attributeInfoName << "->attributePtr.GetPtr());";
		NextLine(stream);
		stream << "I_ASSERT(" << attributeName << " != NULL);";
		NextLine(stream);
		stream << attributeName << "->SetValue(" << valueString << ");";

		return true;
	}
	else if (GetMultipleAttributeValue(attribute, valueStrings, attributeType)){
		NextLine(stream);
		stream << attributeType << "* n" << attributeInfoName << " = dynamic_cast<" << attributeType << ">(" << attributeInfoName << ");";
		NextLine(stream);
		stream << "I_ASSERT(n" << attributeInfoName << " != NULL);";
		for (		std::list<std::string>::const_iterator iter = valueStrings.begin();
					iter != valueStrings.end();
					++iter){
			NextLine(stream);
			stream << "n" << attributeInfoName << "->InsertValue(" << valueString << ");";
		}

		return true;
	}

	return false;
}


bool CRegistryCodeSaverComp::GetSingleAttributeValue(
			const iser::ISerializable& attribute,
			std::string& valueString,
			std::string& typeName) const
{
	const icomp::CBoolAttribute* boolAttribute = dynamic_cast<const icomp::CBoolAttribute*>(&attribute);
	if (boolAttribute != NULL){
		valueString = boolAttribute->GetValue() ? "true": "false";
		typeName = "icomp::CBoolAttribute";

		return true;
	}

	const icomp::CDoubleAttribute* doubleAttribute = dynamic_cast<const icomp::CDoubleAttribute*>(&attribute);
	if (doubleAttribute != NULL){
		valueString = istd::CString::FromNumber(doubleAttribute->GetValue()).ToString();
		typeName = "icomp::CDoubleAttribute";

		return true;
	}

	const icomp::CIntAttribute* intAttribute = dynamic_cast<const icomp::CIntAttribute*>(&attribute);
	if (intAttribute != NULL){
		valueString = istd::CString::FromNumber(intAttribute->GetValue()).ToString();
		typeName = "icomp::CIntAttribute";

		return true;
	}

	const icomp::CStringAttribute* stringAttribute = dynamic_cast<const icomp::CStringAttribute*>(&attribute);
	if (stringAttribute != NULL){
		valueString = (istd::CString("\"") + stringAttribute->GetValue() + "\"").ToString();
		typeName = "icomp::CStringAttribute";

		return true;
	}

	const icomp::TSingleAttribute<std::string>* idPtr = dynamic_cast<const icomp::TSingleAttribute<std::string>*>(&attribute);
	if (idPtr != NULL){		
		valueString = istd::CString("\"" + idPtr->GetValue() + "\"").ToString();
		typeName = "icomp::TSingleAttribute<std::string>";

		return true;
	}

	return false;
}


bool CRegistryCodeSaverComp::GetMultipleAttributeValue(
			const iser::ISerializable& attribute,
			std::list<std::string>& valueStrings,
			std::string& typeName) const
{
	valueStrings.clear();

	const icomp::CMultiStringAttribute* stringListAttribute = dynamic_cast<const icomp::CMultiStringAttribute*>(&attribute);
	if (stringListAttribute != NULL){
		for (int index = 0; index < stringListAttribute->GetValuesCount(); index++){
			valueStrings.push_back((istd::CString("\"") + stringListAttribute->GetValueAt(index) + "\"").ToString());
			typeName = "icomp::CMultiStringAttribute";
		}

		return true;
	}

	const icomp::CMultiIntAttribute* intListAttribute = dynamic_cast<const icomp::CMultiIntAttribute*>(&attribute);
	if (intListAttribute != NULL){
		for (int index = 0; index < intListAttribute->GetValuesCount(); index++){
			valueStrings.push_back(istd::CString::FromNumber(intListAttribute->GetValueAt(index)).ToString());
			typeName = "icomp::CMultiIntAttribute";
		}

		return true;
	}

	const icomp::CMultiDoubleAttribute* doubleListAttribute = dynamic_cast<const icomp::CMultiDoubleAttribute*>(&attribute);
	if (doubleListAttribute != NULL){
		for (int index = 0; index < doubleListAttribute->GetValuesCount(); index++){
			valueStrings.push_back(istd::CString::FromNumber(doubleListAttribute->GetValueAt(index)).ToString());
			typeName = "icomp::CMultiDoubleAttribute";
		}

		return true;
	}

	const icomp::TMultiAttribute<std::string>* multiIdPtr = dynamic_cast<const icomp::CMultiReferenceAttribute*>(&attribute);
	if (multiIdPtr != NULL){
		for (int index = 0; index < multiIdPtr->GetValuesCount(); index++){
			valueStrings.push_back(istd::CString("\"" + multiIdPtr->GetValueAt(index) + "\"").ToString());
			typeName = "icomp::TMultiAttribute<std::string>";
		}

		return true;
	}

	return false;
}


bool CRegistryCodeSaverComp::NextLine(std::ofstream& stream)
{
	stream << std::endl;

	for (int i = 0; i < m_indentCount; ++i){
		stream << "\t";
	}

	return !stream.fail();
}


int CRegistryCodeSaverComp::ChangeIndent(int difference)
{
	m_indentCount += difference;

	I_ASSERT(m_indentCount >= 0);

	return m_indentCount;
}


} // namespace ibase


