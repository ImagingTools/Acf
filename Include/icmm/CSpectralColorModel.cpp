#include <icmm/CSpectralColorModel.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <icmm/CSpectrumInfo.h>


namespace icmm
{


// public methods

CSpectralColorModel::CSpectralColorModel(ISpectralColorSpecification::ConstSpectralColorSpecPtr spec)
	:m_spec(spec)
{
}


// reimplemented (icmm::IColorModel)

IColorModel::ModelType CSpectralColorModel::GetModelType() const
{
	return MI_SPECTRAL;
}


IColorModel::ModelClass CSpectralColorModel::GetModelClass() const
{
	return MC_DEVICE_INDEPENDENT;
}


IColorModel::ColorSpaceClass CSpectralColorModel::GetColorSpaceClass() const
{
	return CSC_PERCEPTUAL;
}


int CSpectralColorModel::GetColorSpaceDimensionality() const
{
	if (m_spec == nullptr) {
		return 0;
	}

	int componentCount = 0;

	const ISpectrumInfo* infoPtr = m_spec->GetSpectrumInfo();
	if (infoPtr != nullptr) {
		int step = infoPtr->GetStep();
		if (step > 0) {
			istd::CIntRange range = infoPtr->GetSpectralRange();

			componentCount = 1 + range.GetLength() / step;
		}
	}

	return componentCount;
}


const imath::IUnitInfo* CSpectralColorModel::GetColorSpaceComponentInfo(int /*componentIndex*/) const
{
	return nullptr;
}


QString CSpectralColorModel::GetColorSpaceComponentName(int /*componentIndex*/) const
{
	return QString();
}


const icmm::IColorTransformation* CSpectralColorModel::CreateColorTranformation(
	const IColorModel& /*otherColorModel*/,
	const QByteArray& /*transformationId*/) const
{
	return nullptr;
}


IColorSpecification::ConstColorSpecPtr CSpectralColorModel::GetSpecification() const
{
	return m_spec;
}


// reimplemented (istd::IChangeable)

int CSpectralColorModel::GetSupportedOperations() const
{
	return SO_CLONE | SO_COMPARE | SO_COPY;
}


bool CSpectralColorModel::CopyFrom(const IChangeable& object, CompatibilityMode mode)
{
	const CSpectralColorModel* objectPtr = dynamic_cast<const CSpectralColorModel*>(&object);
	if (objectPtr != nullptr){
		istd::CChangeNotifier notifier(this);

		// Only copy external resources if mode is CM_WITH_REFS
		if (mode == CM_WITH_REFS){
			m_spec = objectPtr->m_spec;
		}

		return true;
	}

	return false;
}


istd::IChangeableUniquePtr CSpectralColorModel::CloneMe(CompatibilityMode mode) const
{
	istd::IChangeableUniquePtr clonePtr(new CSpectralColorModel());
	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr;
	}

	return istd::IChangeableUniquePtr();
}


} // namespace icmm


