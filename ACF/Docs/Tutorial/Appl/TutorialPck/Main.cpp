#include "icomp/export.h"
#include "icomp/TModelCompWrap.h"

#include "DocView/CTextEditorComp.h"
#include "DocView/CTextModelComp.h"


I_EXPORT_PACKAGE("Tutorial", "Tutorial package", "Qt Tutorial");
I_EXPORT_COMPONENT(icomp::TModelCompWrap<CTextModelComp>, TextModel, "Simple data model of text", "Text Tutorial Document Model Observer");
I_EXPORT_COMPONENT(CTextEditorComp, TextEditor, "Text editor being observer of text model", "Text Tutorial Document Observer Qt GUI");


