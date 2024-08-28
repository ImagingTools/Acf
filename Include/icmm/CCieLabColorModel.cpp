#include <icmm/CCieLabColorModel.h>


namespace icmm
{


// public methods

CCieLabColorModel::CCieLabColorModel(ObserverType observer, StandardIlluminant illuminant)
	:m_observerType(observer),
	m_illuminant(illuminant)
{
}


StandardIlluminant CCieLabColorModel::GetIlluminant() const
{
	return m_illuminant;
}


ObserverType CCieLabColorModel::GetObserverType() const
{
	return m_observerType;
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


const imath::IUnitInfo* CCieLabColorModel::GetColorSpaceComponentInfo(int /*componentIndex*/) const
{
	return nullptr;
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


} // namespace icmm


