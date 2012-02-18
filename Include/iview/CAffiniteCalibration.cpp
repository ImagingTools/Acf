#include "iview/CAffiniteCalibration.h"


#include "i2d/CLine2d.h"


namespace iview
{


CAffiniteCalibration::CAffiniteCalibration()
{
	Reset();
}


const i2d::CAffine2d& CAffiniteCalibration::GetLogToViewTransform() const
{
	return m_logToViewTransform;
}


void CAffiniteCalibration::GetLogToViewTransform(i2d::CAffine2d& result) const
{
	result = m_logToViewTransform;
}


i2d::CAffine2d CAffiniteCalibration::GetViewToLogTransform() const
{
	return m_logToViewTransform.GetInverted();
}


void CAffiniteCalibration::GetViewToLogTransform(i2d::CAffine2d& result) const
{
	result = m_logToViewTransform.GetInverted();
}


void CAffiniteCalibration::Reset()
{
	m_logToViewTransform.Reset();
}


void CAffiniteCalibration::Reset(const i2d::CVector2d& position, double angle, double scale)
{
	m_logToViewTransform.Reset(position, angle, scale);
}


void CAffiniteCalibration::SetTransform(const i2d::CAffine2d& transform)
{
	m_logToViewTransform = transform;
}


i2d::CVector2d CAffiniteCalibration::GetCenter() const
{
	return m_logToViewTransform.GetTranslation();
}


double CAffiniteCalibration::GetAngle() const
{
	return m_logToViewTransform.GetDeformMatrix().GetApproxAngle();
}


double CAffiniteCalibration::GetScaleFactor() const
{
	return m_logToViewTransform.GetDeformMatrix().GetApproxScale();
}


void CAffiniteCalibration::Apply(const iview::IIsomorphicCalibration& calib)
{
	m_logToViewTransform.Apply(ExtractTransform(i2d::CVector2d(0, 0), calib));
}


void CAffiniteCalibration::ApplyLeft(const iview::IIsomorphicCalibration& calib)
{
	m_logToViewTransform.ApplyLeft(ExtractTransform(i2d::CVector2d(0, 0), calib));
}


CAffiniteCalibration CAffiniteCalibration::GetApply(const iview::IIsomorphicCalibration& calib)
{
	i2d::CAffine2d localTrans;
	m_logToViewTransform.GetApply(ExtractTransform(i2d::CVector2d(0, 0), calib), localTrans);

	CAffiniteCalibration result;
	result.SetTransform(localTrans);

	return result;
}


CAffiniteCalibration CAffiniteCalibration::GetApplyLeft(const iview::IIsomorphicCalibration& calib)
{
	i2d::CAffine2d inputTrans = ExtractTransform(i2d::CVector2d(0, 0), calib);

	i2d::CAffine2d localTrans;
	inputTrans.GetApply(m_logToViewTransform, localTrans);

	CAffiniteCalibration result;
	result.SetTransform(localTrans);

	return result;
}


// reimplemented (iview::ICalibration)

ICalibration::CalcStatus CAffiniteCalibration::GetLogLength(const i2d::CLine2d& line, double& result) const
{
	i2d::CVector2d log1;
	i2d::CVector2d log2;

	CalcStatus stat1;
	CalcStatus stat2;

	if (			((stat1 = GetApplyToLog(line.GetPoint1(), log1)) != CS_FAILED) &&
					((stat2 = GetApplyToLog(line.GetPoint2(), log2)) != CS_FAILED)){
		result = log1.GetDistance(log2);

		if ((stat1 == CS_OK) && (stat2 == CS_OK)){
			return CS_OK;
		}
		else{
			return CS_INVALID;
		}
	}
	else{
		return CS_FAILED;
	}
}


ICalibration::CalcStatus CAffiniteCalibration::GetLogDeform(const i2d::CVector2d& /*logPosition*/, i2d::CMatrix2d& result) const
{
	result = m_logToViewTransform.GetDeformMatrix();

	return CS_OK;
}


ICalibration::CalcStatus CAffiniteCalibration::GetViewDeform(const i2d::CVector2d& /*viewPosition*/, i2d::CMatrix2d& result) const
{
	i2d::CMatrix2d localDeform = m_logToViewTransform.GetDeformMatrix();
	if (localDeform.GetInverted(result)){
		return CS_OK;
	}
	else {
		return CS_FAILED;
	}
}


// reimplemented (iview::IIsomorphicCalibration)

ICalibration::CalcStatus CAffiniteCalibration::GetApplyToLog(const i2d::CVector2d& viewPosition, i2d::CVector2d& result) const
{
	if (m_logToViewTransform.GetInvertedApply(viewPosition, result)){
		return CS_OK;
	}
	else{
		return CS_FAILED;
	}
}


ICalibration::CalcStatus CAffiniteCalibration::GetApplyToView(const i2d::CVector2d& logPosition, i2d::CVector2d& result) const
{
	m_logToViewTransform.GetApply(logPosition, result);

	return CS_OK;
}


// reimplemented (iser::ISerializable)

bool CAffiniteCalibration::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag transformTag("Transform", "Transformation Info");
	bool retVal = archive.BeginTag(transformTag);
	retVal = retVal && m_logToViewTransform.Serialize(archive);
	retVal = retVal && archive.EndTag(transformTag);

	return retVal;
}


// protected static methods

i2d::CAffine2d CAffiniteCalibration::ExtractTransform(const i2d::CVector2d& position, const iview::IIsomorphicCalibration& calibration)
{
	i2d::CAffine2d outputTransformation;

	calibration.GetLogDeform(position, outputTransformation.GetDeformMatrixRef());
	calibration.GetApplyToView(position, outputTransformation.GetTranslationRef());

	return outputTransformation;
}


}