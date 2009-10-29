#ifndef iqt2d_CAnnulusShape_included
#define iqt2d_CAnnulusShape_included


#include "i2d/CAnnulus.h"

#include "iqt2d/TObjectShapeBase.h"
#include "iqt2d/CGripShape.h"


namespace iqt2d
{


class CAnnulusShape: public TObjectShapeBase<QGraphicsPathItem, i2d::CAnnulus>
{
	Q_OBJECT

public:
	typedef TObjectShapeBase<QGraphicsPathItem, i2d::CAnnulus> BaseClass;

	CAnnulusShape(bool isEditable = false, const ISceneProvider* providerPtr = NULL);

protected slots:
	virtual void OnInnerGripPositionChanged(const i2d::CVector2d& point);
	virtual void OnOuterGripPositionChanged(const i2d::CVector2d& point);

protected:
	virtual void CalcVisualization(const i2d::CAnnulus& annulus);

	// reimplemented (iqt2d::TObjectShapeBase)
	void UpdateGraphicsItem(const i2d::CAnnulus& annulus);

private:
	CGripShape m_leftInnerGrip;
	CGripShape m_rightInnerGrip;
	CGripShape m_topInnerGrip;
	CGripShape m_bottomInnerGrip;
	CGripShape m_leftOuterGrip;
	CGripShape m_rightOuterGrip;
	CGripShape m_topOuterGrip;
	CGripShape m_bottomOuterGrip;
};


} // namespace iqt2d


#endif // !iqt2d_CAnnulusShape_included


