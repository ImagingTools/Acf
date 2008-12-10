#include "icomp/export.h"

#include "iqt/CDefaultServicesProvider.h"

#include "QsciPck.h"


namespace QsciPck
{


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);


I_EXPORT_PACKAGE("Qsci", "QScintilla package", "QScintilla Qt Text");


I_EXPORT_COMPONENT(ScintillaTextEditor, "Scintilla based text editor", "Qt Text Editor GUI Scintilla");


} // namespace QsciPck


