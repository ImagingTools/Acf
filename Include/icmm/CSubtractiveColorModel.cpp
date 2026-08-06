// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icmm/CSubtractiveColorModel.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <icmm/CCmykColorModel.h>
#include <iser/IArchive.h>
#include <iser/CPrimitiveTypesSerializer.h>


namespace icmm
{


// public methods

CSubtractiveColorModel::CSubtractiveColorModel()
	: m_bridge(this, imod::CModelUpdateBridge::UF_SOURCE)
{
	m_colorants.AttachObserver(&m_bridge);
}


CSubtractiveColorModel::CSubtractiveColorModel(const ColorantIds& colorantIds)
	: CSubtractiveColorModel()
{
	for (const ColorantId& colorantId : colorantIds){
		ColorantUsage usage = GetDefaultUsageFromColorantName(colorantId);

		m_colorants.push_back({colorantId, usage});
	}
}


CSubtractiveColorModel::CSubtractiveColorModel(const CSubtractiveColorModel& other)
	: CSubtractiveColorModel()
{
	CSubtractiveColorModelBase::operator=(other);
	m_colorants.SetBaseObject(other.m_colorants);
}


CSubtractiveColorModel::CSubtractiveColorModel(const ISubtractiveColorModel& other)
	: CSubtractiveColorModel()
{
	for (const ColorantId& colorantId : other.GetColorantIds()){
		ColorantUsage usage = other.GetColorantUsage(colorantId);

		m_colorants.push_back({ colorantId, usage });
		icmm::CCieLabColor cieLab(nullptr);
		if (other.GetColorantVisualInfo(colorantId, cieLab)){
			SetColorantPreview(colorantId, cieLab.GetLab());
		}
	}
}


CSubtractiveColorModel& CSubtractiveColorModel::operator=(const CSubtractiveColorModel& other)
{
	CSubtractiveColorModelBase::operator=(other);
	m_colorants.SetBaseObject(other.m_colorants);
	return *this;
}


bool CSubtractiveColorModel::operator==(const CSubtractiveColorModel& ref) const
{
	return m_colorants == ref.m_colorants;
}


bool CSubtractiveColorModel::operator!=(const CSubtractiveColorModel& ref) const
{
	return !operator==(ref);
}


bool CSubtractiveColorModel::ContainsColorant(const ColorantId& colorantId) const
{
	return FindColorantIndex(colorantId) >= 0;
}


bool CSubtractiveColorModel::InsertColorant(const ColorantId & colorantId, ColorantUsage usage, int index)
{
	int existingIndex = FindColorantIndex(colorantId);
	if (existingIndex >= 0){
		return false;
	}

	if (index > m_colorants.size()){
		return false;
	}

	int insertPosition = index >= 0 ? index : m_colorants.size();

	m_colorants.insert(insertPosition, { colorantId, usage });

	return true;
}


bool CSubtractiveColorModel::MoveColorant(const ColorantId& colorantId, int index)
{
	if (index < 0 || index >= (int)m_colorants.size()){
		return false;
	}

	int fromIndex = FindColorantIndex(colorantId);
	if (fromIndex < 0){
		return false;
	}

	if (fromIndex == index){
		return true;
	}

	ColorantInfo colorantInfo = m_colorants[fromIndex];

	auto moveChanges = ElementMoveChanges(fromIndex, index);
	istd::CChangeNotifier changeNotifier(this, &moveChanges);

	m_colorants.remove(fromIndex);
	m_colorants.insert(index, colorantInfo);

	return true;
}


bool CSubtractiveColorModel::RemoveColorant(const ColorantId & colorantId)
{
	for (qsizetype i = 0; i < m_colorants.size(); ++i){
		if (m_colorants[i].id == colorantId){
			m_colorants.remove(i);
			return true;
		}
	}

	return false;
}


bool CSubtractiveColorModel::SetColorantUsage(const ColorantId& colorantId, ColorantUsage usage)
{
	int colorantIndex = FindColorantIndex(colorantId);
	if (colorantIndex >= 0){
		if (m_colorants[colorantIndex].usage != usage){
			auto changes = ElementUpdatedChanges(colorantIndex);
			istd::CChangeNotifier changeNotifier(this, &changes);

			m_colorants[colorantIndex].usage = usage;
		}

		return true;
	}

	Q_ASSERT_X(false, "CSubtractiveColorModel::SetColorantUsage", "Trying to change a non-existing colorant");

	return false;
}


bool CSubtractiveColorModel::SetColorantId(const ColorantId& colorantId, const ColorantId& newColorantId)
{
	if (colorantId == newColorantId){
		return true;
	}

	int colorantIndex = FindColorantIndex(colorantId);
	if (colorantIndex >= 0){
		auto changes = ElementUpdatedChanges(colorantIndex);
		istd::CChangeNotifier changeNotifier(this, &changes);
		
		m_colorants[colorantIndex].id = newColorantId;

		return true;
	}

	Q_ASSERT_X(false, "CSubtractiveColorModel::SetColorantId", "Trying to change a non-existing colorant");

	return false;
}


bool CSubtractiveColorModel::AppendColorModel(const ISubtractiveColorModel& other)
{
	CSubtractiveColorModel temp(*this);
	
	ColorantIds otherColorantIds = other.GetColorantIds();
	for (const ColorantId& otherColorantId : otherColorantIds){
		ColorantUsage otherUsage = other.GetColorantUsage(otherColorantId);

		// Check if the current model contains the colorant from the other model:
		int existingIndex = temp.FindColorantIndex(otherColorantId);
		if (existingIndex >= 0){
			// If yes, check the corresponding colorant usage:
			ColorantUsage myUsage = temp.GetColorantUsage(otherColorantId);
			
			// If colorant usages are not the same, the color model cannot be merged:
			if (myUsage != otherUsage){
				return false;
			}
			else {
				continue;
			}
		}

		temp.m_colorants.push_back({ otherColorantId, otherUsage });

		icmm::CCieLabColor cieLab(nullptr);
		if (other.GetColorantVisualInfo(otherColorantId, cieLab)){
			temp.SetColorantPreview(otherColorantId, cieLab.GetLab());
		}
	}

	if (m_colorants != temp.m_colorants){
		istd::CChangeNotifier changeNotifier(this, &istd::IChangeable::GetAllChanges());

		m_colorants.SetBaseObject(temp.m_colorants);
	}

	return true;
}


bool CSubtractiveColorModel::HasProcessColorants() const
{
	for (const ColorantInfo& colorant : m_colorants){
		if ((colorant.usage != icmm::CU_ECG) && (colorant.usage != icmm::CU_SPOT)){
			return true;
		}
	}

	return false;
}


bool CSubtractiveColorModel::HasEcg() const
{
	for (const ColorantInfo& colorant : m_colorants){
		if (colorant.usage == icmm::CU_ECG){
			return true;
		}
	}

	return false;
}


bool CSubtractiveColorModel::HasSpot() const
{
	for (const ColorantInfo& colorant : m_colorants){
		if (colorant.usage == icmm::CU_SPOT){
			return true;
		}
	}

	return false;
}


IColorantList::ColorantIds CSubtractiveColorModel::GetProcessColorants() const
{
	ColorantIds processColorants;

	for (const ColorantInfo& colorant : m_colorants){
		if ((colorant.usage != icmm::CU_ECG) && (colorant.usage != icmm::CU_SPOT)){
			processColorants.push_back(colorant.id);
		}
	}

	return processColorants;
}


IColorantList::ColorantIds CSubtractiveColorModel::GetEcgColorants() const
{
	ColorantIds ecgColorants;

	for (const ColorantInfo& colorant : m_colorants){
		if (colorant.usage == icmm::CU_ECG){
			ecgColorants.push_back(colorant.id);
		}
	}

	return ecgColorants;
}


IColorantList::ColorantIds CSubtractiveColorModel::GetSpotColorants() const
{
	ColorantIds spotColorants;

	for (const ColorantInfo& colorant : m_colorants){
		if (colorant.usage == icmm::CU_SPOT){
			spotColorants.push_back(colorant.id);
		}
	}

	return spotColorants;
}


// reimplemented ISubtractiveColorModel

std::unique_ptr<ISubtractiveColorModel> CSubtractiveColorModel::CreateSubspaceModel(const QStringList& colorantIds) const
{
	return CreateSubspaceModelFrom(*this, colorantIds);
}


// reimplemented (icmm::IColorantList)

IColorantList::ColorantIds CSubtractiveColorModel::GetColorantIds() const
{
	ColorantIds retVal;

	for (const ColorantInfo& colorant : m_colorants){
		retVal.push_back(colorant.id);
	}

	return retVal;
}


icmm::ColorantUsage CSubtractiveColorModel::GetColorantUsage(const ColorantId& colorantId) const
{
	for (const ColorantInfo& colorant : m_colorants){
		if (colorantId == colorant.id){
			return colorant.usage;
		}
	}

	return CU_NONE;
}


// reimplemented (iser::ISerializable)

bool CSubtractiveColorModel::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	int colorantCount = m_colorants.size();

	const iser::CArchiveTag colorantListTag("Colorants", "List of colorants", iser::CArchiveTag::TT_MULTIPLE);
	const iser::CArchiveTag colorantInfoTag("ColorantInfo", "Single colorant description", iser::CArchiveTag::TT_GROUP, &colorantListTag);

	retVal = retVal && archive.BeginMultiTag(colorantListTag, colorantInfoTag, colorantCount);
	if (!retVal){
		return false;
	}

	istd::CChangeNotifier changeNotifier(archive.IsStoring() ? nullptr : this, &istd::IChangeable::GetAllChanges());

	if (!archive.IsStoring()){
		m_colorants.resize(colorantCount);
	}

	for (int i = 0; i < colorantCount; ++i){
		retVal = retVal && archive.BeginTag(colorantInfoTag);
		retVal = retVal && SerializeColorantInfo(archive, m_colorants[i], &colorantInfoTag);
		retVal = retVal && archive.EndTag(colorantInfoTag);
	}

	retVal = retVal && archive.EndTag(colorantListTag);

	const iser::IVersionInfo& versionInfo = archive.GetVersionInfo();
	quint32 frameworkVersion = 0;
	if (!versionInfo.GetVersionNumber(iser::IVersionInfo::AcfVersionId, frameworkVersion) || (frameworkVersion >= 5436)){
		const iser::CArchiveTag previewSpecTag("PreviewSpec", "Colorant preview specification", iser::CArchiveTag::TT_GROUP);
		retVal = retVal && archive.BeginTag(previewSpecTag);
		retVal = retVal && m_previewSpec.Serialize(archive);
		retVal = retVal && archive.EndTag(previewSpecTag);

		auto SerializeKey = [](iser::IArchive& archive, ColorantId& key){
			return archive.Process(key);
		};

		const iser::CArchiveTag colorantPreviewMapTag("ColorantPreviews", "Previews of the model colorants", iser::CArchiveTag::TT_GROUP);
		retVal = retVal && archive.BeginTag(colorantPreviewMapTag);
		retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeAssociativeObjectContainer<ColorantPreviewMap, ColorantId>(archive, m_colorantPreviewMap, SerializeKey, "Colorants", "Colorant", "ColorantId", "Lab");
		retVal = retVal && archive.EndTag(colorantPreviewMapTag);
	}

	return retVal;
}


// reimplemented (istd::IChangeable)

int CSubtractiveColorModel::GetSupportedOperations() const
{
	return SO_CLONE | SO_COPY | SO_RESET;
}


bool CSubtractiveColorModel::CopyFrom(const istd::IChangeable& object, CompatibilityMode /*mode*/)
{
	const CSubtractiveColorModel* sourceModelPtr = dynamic_cast<const CSubtractiveColorModel*>(&object);
	if (sourceModelPtr != nullptr){
		istd::CChangeNotifier changeNotifier(this, &istd::IChangeable::GetAllChanges());

		bool retVal = m_colorants.CopyFrom(sourceModelPtr->m_colorants);
		retVal = retVal && m_previewSpec.CopyFrom(sourceModelPtr->m_previewSpec);

		if (retVal){
			m_colorantPreviewMap = sourceModelPtr->m_colorantPreviewMap;
		}

		return retVal;
	}

	return false;
}


istd::IChangeableUniquePtr CSubtractiveColorModel::CloneMe(CompatibilityMode /*mode*/) const
{
	return new CSubtractiveColorModel(*this);
}


// protected static methods

icmm::ColorantUsage CSubtractiveColorModel::GetDefaultUsageFromColorantName(const ColorantId& colorantId)
{
	if (colorantId == icmm::CCmykColorModel::GetCyan()){
		return icmm::CU_CYAN;
	}
	else if (colorantId == icmm::CCmykColorModel::GetMagenta()){
		return icmm::CU_MAGENTA;
	}
	else if (colorantId == icmm::CCmykColorModel::GetYellow()){
		return icmm::CU_YELLOW;
	}
	else if (colorantId == icmm::CCmykColorModel::GetBlack()){
		return icmm::CU_BLACK;
	}
	else if (colorantId == GetEcgOrange()){
		return icmm::CU_ECG;
	}
	else if (colorantId == GetEcgGreen()){
		return icmm::CU_ECG;
	}
	else if (colorantId == GetEcgViolet()){
		return icmm::CU_ECG;
	}
	else if (colorantId == GetEcgRed()){
		return icmm::CU_ECG;
	}
	else if (colorantId == GetEcgBlue()){
		return icmm::CU_ECG;
	}

	return CU_SPOT;
}


icmm::IColorantList::ColorantId CSubtractiveColorModel::GetEcgGreen()
{
	return "Green";
}


icmm::IColorantList::ColorantId CSubtractiveColorModel::GetEcgOrange()
{
	return "Orange";
}


icmm::IColorantList::ColorantId CSubtractiveColorModel::GetEcgViolet()
{
	return "Violet";
}


icmm::IColorantList::ColorantId CSubtractiveColorModel::GetEcgRed()
{
	return "Red";
}


icmm::IColorantList::ColorantId CSubtractiveColorModel::GetEcgBlue()
{
	return "Blue";
}


int CSubtractiveColorModel::FindColorantIndex(const ColorantId& colorantId) const
{
	for (int index = 0; index < m_colorants.size(); ++index){
		if (m_colorants[index].id == colorantId){
			return index;
		}
	}

	return -1;
}


bool CSubtractiveColorModel::SerializeColorantInfo(
			iser::IArchive& archive,
			ColorantInfo& colorantInfo,
			const iser::CArchiveTag* parentTagPtr) const
{
	bool retVal = true;

	const iser::CArchiveTag colorantIdTag("ColorantId", "ID of the colorant", iser::CArchiveTag::TT_LEAF, parentTagPtr);
	retVal = retVal && archive.BeginTag(colorantIdTag);
	retVal = retVal && archive.Process(colorantInfo.id);
	retVal = retVal && archive.EndTag(colorantIdTag);

	const iser::CArchiveTag usageTag("Usage", "Usage of the colorant", iser::CArchiveTag::TT_LEAF, parentTagPtr);
	retVal = retVal && archive.BeginTag(usageTag);
	retVal = retVal && I_SERIALIZE_ENUM(ColorantUsage, archive, colorantInfo.usage);
	retVal = retVal && archive.EndTag(usageTag);

	return retVal;
}

// static methods

std::unique_ptr<ISubtractiveColorModel> CSubtractiveColorModel::CreateSubspaceModelFrom(const CSubtractiveColorModelBase& model, const QStringList& colorantIds) 
{
	auto subModel = std::make_unique<imod::TModelWrap<CSubtractiveColorModel>>();
	subModel->SetPreviewSpec(model.GetPreviewSpec());

	auto modelIds = model.GetColorantIds();

	for (const auto& id : colorantIds){
		if (!modelIds.contains(id)){
			return nullptr;
		}
		if (!subModel->InsertColorant(id, model.GetColorantUsage(id))){
			return nullptr;
		}
		icmm::CCieLabColor cieLab(nullptr);
		if (model.GetColorantVisualInfo(id, cieLab)){
			subModel->SetColorantPreview(id, cieLab.GetLab());
		}
	}

	return subModel;
}


} // namespace icmm


