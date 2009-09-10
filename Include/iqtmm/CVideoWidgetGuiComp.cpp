#include "iqtmm/CVideoWidgetGuiComp.h"


namespace iqtmm
{


// public methods

// reimplemented (CGuiComponentBase)

void CVideoWidgetGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_videoPathAttrPtr.IsValid()){
		Phonon::MediaObject* mediaPtr = new Phonon::MediaObject(this);
		mediaPtr->setCurrentSource(iqt::GetQString(*m_videoPathAttrPtr));

		Phonon::createPath(mediaPtr, GetQtWidget());

		mediaPtr->play();
	}	
}


void CVideoWidgetGuiComp::OnGuiDestroyed()
{
	BaseClass::OnGuiDestroyed();
}


} // namespace iqtmm


