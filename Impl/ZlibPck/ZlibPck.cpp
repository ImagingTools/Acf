#include "icomp/export.h"

#include "ZlibPck.h"


namespace ZlibPck
{


I_EXPORT_DEFAULT_SERVICES;

I_EXPORT_PACKAGE("Zlib", "Package based on compression library ZLIB", "ZLIB Compression File Archive Load Save");

I_EXPORT_COMPONENT(GzXmlFileSerializer, "Compressed XML file serializer", "Compression Compressed Serializer File Archive Load Save GZ ZLIB ZIP");


} // namespace ZlibPck


