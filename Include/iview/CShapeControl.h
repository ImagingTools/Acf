#ifndef iview_CShapeControl_included
#define iview_CShapeControl_included


#include "iview/CScreenTransform.h"

#include "iview/ILogicalCoords.h"
#include "iview/IDisplay.h"


namespace iview
{


class CShapeControl: public ILogicalCoords
{
public:
	CShapeControl();

	/**
		Describe how shape tranformation is interpreted.
	*/
	enum ShapeTransformMode
	{
		/**
			Shape transformation will be ignored, view transformation will be used.
		*/
		STM_VIEW,
		
		/**
			View transformation will be ignored, shape transformation will be used.
		*/
		STM_SHAPE,
		
		/**
			Both transformation will be used, shape transformation will be used as local transformation.
		*/
		STM_COMBINE
	};

	/**
		Get a transform from logical units to view units.
	*/
	virtual const i2d::CAffine2d& GetShapeTransform() const;
	
	/**
		Set a transform from logical units to view units.
	*/
	virtual void SetShapeTransform(const i2d::CAffine2d& transform);
	
	/**
		Check if this shape is visible.
	*/
	virtual bool IsVisible() const;
	
	/**
		Make this shape visible.
	*/
	virtual void SetVisible(bool state = true);
	
	/**
		Get shape transformation mode.
		This mode describes which transformation will be used to display shape.
	*/
	ShapeTransformMode GetTransformMode() const;
	
	/**
		Set shape transformation mode.
		This mode describes which transformation will be used to display shape.
	*/
	void SetTransformMode(ShapeTransformMode mode);

	// reimplemented (iview::ILogicalCoords)
	virtual const i2d::CAffine2d& GetLogToViewTransform() const;
	virtual const iview::CScreenTransform& GetLogToScreenTransform() const;

protected:
	void InvalidateTransforms();
	bool AreTransformsValid() const;

	/**
		Calculate internal transformations.
		\sa	GetLogToViewTransform(), GetLogToScreenTransform()
		It can be called only when display is connected \sa IsDisplayConnected().
	*/
	void CalcTransforms() const;

	// abstract methods
	
	/**
		Get a transform from view units to screen units
		It can be called only when display is connected \sa IsDisplayConnected().
	*/
	virtual const iview::CScreenTransform& GetViewToScreenTransform() const = 0;
	
	/**
		Force this shape to repaint by next image update.
	*/
	virtual void Invalidate(int changeFlags = 0) = 0;
	
	/**
		Get access to display object.
	*/
	virtual IDisplay* GetDisplayPtr() const = 0;

private:
	i2d::CAffine2d m_shapeTransform;
	ShapeTransformMode m_shapeTransformMode;
	bool m_isVisible;
	mutable i2d::CAffine2d m_logToViewTransform;
	mutable iview::CScreenTransform m_logToScreenTransform;
	mutable bool m_areTransformsValid;
};


inline const i2d::CAffine2d& CShapeControl::GetShapeTransform() const
{
	return m_shapeTransform;
}


inline void CShapeControl::SetShapeTransform(const i2d::CAffine2d& transform)
{
	m_shapeTransform = transform;
	if (m_shapeTransformMode != STM_VIEW){
		InvalidateTransforms();
		Invalidate(CS_CONSOLE);
	}
}


inline CShapeControl::ShapeTransformMode CShapeControl::GetTransformMode() const
{
	return m_shapeTransformMode;
}


inline bool CShapeControl::IsVisible() const
{
	return m_isVisible;
}


inline void CShapeControl::SetVisible(bool state)
{
	if (m_isVisible != state){
		m_isVisible = state;
		Invalidate(CS_CONSOLE);
	}
}


// reimplemented (iview::ILogicalCoords)

inline const i2d::CAffine2d& CShapeControl::GetLogToViewTransform() const
{
	if (!AreTransformsValid()){
		CalcTransforms();
	}

	return m_logToViewTransform;
}


inline const iview::CScreenTransform& CShapeControl::GetLogToScreenTransform() const
{
	if (!AreTransformsValid()){
		CalcTransforms();
	}

	return m_logToScreenTransform;
}


// protected inline methods

inline void CShapeControl::InvalidateTransforms()
{
	m_areTransformsValid = false;
}


inline bool CShapeControl::AreTransformsValid() const
{
	return m_areTransformsValid;
}


} // namespace iview


#endif // !iview_CShapeControl_included


