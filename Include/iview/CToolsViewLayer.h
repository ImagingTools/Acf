#ifndef iview_CToolsViewLayer_included
#define iview_CToolsViewLayer_included


// ACF includes
#include <iview/ISelectableLayer.h>
#include <iview/CViewLayer.h>


namespace iview
{


class CToolsViewLayer:
			public CViewLayer,
			virtual public ISelectableLayer
{
public:
	typedef CViewLayer BaseClass;

	// reimplemented (iview::ISelectableLayer)
	virtual bool ConnectInteractiveShape(IInteractiveShape* shapePtr);
	virtual int GetUnselectedShapesCount() const;
	virtual void DrawFocusedShape(QPainter& drawContext);
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag);
	virtual bool OnFocusedMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag);
	virtual bool OnFocusedMouseMove(istd::CIndex2d position);
	virtual TouchState IsTouched(istd::CIndex2d position, IInteractiveShape** shapePtrPtr = NULL) const;

	// reimplemented (iview::ISelectable)
	virtual int GetSelectedShapesCount() const;
	virtual void InsertSelectedShapes(SelectedShapes& result) const;
	virtual void DeselectAllShapes();
	virtual int GetKeysState() const;
	virtual int GetEditMode() const;
	virtual void OnShapeFocused(IInteractiveShape* shapePtr);
	virtual void OnShapeDefocused(IInteractiveShape* shapePtr);
	virtual void OnShapeSelected(IInteractiveShape& shape, bool state = true);

	// reimplemented (iview::ITouchable)
	virtual TouchState IsTouched(istd::CIndex2d position) const;

	// reimplemented (iview::IDraggable)
	virtual void BeginDrag(const istd::CIndex2d& reference);
	virtual void SetDragPosition(const istd::CIndex2d& position);
	virtual void EndDrag();
	virtual bool IsDraggable() const;
};


} // namespace iview


#endif // !iview_CToolsViewLayer_included


