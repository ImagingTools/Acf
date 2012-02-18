#include "iview/CNoneCalibration.h"


namespace iview
{


// public methods

// reimplemented (iview::ICalibration)

CNoneCalibration::CalcStatus CNoneCalibration::GetLogLength(const i2d::CLine2d& line, double& result) const
{
	result = line.GetLength();
	
	return CS_OK;
}



CNoneCalibration::CalcStatus CNoneCalibration::GetLogDeform(const i2d::CVector2d& /*logPosition*/, i2d::CMatrix2d& result) const
{
	result = i2d::CMatrix2d::GetIdentity();
	
	return CS_OK;
}



CNoneCalibration::CalcStatus CNoneCalibration::GetViewDeform(const i2d::CVector2d& /*viewPosition*/, i2d::CMatrix2d& result) const
{
	result = i2d::CMatrix2d::GetIdentity();
	
	return CS_OK;
}



// reimplemented (iview::IIsomorphicCalibration)

CNoneCalibration::CalcStatus CNoneCalibration::GetApplyToLog(const i2d::CVector2d& viewPosition, i2d::CVector2d& result) const
{
	result = viewPosition;
	
	return CS_OK;
}



CNoneCalibration::CalcStatus CNoneCalibration::GetApplyToView(const i2d::CVector2d& logPosition, i2d::CVector2d& result) const
{
	result = logPosition;

	return CS_OK;
}



// reimplemented (iser::ISerializable)

bool CNoneCalibration::Serialize(iser::IArchive& /*archive*/)
{
	return true;
}



// static methods

const CNoneCalibration& CNoneCalibration::GetInstance()
{
	return s_defaultInstance;
}


// static attributes

CNoneCalibration CNoneCalibration::s_defaultInstance;


} // namespace iview



