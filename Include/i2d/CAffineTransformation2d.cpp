#include "i2d/CAffineTransformation2d.h"


namespace i2d
{


// reimplemented (i2d::ITransformation2d)

int CAffineTransformation2d::GetTransformationFlags() const
{
	return TF_FORWARD | TF_INVERTED | TF_INJECTIVE | TF_SURJECTIVE | TF_AFFINE | TF_CONTINUES;
}


bool CAffineTransformation2d::GetDistance(
			const CVector2d& origPos1,
			const CVector2d& origPos2,
			double& result,
			ExactnessMode /*mode*/) const
{
	result = GetApply(origPos1).GetDistance(GetApply(origPos2));

	return true;
}


bool CAffineTransformation2d::GetPositionAt(
			const CVector2d& origPosition,
			CVector2d& result,
			ExactnessMode /*mode*/) const
{
	GetApply(origPosition, result);

	return true;
}


bool CAffineTransformation2d::GetInvPositionAt(
			const CVector2d& transfPosition,
			CVector2d& result,
			ExactnessMode /*mode*/) const
{
	return GetInvertedApply(transfPosition, result);
}


bool CAffineTransformation2d::GetLocalTransform(
			const CVector2d& /*origPosition*/,
			CAffine2d& result,
			ExactnessMode /*mode*/) const
{
	result = *this;

	return true;
}


bool CAffineTransformation2d::GetLocalInvTransform(
			const CVector2d& /*transfPosition*/,
			CAffine2d& result,
			ExactnessMode /*mode*/) const
{
	return GetInverted(result);
}


const ITransformation2d* CAffineTransformation2d::CreateCombinedTransformation(const ITransformation2d& transform) const
{
	const CAffineTransformation2d* affineTransformPtr = dynamic_cast<const CAffineTransformation2d*>(&transform);
	if (affineTransformPtr != NULL){
		CAffineTransformation2d* combinedPtr = new CAffineTransformation2d();

		GetApply(*affineTransformPtr, *combinedPtr);

		return combinedPtr;
	}

	return NULL;
}


// reimplemented (imath::TISurjectFunction)

bool CAffineTransformation2d::GetInvValueAt(const CVector2d& argument, CVector2d& result) const
{
	return GetInvertedApply(argument, result);
}


CVector2d CAffineTransformation2d::GetInvValueAt(const CVector2d& argument) const
{
	return GetInvertedApply(argument);
}


// reimplemented (imath::TIMathFunction)

bool CAffineTransformation2d::GetValueAt(const CVector2d& argument, CVector2d& result) const
{
	GetApply(argument, result);

	return true;
}


CVector2d CAffineTransformation2d::GetValueAt(const CVector2d& argument) const
{
	return GetApply(argument);
}


// reimplemented (iser::ISerializable)

bool CAffineTransformation2d::Serialize(iser::IArchive& archive)
{
	return BaseClass::Serialize(archive);
}


} // namespace i2d


