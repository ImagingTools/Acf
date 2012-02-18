#ifndef iview_CInteractiveTransformableRectangle_included
#define iview_CInteractiveTransformableRectangle_included


// ACF includes
#include "i2d/CParallelogram.h"



#include "iview/CInteractiveShapeBase.h"


namespace iview
{


class CInteractiveTransformableRectangle: public CInteractiveShapeBase
{
public:
	typedef CInteractiveShapeBase BaseClass;
	
	CInteractiveTransformableRectangle();

	/**	Check, if rotation is editable.
	*/
	virtual bool IsEditableRotation();
	virtual void SetEditableRotation(bool state = true);
	/**	Check, if width is editable.
	*/
	virtual bool IsEditableWidth();
	virtual void SetEditableWidth(bool state = true);
	/**	Check, if height is editable.
	*/
	virtual bool IsEditableHeight();
	virtual void SetEditableHeight(bool state = true);
	/**	Check, if both axis will be scaled proportional.
	*/
	virtual bool IsProportionalScaled() const;
	virtual void SetProportionalScaled(bool state = true);

	// reimplemented (iview::CInteractiveShapeBase, see iview::TShapeBase)
	virtual void InvalidateBoundingBox();

	// reimplemented (iview::IInteractiveShape)
	virtual TouchState IsTouched(istd::CIndex2d position) const;

	// reimplemented (iview::IShape)
	virtual void Draw(QPainter& drawContext) const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag);
	virtual bool OnMouseMove(istd::CIndex2d position);

protected:
	enum EditNode
	{
		EN_NODE1 = 0,
		EN_NODE2,
		EN_NODE3,
		EN_NODE4,
		EN_CENTER,
		EN_MIDDLE12,
		EN_MIDDLE23,
		EN_MIDDLE34,
		EN_MIDDLE41,
		EN_ROTATION1,
		EN_ROTATION2,
		EN_LAST = EN_ROTATION2
	};

	enum MouseMode
	{
		MM_NONE,
		MM_ROTATION1,
		MM_ROTATION2,
		MM_WIDTH1,
		MM_WIDTH2,
		MM_HEIGHT1,
		MM_HEIGHT2,
		MM_SIZE1,
		MM_SIZE2,
		MM_SIZE3,
		MM_SIZE4
	};

	MouseMode m_mouseMode;
	i2d::CVector2d m_referencePosition;

	void CalcNodes(const i2d::CAffine2d& parallTransform) const;
	void ResetNodes() const;
	bool AreNodesValid() const;
	const istd::CIndex2d* GetNodes() const;

	virtual void EnsureValidNodes() const;
	i2d::CAffine2d CalcMoveTransform(i2d::CVector2d position, const i2d::CAffine2d& transform);

	virtual bool IsTickerTouched(istd::CIndex2d position) const;
	virtual bool IsFigureTouched(istd::CIndex2d position) const;
	virtual void DrawTickers(QPainter& drawContext) const;
	virtual void DrawFigure(QPainter& drawContext) const;

	bool IsParallTouched(
					const i2d::CAffine2d& parallTransform,
					istd::CIndex2d position) const;
	i2d::CAffine2d CalcScaleTransform(
					const i2d::CVector2d& center,
					const i2d::CVector2d& isPos,
					const i2d::CVector2d& shouldPos);
	i2d::CAffine2d CalcSizeTransform(
					const i2d::CVector2d& center,
					const i2d::CVector2d& isPos,
					const i2d::CVector2d& shouldPos);
	i2d::CAffine2d CalcRotatedTransform(
					const i2d::CVector2d& center,
					const i2d::CVector2d& isPos,
					const i2d::CVector2d& shouldPos);

	// reimplemented (iview::CInteractiveShapeBase)
	virtual void CalcBoundingBox(i2d::CRect& result) const;
	virtual void BeginLogDrag(const i2d::CVector2d& reference);
	virtual void SetLogDragPosition(const i2d::CVector2d& position);

private:
	bool m_isEditableRotation;
	bool m_isEditableWidth;
	bool m_isEditableHeight;
	bool m_isProportionalScaled;

	mutable istd::CIndex2d m_nodes[EN_LAST + 1];
	mutable bool m_areNodesValid;
};


// inline methods

inline bool CInteractiveTransformableRectangle::IsEditableRotation()
{
	return m_isEditableRotation;
}


inline void CInteractiveTransformableRectangle::SetEditableRotation(bool state)
{
	m_isEditableRotation = state;
}


inline bool CInteractiveTransformableRectangle::IsEditableWidth()
{
	return m_isEditableWidth;
}


inline void CInteractiveTransformableRectangle::SetEditableWidth(bool state)
{
	m_isEditableWidth = state;
}


inline bool CInteractiveTransformableRectangle::IsEditableHeight()
{
	return m_isEditableHeight;
}


inline void CInteractiveTransformableRectangle::SetEditableHeight(bool state)
{
	m_isEditableHeight = state;
}


inline bool CInteractiveTransformableRectangle::IsProportionalScaled() const
{
	return m_isProportionalScaled;
}


inline void CInteractiveTransformableRectangle::SetProportionalScaled(bool state)
{
	m_isProportionalScaled = state;
}


// protected methods

inline bool CInteractiveTransformableRectangle::AreNodesValid() const
{
	return m_areNodesValid;
}


inline const istd::CIndex2d* CInteractiveTransformableRectangle::GetNodes() const
{
	return m_nodes;
}


} // namespace iview


#endif // !iview_CInteractiveTransformableRectangle_included



