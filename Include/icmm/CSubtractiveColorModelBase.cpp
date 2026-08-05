// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icmm/CSubtractiveColorModelBase.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <imath/CGeneralUnitInfo.h>
#include <iser/CPrimitiveTypesSerializer.h>


namespace icmm
{


// public methods

CSubtractiveColorModelBase::CSubtractiveColorModelBase()
	:m_previewSpec(ObserverType::TwoDegree, std::make_shared<CIlluminant>(), AstmTableType::Unknown)
{
}


const icmm::CTristimulusSpecification& CSubtractiveColorModelBase::GetPreviewSpec() const
{
	return m_previewSpec;
}


void CSubtractiveColorModelBase::SetPreviewSpec(const ITristimulusSpecification& previewSpec)
{
	istd::CChangeNotifier changeNotifier(this);

	m_previewSpec = icmm::CTristimulusSpecification(previewSpec);
}


void CSubtractiveColorModelBase::SetColorantPreview(const ColorantId& colorantId, const icmm::CLab& preview)
{
	istd::CChangeNotifier changeNotifier(this);

	m_colorantPreviewMap[colorantId] = preview;
}


// reimplemented (icmm::IColorModel)

IColorModel::ModelType CSubtractiveColorModelBase::GetModelType() const
{
	return MT_COLORANTS;
}


IColorModel::ModelClass CSubtractiveColorModelBase::GetModelClass() const
{
	return MC_DEVICE_DEPENDENT;
}


IColorModel::ColorSpaceClass CSubtractiveColorModelBase::GetColorSpaceClass() const
{
	return CSC_SUBSTRACTIVE;
}


int CSubtractiveColorModelBase::GetColorSpaceDimensionality() const
{
	return GetColorantIds().count();
}


const imath::IUnitInfo* CSubtractiveColorModelBase::GetColorSpaceComponentInfo(int /*componentIndex*/) const
{
	static const imath::CGeneralUnitInfo unitInfoBase(imath::IUnitInfo::UnitType::UT_RELATIVE, "", 100.0, istd::CRange(0, 1.0));

	return &unitInfoBase;
}


QString CSubtractiveColorModelBase::GetColorSpaceComponentName(int componentIndex) const
{
	ColorantIds colorantIds = GetColorantIds();

	Q_ASSERT(componentIndex < colorantIds.count());
	Q_ASSERT(componentIndex >= 0);

	return colorantIds[componentIndex];
}


const icmm::IColorTransformation* CSubtractiveColorModelBase::CreateColorTranformation(
	const IColorModel& /*otherColorModel*/,
	const QByteArray& /*transformationId*/) const
{
	return nullptr;
}


bool CSubtractiveColorModelBase::GetColorantVisualInfo(const ColorantId& colorantId, icmm::ICieLabColor& preview) const
{
	if (m_colorantPreviewMap.contains(colorantId)){
		return preview.Initialize(m_colorantPreviewMap[colorantId], m_previewSpec);
	}

	return false;
}


// reimplemented (istd::IChangeable)

int CSubtractiveColorModelBase::GetSupportedOperations() const
{
	return SO_CLONE | SO_COMPARE | SO_COPY;
}


bool CSubtractiveColorModelBase::CopyFrom(const IChangeable& object, CompatibilityMode /*mode*/)
{
	const CSubtractiveColorModelBase* objectPtr = dynamic_cast<const CSubtractiveColorModelBase*>(&object);
	if (objectPtr != nullptr){
		istd::CChangeNotifier notifier(this);

		m_previewSpec = objectPtr->m_previewSpec;
		m_colorantPreviewMap = objectPtr->m_colorantPreviewMap;

		return true;
	}

	return false;
}


} // namespace icmm


