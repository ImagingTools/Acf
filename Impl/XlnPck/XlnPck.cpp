#include "icomp/export.h"

#include "XlnPck.h"


namespace XlnPck
{


I_EXPORT_DEFAULT_SERVICES;

I_EXPORT_PACKAGE("Xln", "Xml processing package based on XERCES and XALAN", "XML XERCES XALAN XSLT Transformation Parser DOM");

I_EXPORT_COMPONENT(XmlFileSerializer, "XERCES based file loader serializing objects using XML format", "File XERCES Loader Load Save Serializer Archive XML Format");


} // namespace XlnPck


