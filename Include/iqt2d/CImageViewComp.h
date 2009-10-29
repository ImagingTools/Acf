#ifndef iqt2d_CImageViewComp_included
#define iqt2d_CImageViewComp_included


#include "iqtgui/TGuiObserverWrap.h"

#include "iqt2d/CImageShape.h"
#include "iqt2d/CSceneProviderGuiComp.h"


namespace iqt2d
{


/**
	Show observed bitmap on the scene.
	This class extends standard scene provider to provide background bitmap object.
*/
class CImageViewComp: public iqtgui::TGuiObserverWrap<CSceneProviderGuiComp, CImageShape>
{
public:
	typedef iqtgui::TGuiObserverWrap<CSceneProviderGuiComp, CImageShape> BaseClass;

	I_BEGIN_COMPONENT(CImageViewComp)
		I_REGISTER_INTERFACE(imod::IObserver)
		I_ASSIGN(m_isFrameVisibleAttrPtr, "IsImageFrameVisible", "If true, image frame will be visible", true, false);
		I_ASSIGN(m_imagePositionModeAttrPtr, "ImagePositionMode", "Mode of image position:\n 0 - corner\n 1 - center", true, 0);
	I_END_COMPONENT

	CImageViewComp();

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	I_ATTR(bool, m_isFrameVisibleAttrPtr);
	I_ATTR(int, m_imagePositionModeAttrPtr);
};


} // namespace iqt2d


#endif // !iqt2d_CImageViewComp_included


