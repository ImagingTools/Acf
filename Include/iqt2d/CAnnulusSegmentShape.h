#ifndef iqt2d_CAnnulusSegmentShape_included
#define iqt2d_CAnnulusSegmentShape_included


#include "iqt2d/CAnnulusShape.h"
#include "iqt2d/CGripShape.h"


namespace iqt2d
{


class CAnnulusSegmentShape: public CAnnulusShape
{
	Q_OBJECT

public:
	typedef CAnnulusShape BaseClass;

	CAnnulusSegmentShape(bool isEditable = false, const ISceneProvider* providerPtr = NULL);

protected slots:
	virtual void OnAngleGripPositionChanged(const i2d::CVector2d& point);

protected:
	// reimplemented (iqt2d::CAnnulusShape)
	virtual void CalcVisualization(const i2d::CAnnulus& annulus);

	// reimplemented (iqt2d::TObjectShapeBase)
	void UpdateGraphicsItem(const i2d::CAnnulus& circle);

private:
	CGripShape m_beginAngleGrip;
	CGripShape m_endAngleGrip;
};


} // namespace iqt2d


#endif // !iqt2d_CAnnulusSegmentShape_included


