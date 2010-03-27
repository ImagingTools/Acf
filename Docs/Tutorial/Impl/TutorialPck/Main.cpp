#include "icomp/export.h"

#include "TutorialPck.h"


namespace TutorialPck
{


I_EXPORT_PACKAGE("Tutorial", "Tutorial package", "Qt Tutorial");

I_EXPORT_COMPONENT(
			TextEditor,
			"Text editor being observer of text model",
			"Text Tutorial Document Observer Qt GUI",
			icomp::IComponentStaticInfo::CCT_GUI | icomp::IComponentStaticInfo::CCT_DATA_PRESENTATION);


} // namespace TutorialPck


