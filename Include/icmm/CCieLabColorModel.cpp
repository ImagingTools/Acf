// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icmm/CCieLabColorModel.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <imath/CGeneralUnitInfo.h>


namespace icmm
{


// public methods

CCieLabColorModel::CCieLabColorModel(const ITristimulusSpecification& spec)
	:m_spec(spec)
{
}


void CCieLabColorModel::SetTristimulusSpecification(const ITristimulusSpecification& spec)
{
	istd::CChangeNotifier changeNotifier(this);

	m_spec = spec;
}


const ITristimulusSpecification& CCieLabColorModel::GetTristimulusSpecification() const
{
	return m_spec;
}


// reimplemented (icmm::IColorModel)

IColorModel::ModelType CCieLabColorModel::GetModelType() const
{
	return MT_LAB;
}


IColorModel::ModelClass CCieLabColorModel::GetModelClass() const
{
	return MC_DEVICE_INDEPENDENT;
}


IColorModel::ColorSpaceClass CCieLabColorModel::GetColorSpaceClass() const
{
	return CSC_PERCEPTUAL;
}


int CCieLabColorModel::GetColorSpaceDimensionality() const
{
	return 3;
}


const imath::IUnitInfo* CCieLabColorModel::GetColorSpaceComponentInfo(int componentIndex) const
{
	static const std::array<imath::CGeneralUnitInfo, 3> cieLabUnitInfo = {
	imath::CGeneralUnitInfo(imath::IUnitInfo::UnitType::UT_TECHNICAL, "", 1.0, istd::CRange(0.0, 100.0)),
	imath::CGeneralUnitInfo(imath::IUnitInfo::UnitType::UT_TECHNICAL, "", 1.0, istd::CRange(-128.0, 127.0)),
	imath::CGeneralUnitInfo(imath::IUnitInfo::UnitType::UT_TECHNICAL, "", 1.0, istd::CRange(-128.0, 127.0))
	};

	return &cieLabUnitInfo.at(componentIndex);
}


QString CCieLabColorModel::GetColorSpaceComponentName(int componentIndex) const
{
	switch (componentIndex){
	case 0:
		return "L";
	case 1:
		return "a";
	case 2:
		return "b";
	}

	Q_ASSERT(false);

	return QString();
}


const icmm::IColorTransformation* CCieLabColorModel::CreateColorTranformation(
	const IColorModel& /*otherColorModel*/,
	const QByteArray& /*transformationId*/) const
{
	return nullptr;
}


IColorSpecification::ConstColorSpecPtr CCieLabColorModel::GetSpecification() const
{
	return std::make_shared<CTristimulusSpecification>(m_spec);
}


// reimplemented (istd::IChangeable)

int CCieLabColorModel::GetSupportedOperations() const
{
	return SO_CLONE | SO_COMPARE | SO_COPY;
}


bool CCieLabColorModel::CopyFrom(const IChangeable& object, CompatibilityMode mode)
{
	const CCieLabColorModel* objectPtr = dynamic_cast<const CCieLabColorModel*>(&object);
	if (objectPtr != nullptr){
		istd::CChangeNotifier notifier(this);

		m_spec.CopyFrom(objectPtr->m_spec, mode);

		return true;
	}

	return false;
}


istd::IChangeableUniquePtr CCieLabColorModel::CloneMe(CompatibilityMode mode) const
{
	istd::IChangeableUniquePtr clonePtr(new CCieLabColorModel());
	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr;
	}

	return istd::IChangeableUniquePtr();
}


} // namespace icmm


