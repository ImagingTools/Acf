#ifndef iview_CInteractiveShapeBase_included
#define iview_CInteractiveShapeBase_included


// ACF includes
#include "imod/IModel.h"

#include "iview/IInteractiveShape.h"
#include "iview/IDraggable.h"
#include "iview/ISelectable.h"
#include "iview/CShapeBase.h"

// Qt includes
#include <QtGui/QPen>


namespace iview
{


class CInteractiveShapeBase:
			public CShapeBase,
			virtual public IInteractiveShape,
			virtual public IDraggable
{
public:
	typedef CShapeBase BaseClass;

	CInteractiveShapeBase();
	CInteractiveShapeBase(const CInteractiveShapeBase& shape);

	virtual ~CInteractiveShapeBase();

	virtual bool IsEditablePosition() const;
	virtual void SetEditablePosition(bool state = true);

	// reimplemented (iview::IInteractiveShape)
	virtual bool IsSelected() const;
	virtual void SetSelected(bool selectFlag = true);

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag);
	virtual bool OnMouseMove(istd::CIndex2d position);

	// reimplemented (iview::IDraggable)
	virtual bool IsDraggable() const;
	virtual void BeginDrag(const i2d::CVector2d& position);
	virtual void SetDragPosition(const i2d::CVector2d& position);
	virtual void EndDrag();

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);

protected:
	void BeginModelChanges();
	void EndModelChanges();
	void UpdateModelChanges();

	/**
		Get actual keys state.
		Note, that display must be connected to use this method.
	*/
	int GetKeysState() const;

	/**
		Get actual edit mode.
		Note, that display must be connected to use this method.
	*/
	int GetEditMode() const;

	/**
		Draw text on a given position.
	*/
	void DrawText(QPainter& drawContext, istd::CIndex2d point, const QString& text) const;

	/** 
		Draw an orientation marker consisting of two triangles (black for 
		outside and white for inside)
	 */
	virtual void DrawOrientationMarker(
				QPainter& drawContext,
				const QPen& rightPen,
				const QBrush& rightBrush,
				const QPen& leftPen,
				const QBrush& leftBrush,
				const i2d::CLine2d& segmentLine,
				double scale) const;

	// reimplemented (iview::CShapeBase)
	virtual int GetDisplayChangesMask();

	// abstract methods
	virtual void BeginLogDrag(const i2d::CVector2d& reference) = 0;
	virtual void SetLogDragPosition(const i2d::CVector2d& position) = 0;

private:
	bool m_isSelected;
	bool m_isEditablePosition;
};


// inline methods

inline bool CInteractiveShapeBase::IsEditablePosition() const
{
	return m_isEditablePosition;
}


inline void CInteractiveShapeBase::SetEditablePosition(bool state)
{
	m_isEditablePosition = state;
}


// reimplemented (iview::IDraggable)

inline bool CInteractiveShapeBase::IsDraggable() const
{
	return m_isEditablePosition;
}


inline void CInteractiveShapeBase::BeginDrag(const i2d::CVector2d& position)
{
	const i2d::CAffine2d& invert = GetLogToViewTransform().GetInverted();

	BeginLogDrag(invert.GetApply(position));
}


inline void CInteractiveShapeBase::SetDragPosition(const i2d::CVector2d& position)
{
	const i2d::CAffine2d& invert = GetLogToViewTransform().GetInverted();

	SetLogDragPosition(invert.GetApply(position));
}


inline void CInteractiveShapeBase::EndDrag()
{
}


// protected methods

inline int CInteractiveShapeBase::GetKeysState() const
{
	ISelectable* selectablePtr = dynamic_cast<ISelectable*>(GetDisplayPtr());
	if (selectablePtr != NULL){
		return selectablePtr->GetKeysState();
	}
	else{
		return 0;
	}
}



inline int CInteractiveShapeBase::GetEditMode() const
{
	ISelectable* selectablePtr = dynamic_cast<ISelectable*>(GetDisplayPtr());
	if (selectablePtr != NULL){
		return selectablePtr->GetEditMode();
	}
	else{
		return ISelectable::EM_NONE;
	}
}


// reimplemented (iview::CShapeBase)

inline int CInteractiveShapeBase::GetDisplayChangesMask()
{
	return BaseClass::GetDisplayChangesMask() | CF_EDIT_MODE;
}


} // namespace iview


#endif // !iview_CInteractiveShapeBase_included


