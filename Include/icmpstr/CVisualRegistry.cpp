#include "icmpstr/CVisualRegistry.h"


// Qt includes
#include <QtCore/QDir>

// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/TChangeDelegator.h"
#include "istd/CClassInfo.h"

#include "icomp/CInterfaceManipBase.h"


// public methods


namespace icmpstr
{


CVisualRegistry::CVisualRegistry()
:	m_serializeUserData(true)
{
}


bool CVisualRegistry::SerializeComponentsLayout(iser::IArchive& archive)
{
	static iser::CArchiveTag positionMapTag("PositionMap", "Map of component name to its positions");
	static iser::CArchiveTag elementTag("Element", "Map element");
	static iser::CArchiveTag embeddedRegistriesTag("EmbeddedRegistries", "List of embedded registries for recursive layout loading");
	static iser::CArchiveTag embeddedRegistryTag("EmbededRegistry", "Layout information of embedded registry");
	static iser::CArchiveTag embeddedRegistryIdTag("Id", "ID of embedded registry");

	bool retVal = true;

	Ids ids = GetElementIds();
	int positionsCount = ids.size();

	retVal = retVal && archive.BeginMultiTag(positionMapTag, elementTag, positionsCount);

	if (!retVal){
		return false;
	}

	if (archive.IsStoring()){
		for (Ids::const_iterator iter = ids.begin(); iter != ids.end(); ++iter){
			QByteArray elementId = *iter;

			i2d::CVector2d position(0, 0);
			QString componentNote;
			const ElementInfo* infoPtr = GetElementInfo(elementId);
			if (infoPtr != NULL){
				const CVisualRegistryElement* elementPtr = dynamic_cast<const CVisualRegistryElement*>(infoPtr->elementPtr.GetPtr());
				if (elementPtr != NULL){
					position = elementPtr->GetCenter();

					componentNote = elementPtr->GetNote();
				}
			}

			retVal = retVal && archive.BeginTag(elementTag);

			retVal = retVal && SerializeComponentInfo(archive, elementId, position, componentNote);

			retVal = retVal && archive.EndTag(elementTag);
		}
	}
	else{
		istd::CChangeNotifier notifier(this);

		for (int i = 0; i < positionsCount; ++i){
			retVal = retVal && archive.BeginTag(elementTag);
			
			QByteArray elementId;
			i2d::CVector2d position;
			QString componentNote;

			retVal = retVal && SerializeComponentInfo(archive, elementId, position, componentNote);
			if (!retVal){
				return false;
			}

			const ElementInfo* infoPtr = GetElementInfo(elementId);
			if (infoPtr != NULL){
				CVisualRegistryElement* elementPtr = dynamic_cast<CVisualRegistryElement*>(infoPtr->elementPtr.GetPtr());
				if (elementPtr != NULL){
					elementPtr->MoveCenterTo(position);

					elementPtr->SetNote(componentNote);
				}
			}

			retVal = retVal && archive.EndTag(elementTag);
		}
	}

	retVal = retVal && archive.EndTag(positionMapTag);

	const iser::IVersionInfo& versionInfo = archive.GetVersionInfo();
	quint32 frameworkVersion = 0;
	if (		!versionInfo.GetVersionNumber(iser::IVersionInfo::AcfVersionId, frameworkVersion) ||
				(frameworkVersion >= 2516)){
		// process embedded registries
		ids = GetEmbeddedRegistryIds();
		positionsCount = ids.size();

		retVal = retVal && archive.BeginMultiTag(embeddedRegistriesTag, embeddedRegistryTag, positionsCount);

		if (!retVal){
			return false;
		}

		if (archive.IsStoring()){
			for (Ids::const_iterator iter = ids.begin(); iter != ids.end(); iter++){
				QByteArray id = *iter;
				CVisualRegistry* embeddedRegPtr = dynamic_cast<CVisualRegistry*> (GetEmbeddedRegistry(id));
				if (embeddedRegPtr != NULL){
					retVal = retVal && archive.BeginTag(embeddedRegistryTag);
					retVal = retVal && archive.BeginTag(embeddedRegistryIdTag);
					archive.Process(id);
					retVal = retVal && archive.EndTag(embeddedRegistryIdTag);

					// serialize embedded PositionMap etc.
					embeddedRegPtr->SerializeComponentsLayout(archive);

					retVal = retVal && archive.EndTag(embeddedRegistryTag);
				}
			}
		}
		else{
			for (int i = 0; i < positionsCount; i++){
				retVal = retVal && archive.BeginTag(embeddedRegistryTag);
				retVal = retVal && archive.BeginTag(embeddedRegistryIdTag);
				QByteArray id;

				archive.Process(id);

				retVal = retVal && archive.EndTag(embeddedRegistryIdTag);

				if (!retVal){
					return false;
				}

				CVisualRegistry* embeddedRegPtr = dynamic_cast<CVisualRegistry*> (GetEmbeddedRegistry(id));
				if (embeddedRegPtr != NULL){
					// serialize embedded PositionMap etc.
					embeddedRegPtr->SerializeComponentsLayout(archive);
				}

				retVal = retVal && archive.EndTag(embeddedRegistryTag);
			}
		}

		retVal = retVal && archive.EndTag(embeddedRegistriesTag);
	}

	return retVal;
}


bool CVisualRegistry::SerializeRegistry(iser::IArchive& archive)
{
	return BaseClass::Serialize(archive);
}


bool CVisualRegistry::SerializeUserData(iser::IArchive& archive)
{
	return SerializeComponentsLayout(archive);
}


// reimplemented (IComponentNoteController)

QString CVisualRegistry::GetComponentNote(const QByteArray& componentName)
{
	const ElementInfo* elementInfoPtr = GetElementInfo(componentName);
	if (elementInfoPtr != NULL){
		CVisualRegistryElement* elementPtr = dynamic_cast<CVisualRegistryElement*>(elementInfoPtr->elementPtr.GetPtr());
		if (elementPtr != NULL){
			return elementPtr->GetNote();
		}
	}

	return QString();
}


void CVisualRegistry::SetComponentNote(const QByteArray& componentName, const QString& componentNote)
{
	const ElementInfo* elementInfoPtr = GetElementInfo(componentName);
	if (elementInfoPtr == NULL){
		return;
	}

	CVisualRegistryElement* elementPtr = dynamic_cast<CVisualRegistryElement*>(elementInfoPtr->elementPtr.GetPtr());
	if ((elementPtr != NULL) && (elementPtr->GetNote() != componentNote)){
		istd::CChangeNotifier changePtr(this, CF_NOTE_CHANGED | CF_MODEL);

		elementPtr->SetNote(componentNote);
	}
}


// reimplemented (icomp::IRegistry)

CVisualRegistry::ElementInfo* CVisualRegistry::InsertElementInfo(
			const QByteArray& elementId,
			const icomp::CComponentAddress& address,
			bool ensureElementCreated)
{
	ElementInfo* infoPtr = BaseClass::InsertElementInfo(elementId, address, ensureElementCreated);

	if (infoPtr != NULL){
		CVisualRegistryElement* elementPtr = dynamic_cast<CVisualRegistryElement*>(infoPtr->elementPtr.GetPtr());
		if (elementPtr != NULL){
			elementPtr->SetName(elementId);
		}
	}

	return infoPtr;
}


bool CVisualRegistry::RenameElement(const QByteArray& oldElementId, const QByteArray& newElementId)
{
	i2d::CVector2d oldPosition;
	const ElementInfo* oldElementInfoPtr = GetElementInfo(oldElementId);
	if (oldElementInfoPtr != NULL){
		CVisualRegistryElement* elementPtr = dynamic_cast<CVisualRegistryElement*>(oldElementInfoPtr->elementPtr.GetPtr());
		if (elementPtr != NULL){
			oldPosition = elementPtr->GetCenter();
		}
	}

	istd::TChangeNotifier<icomp::IRegistry> changePtr(this, CF_MODEL | CF_ELEMENT_RENAMED);

	if (BaseClass::RenameElement(oldElementId, newElementId)){
		const ElementInfo* newElementInfoPtr = GetElementInfo(newElementId);
		if (newElementInfoPtr != NULL){
			CVisualRegistryElement* elementPtr = dynamic_cast<CVisualRegistryElement*>(newElementInfoPtr->elementPtr.GetPtr());
			if (elementPtr != NULL){
				elementPtr->MoveCenterTo(oldPosition);
			}
		}

		return true;
	}
	else{
		changePtr.Abort();
	}

	return false;
}


// reimplemented (iser::ISerializable)

bool CVisualRegistry::Serialize(iser::IArchive& archive)
{
	bool retVal = SerializeRegistry(archive);

	if (m_serializeUserData){
		retVal = retVal && SerializeUserData(archive);
	}

	return retVal;
}


// protected methods

bool CVisualRegistry::SerializeComponentInfo(
			iser::IArchive& archive,
			QByteArray& componentRole,
			i2d::CVector2d& position,
			QString& componentNote)
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

	const iser::IVersionInfo& versionInfo = archive.GetVersionInfo();
	quint32 frameworkVersion = 0;
	versionInfo.GetVersionNumber(iser::IVersionInfo::AcfVersionId, frameworkVersion);
	
	if (frameworkVersion >= 2246 || archive.IsStoring()){
		static iser::CArchiveTag componentNoteTag("Note", "Component note");	
		retVal = retVal && archive.BeginTag(componentNoteTag);
		retVal = retVal && archive.Process(componentNote);
		retVal = retVal && archive.EndTag(componentNoteTag);
	}

	return retVal;
}


// reimplemented (icomp::CRegistry)

icomp::IRegistryElement* CVisualRegistry::CreateRegistryElement(
			const QByteArray& elementId,
			const icomp::CComponentAddress& address) const
{
	Element* registryElementPtr = new Element;
	if (registryElementPtr != NULL){
		registryElementPtr->Initialize(this, address);
		registryElementPtr->SetName(elementId);
		
		registryElementPtr->SetSlavePtr(const_cast<CVisualRegistry*>(this));

		return registryElementPtr;
	}

	return NULL;
}


icomp::IRegistry* CVisualRegistry::InsertEmbeddedRegistry(const QByteArray& registryId)
{
	// A specialization of this method is needed to enable component layout support in embedded compositions.
	istd::TDelPtr<CVisualRegistry> newRegistryPtr(new istd::TChangeDelegator< imod::TModelWrap<CVisualRegistry> >(this));
	newRegistryPtr->m_serializeUserData = false;

	RegistryPtr& registryPtr = m_embeddedRegistriesMap[registryId];
	if (registryPtr.IsValid()){
		return NULL; // such ID exists yet!
	}

	registryPtr.SetPtr(newRegistryPtr.PopPtr());

	return registryPtr.GetPtr();
}


} // namespace icmpstr


