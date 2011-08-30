#ifndef iqt2d_CLine2dShape_included
#define iqt2d_CLine2dShape_included


#include "i2d/CLine2d.h"

#include "iqt2d/TObjectShapeBase.h"
#include "iqt2d/CGripShape.h"


namespace iqt2d
{


class CLine2dShape: public TObjectShapeBase<QGraphicsLineItem, i2d::CLine2d>
{
	Q_OBJECT
public:
	typedef TObjectShapeBase<QGraphicsLineItem, i2d::CLine2d> BaseClass;

	CLine2dShape(bool isEditable = false, const ISceneProvider* providerPtr = NULL);

protected Q_SLOTS:
	virtual void OnPosition1Changed(const i2d::CVector2d& point);
	virtual void OnPosition2Changed(const i2d::CVector2d& point);

protected:
	// reimplemented (iqt2d::TObjectShapeBase)
	void UpdateGraphicsItem(const i2d::CLine2d& line);

private:
	CGripShape m_pointGrip1;
	CGripShape m_pointGrip2;
};


} // namespace iqt2d


#endif // !iqt2d_CLine2dShape_included


