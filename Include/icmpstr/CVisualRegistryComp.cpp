#include "icmpstr/CVisualRegistryComp.h"


#include "istd/TChangeNotifier.h"
#include "istd/CClassInfo.h"

#include "icomp/CInterfaceManipBase.h"


// public methods


namespace icmpstr
{


bool CVisualRegistryComp::SerializeComponentsLayout(iser::IArchive& archive)
{
	static iser::CArchiveTag positionMapTag("PositionMap", "Map of component name to its positions");
	static iser::CArchiveTag elementTag("Element", "Map element");

	bool retVal = true;

	Ids ids = GetElementIds();
	int positionsCount = ids.size();

	retVal = retVal && archive.BeginMultiTag(positionMapTag, elementTag, positionsCount);

	if (!retVal){
		return false;
	}

	if (archive.IsStoring()){
		for (Ids::const_iterator iter = ids.begin(); iter != ids.end(); ++iter){
			std::string elementId = *iter;

			i2d::CVector2d position(0, 0);
			const ElementInfo* infoPtr = GetElementInfo(elementId);
			if (infoPtr != NULL){
				const CVisualRegistryElement* elementPtr = dynamic_cast<const CVisualRegistryElement*>(infoPtr->elementPtr.GetPtr());
				if (elementPtr != NULL){
					i2d::CVector2d position = elementPtr->GetCenter();
				}
			}

			retVal = retVal && archive.BeginTag(elementTag);

			retVal = retVal && SerializeComponentPosition(archive, elementId, position);
				
			retVal = retVal && archive.EndTag(elementTag);
		}
	}
	else{
		istd::CChangeNotifier notifier(this);

		for (int i = 0; i < positionsCount; ++i){
			retVal = retVal && archive.BeginTag(elementTag);
			
			std::string elementId;
			i2d::CVector2d position;

			retVal = retVal && SerializeComponentPosition(archive, elementId, position);
			if (!retVal){
				return false;
			}

			const ElementInfo* infoPtr = GetElementInfo(elementId);
			if (infoPtr != NULL){
				CVisualRegistryElement* elementPtr = dynamic_cast<CVisualRegistryElement*>(infoPtr->elementPtr.GetPtr());
				if (elementPtr != NULL){
					elementPtr->MoveTo(position);
				}
			}

			retVal = retVal && archive.EndTag(elementTag);
		}
	
	}

	retVal = retVal && archive.EndTag(positionMapTag);

	return retVal;
}


bool CVisualRegistryComp::SerializeRegistry(iser::IArchive& archive)
{
	return BaseClass2::Serialize(archive);
}


// reimplemented (icomp::IComponent)

void CVisualRegistryComp::OnComponentCreated()
{
	if (m_staticInfoCompPtr.IsValid()){
		SetComponentStaticInfo(m_staticInfoCompPtr.GetPtr());
	}
}


// reimplemented (icomp::IRegistry)

CVisualRegistryComp::ElementInfo* CVisualRegistryComp::InsertElementInfo(
			const std::string& elementId,
			const icomp::CComponentAddress& address,
			bool ensureElementCreated)
{
	ElementInfo* infoPtr = BaseClass2::InsertElementInfo(elementId, address, ensureElementCreated);

	if (infoPtr != NULL){
		CVisualRegistryElement* elementPtr = dynamic_cast<CVisualRegistryElement*>(infoPtr->elementPtr.GetPtr());
		if (elementPtr != NULL){
			elementPtr->SetName(elementId);
		}
	}
	else{
		SendErrorMessage(
					MI_CANNOT_CREATE_ELEMENT,
					iqt::GetCString(QObject::tr("Cannot create %1 (%2: %3)").
								arg(elementId.c_str()).
								arg(address.GetPackageId().c_str()).
								arg(address.GetComponentId().c_str())));
	}

	return infoPtr;
}


// reimplemented (iser::ISerializable)

bool CVisualRegistryComp::Serialize(iser::IArchive& archive)
{
	return BaseClass2::Serialize(archive) && SerializeComponentsLayout(archive);
}


// protected methods

bool CVisualRegistryComp::SerializeComponentPosition(iser::IArchive& archive, std::string& componentRole, i2d::CVector2d& position)
{
	static iser::CArchiveTag nameTag("ComponentName", "Name of component");
	static iser::CArchiveTag positionXTag("X", "X position of component");
	static iser::CArchiveTag positionYTag("Y", "Y position of component");
	
	bool retVal = archive.BeginTag(nameTag);
	retVal = retVal && archive.Process(componentRole);
	retVal = retVal && archive.EndTag(nameTag);

	retVal = retVal && archive.BeginTag(positionXTag);
	retVal = retVal && archive.Process(position[0]);
	retVal = retVal && archive.EndTag(positionXTag);

	retVal = retVal && archive.BeginTag(positionXTag);
	retVal = retVal && archive.Process(position[1]);
	retVal = retVal && archive.EndTag(positionXTag);

	return retVal;
}


// reimplemented (icomp::CRegistry)

icomp::IRegistryElement* CVisualRegistryComp::CreateRegistryElement(const icomp::CComponentAddress& address) const
{
	const icomp::IComponentStaticInfo* componentsFactoryPtr = GetComponentStaticInfo();
	if (componentsFactoryPtr != NULL){
		const icomp::IComponentStaticInfo* packageInfoPtr = componentsFactoryPtr->GetSubcomponentInfo(address.GetPackageId());
		if (packageInfoPtr != NULL){
			const icomp::IComponentStaticInfo* componentInfoPtr = packageInfoPtr->GetSubcomponentInfo(address.GetComponentId());
			if (componentInfoPtr != NULL){
				Element* registryElementPtr = new Element;
				if (registryElementPtr != NULL){
					registryElementPtr->Initialize(this, componentInfoPtr, address);
					registryElementPtr->SetSlavePtr(const_cast<CVisualRegistryComp*>(this));

					return registryElementPtr;
				}
			}
		}
	}

	return NULL;
}


} // namespace icmpstr


