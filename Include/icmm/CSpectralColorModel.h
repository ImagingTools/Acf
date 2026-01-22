// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// ACF includes
#include <icmm/IColorModel.h>
#include <icmm/ISpectralColorSpecification.h>


namespace icmm
{


class CSpectralColorModel: virtual public icmm::IColorModel
{
public:
	CSpectralColorModel(ISpectralColorSpecification::ConstSpectralColorSpecPtr spec = nullptr);

	// reimplemented (icmm::IColorModel)
	virtual ModelType GetModelType() const override;
	virtual ModelClass GetModelClass() const override;
	virtual ColorSpaceClass GetColorSpaceClass() const override;
	virtual int GetColorSpaceDimensionality() const override;
	virtual const imath::IUnitInfo* GetColorSpaceComponentInfo(int componentIndex) const override;
	virtual QString GetColorSpaceComponentName(int componentIndex) const override;
	virtual const icmm::IColorTransformation* CreateColorTranformation(
				const IColorModel& otherColorModel,
				const QByteArray& transformationId = QByteArray()) const override;
	virtual IColorSpecification::ConstColorSpecPtr GetSpecification() const override;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual istd::IChangeableUniquePtr CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;

private:
	std::shared_ptr<const ISpectralColorSpecification> m_spec;
};


} // namespace icmm


