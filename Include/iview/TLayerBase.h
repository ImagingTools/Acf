#ifndef iview_TLayerBase_included
#define iview_TLayerBase_included


#include "iview/IShapeView.h"
#include "iview/ILayer.h"


namespace iview
{


/**
	This is internal class to generate common implementation
	for diffrent layers implementations.
	Template paramener must be derivate from iview::ILayer interface.
*/
template <class Layer, class View = IShapeView>
class TLayerBase: public Layer
{
public:
	typedef Layer BaseClass;

	TLayerBase();
	virtual ~TLayerBase();

	View* GetTViewPtr() const;

	// reimplemented (iview::ILayer)
	virtual void OnConnectView(IShapeView* viewPtr);
	virtual void OnDisconnectView(IShapeView* viewPtr);
	virtual IShapeView* GetViewPtr() const;
	virtual bool IsVisible() const;
	virtual void SetVisible(bool state = true);

	// reimplemented (iview::IDisplay)
	virtual IDisplay* GetParentDisplayPtr() const;
	virtual const iview::CScreenTransform& GetTransform() const;
	virtual i2d::CRect GetClientRect() const;
    virtual const IColorShema& GetColorShema() const;
	virtual void OnAreaInvalidated(const i2d::CRect& prevArea, const i2d::CRect& newArea);

private:
	View* m_viewPtr;
	bool m_isVisible;
};


template <class Layer, class View>
inline bool TLayerBase<Layer, View>::IsVisible() const
{
	return m_isVisible;
}


template <class Layer, class View>
inline void TLayerBase<Layer, View>::SetVisible(bool state)
{
	if (m_isVisible != state){
		m_isVisible = state;

		if (m_isVisible){
			// Show.
			OnAreaInvalidated(i2d::CRect::GetEmpty(), GetBoundingBox());
		}
		else{
			// Hide.
			OnAreaInvalidated(GetBoundingBox(), i2d::CRect::GetEmpty());
		}
	}
}


// public methods

template <class Layer, class View>
TLayerBase<Layer, View>::TLayerBase()
{
	m_viewPtr = NULL;
	m_isVisible = true;
}


template <class Layer, class View>
TLayerBase<Layer, View>::~TLayerBase()
{
	if (m_viewPtr != NULL){
		int layerIndex = m_viewPtr->GetLayerIndex(*this);
		I_ASSERT(layerIndex >= 0);

		m_viewPtr->RemoveLayer(layerIndex);
	}
}


template <class Layer, class View>
View* TLayerBase<Layer, View>::GetTViewPtr() const
{
	I_ASSERT(m_viewPtr != NULL);

	return m_viewPtr;
}


// reimplemented (iview::ILayer)

template <class Layer, class View>
void TLayerBase<Layer, View>::OnConnectView(IShapeView* viewPtr)
{
	I_ASSERT(m_viewPtr == NULL);
	I_ASSERT(viewPtr != NULL);

	m_viewPtr = viewPtr;
}


template <class Layer, class View>
void TLayerBase<Layer, View>::OnDisconnectView(IShapeView* I_IF_DEBUG(viewPtr))
{
	I_ASSERT(m_viewPtr == viewPtr);
	I_ASSERT(viewPtr != NULL);

	m_viewPtr = NULL;
}


template <class Layer, class View>
IShapeView* TLayerBase<Layer, View>::GetViewPtr() const
{
	return GetTViewPtr();
}


// reimplemented (iview::IDisplay)

template <class Layer, class View>
IDisplay* TLayerBase<Layer, View>::GetParentDisplayPtr() const
{
	return m_viewPtr;
}


template <class Layer, class View>
const iview::CScreenTransform& TLayerBase<Layer, View>::GetTransform() const
{
	I_ASSERT(m_viewPtr != NULL);

	return m_viewPtr->GetTransform();
}


template <class Layer, class View>
i2d::CRect TLayerBase<Layer, View>::GetClientRect() const
{
	I_ASSERT(m_viewPtr != NULL);

	return m_viewPtr->GetClientRect();
}


template <class Layer, class View>
const IColorShema& TLayerBase<Layer, View>::GetColorShema() const
{
	I_ASSERT(m_viewPtr != NULL);

	return m_viewPtr->GetColorShema();
}


template <class Layer, class View>
void TLayerBase<Layer, View>::OnAreaInvalidated(const i2d::CRect& prevArea, const i2d::CRect& newArea)
{
	I_ASSERT(this->GetBoundingBox().IsInside(newArea));

	if (m_viewPtr != NULL){
		m_viewPtr->OnLayerInvalidated(*this, prevArea, newArea);
	}
}


} // namespace iview


#endif // !iview_TLayerBase_included



