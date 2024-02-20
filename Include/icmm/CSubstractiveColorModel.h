#pragma once


// ACF includes
#include <icmm/IColorantList.h>
#include <icmm/IColorModel.h>


namespace icmm
{


class CSubstractiveColorModel: virtual public icmm::IColorModel, virtual public icmm::IColorantList
{
public:
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
};


// public inline methods

// reimplemented (icmm::IColorModel)

inline IColorModel::ModelType CSubstractiveColorModel::GetModelType() const
{
	return MT_COLORANTS;
}


inline IColorModel::ModelClass CSubstractiveColorModel::GetModelClass() const
{
	return MC_DEVICE_DEPENDENT;
}


inline IColorModel::ColorSpaceClass CSubstractiveColorModel::GetColorSpaceClass() const
{
	return CSC_SUBSRACTIVE;
}


inline int CSubstractiveColorModel::GetColorSpaceDimensionality() const
{
	return GetColorantIds().count();
}


inline const imath::IUnitInfo* CSubstractiveColorModel::GetColorSpaceComponentInfo(int /*componentIndex*/) const
{
	return nullptr;
}


inline QString CSubstractiveColorModel::GetColorSpaceComponentName(int componentIndex) const
{
	QByteArrayList colorantIds = GetColorantIds();

	Q_ASSERT(componentIndex < colorantIds.count());
	Q_ASSERT(componentIndex >= 0);

	return GetColorantName(colorantIds[componentIndex]);
}


inline const icmm::IColorTransformation* CSubstractiveColorModel::CreateColorTranformation(
			const IColorModel& /*otherColorModel*/,
			const QByteArray& /*transformationId*/) const
{
	return nullptr;
}


} // namespace icmm


