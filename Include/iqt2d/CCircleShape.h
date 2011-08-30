#ifndef iqt2d_CCircleShape_included
#define iqt2d_CCircleShape_included


#include "i2d/CCircle.h"

#include "iqt2d/TObjectShapeBase.h"
#include "iqt2d/CGripShape.h"


namespace iqt2d
{


class CCircleShape: public TObjectShapeBase<QGraphicsEllipseItem, i2d::CCircle>
{
	Q_OBJECT

public:
	typedef TObjectShapeBase<QGraphicsEllipseItem, i2d::CCircle> BaseClass;

	CCircleShape(bool isEditable = false, const ISceneProvider* providerPtr = NULL);

protected Q_SLOTS:
	virtual void OnGripPositionChanged(const i2d::CVector2d& point);

private:
	// reimplemented (iqt2d::TObjectShapeBase)
	void UpdateGraphicsItem(const i2d::CCircle& circle);

private:
	CGripShape m_leftGrip;
	CGripShape m_rightGrip;
	CGripShape m_topGrip;
	CGripShape m_bottomGrip;
};


} // namespace iqt2d


#endif // !iqt2d_CCircleShape_included


