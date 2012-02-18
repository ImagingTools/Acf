#include "iview/CScaleCalibration.h"


// STL includes
#include <math.h>


// ACF includes
#include "i2d/CLine2d.h"

#include "iview/IVisualCalibrationInfo.h"
#include "iview/CCalibratedViewBase.h"


namespace iview
{


CScaleCalibration::CScaleCalibration()
{
	Reset();
}


void CScaleCalibration::Reset()
{
	m_viewCenter.Reset();

	m_scaleFactor = 1.0;
}


i2d::CAffine2d CScaleCalibration::GetLogToViewTransform() const
{
	i2d::CAffine2d result;

	GetLogToViewTransform(result);

	return result;
}


void CScaleCalibration::GetLogToViewTransform(i2d::CAffine2d& result) const
{
	result.Reset(m_viewCenter, 0, m_scaleFactor);
}


// reimplemented (iview::IIsomorphicCalibration)

ICalibration::CalcStatus CScaleCalibration::GetApplyToLog(const i2d::CVector2d& viewPosition, i2d::CVector2d& result) const
{
	if (::fabs(m_scaleFactor) > I_EPSILON){
		result = (viewPosition - m_viewCenter) / m_scaleFactor;

		return CS_OK;
	}
	else{
		return CS_FAILED;
	}
}


ICalibration::CalcStatus CScaleCalibration::GetApplyToView(const i2d::CVector2d& logPosition, i2d::CVector2d& result) const
{
	result = logPosition * m_scaleFactor + m_viewCenter;

	return CS_OK;
}


// reimplemented (iview::ICalibration)

ICalibration::CalcStatus CScaleCalibration::GetLogLength(const i2d::CLine2d& line, double& result) const
{
	if (::fabs(m_scaleFactor) > I_EPSILON){
		result = line.GetLength() / m_scaleFactor;

		return CS_OK;
	}
	else{
		return CS_FAILED;
	}
}


ICalibration::CalcStatus CScaleCalibration::GetLogDeform(const i2d::CVector2d& /*logPosition*/, i2d::CMatrix2d& result) const
{
	result.Reset(0, m_scaleFactor);

	return CS_OK;
}


ICalibration::CalcStatus CScaleCalibration::GetViewDeform(const i2d::CVector2d& /*viewPosition*/, i2d::CMatrix2d& result) const
{
	if (::fabs(m_scaleFactor) > I_EPSILON){
		result.Reset(0, 1.0 / m_scaleFactor);

		return CS_OK;
	}
	else{
		return CS_FAILED;
	}
}


// reimplemented (iser::ISerializable)

bool CScaleCalibration::Serialize(iser::IArchive& archive)
{
	bool result = m_viewCenter.Serialize(archive);
	result = result && archive.Process(m_scaleFactor);

	return result;
}


} // namespace iview



