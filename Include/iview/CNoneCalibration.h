#ifndef iview_CNoneCalibration
#define iview_CNoneCalibration


#include "iview/IIsomorphicCalibration.h"


namespace iview
{


// TODO: Redesign it to ACF transformation concept.
class CNoneCalibration: virtual public IIsomorphicCalibration
{
public:
	// reimplemented (iview::ICalibration)
	virtual CalcStatus GetLogLength(const i2d::CLine2d& line, double& result) const;
	virtual CalcStatus GetLogDeform(const i2d::CVector2d& logPosition, i2d::CMatrix2d& result) const;
	virtual CalcStatus GetViewDeform(const i2d::CVector2d& viewPosition, i2d::CMatrix2d& result) const;

	// reimplemented (iview::IIsomorphicCalibration)
	virtual CalcStatus GetApplyToLog(const i2d::CVector2d& viewPosition, i2d::CVector2d& result) const;
	virtual CalcStatus GetApplyToView(const i2d::CVector2d& logPosition, i2d::CVector2d& result) const;

	// reimplemented (iser::ISerializable)
	bool Serialize(iser::IArchive& archive);

	// static methods
	static const CNoneCalibration& GetInstance();

private:
	// static attributes
	static CNoneCalibration s_defaultInstance;
};


} // namespace iview


#endif // !iview_CNoneCalibration


