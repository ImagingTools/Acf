#pragma once


// ACF includes
#include <icmm/IColorModel.h>


namespace icmm
{


class CCieLabColorModel: virtual public icmm::IColorModel
{
public:
	StandardIlluminant GetIlluminant() const;
	ObserverType GetObserverType() const;

	CCieLabColorModel(ObserverType observer = OT_2, StandardIlluminant illuminant = SI_D65);

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

private:
	StandardIlluminant m_illuminant;
	ObserverType m_observerType;
};


} // namespace icmm


