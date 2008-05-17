#include "icomp/export.h"

#include "BasePck.h"


namespace BasePck
{


I_EXPORT_PACKAGE("Base", "Base system-undependent general package", "Undependend Standard Base");
I_EXPORT_COMPONENT(XmlFileSerializer, "Simple file serializer using XML format", "File Serializer Archive XML Format");
I_EXPORT_COMPONENT(SingleDocumentTemplate, "Simple file serializer using XML format", "File Serializer Archive XML Format");
I_EXPORT_COMPONENT(ApplicationInfo, "Simple application info based of SVN version", "Application Info SVN SubVersion Version");
I_EXPORT_COMPONENT(ComposedParamsSet, "Composed set of parameters", "Parameters Set List Composition Composed");
I_EXPORT_COMPONENT(FileBitmapAcquisitionParams, "Parameters for bitmap acquisition from file", "Bitmap Acquisition Snap Parameters");


} // namespace BasePck


