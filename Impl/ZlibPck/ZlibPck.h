#ifndef ZlibPck_included
#define ZlibPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "ibase/TFileSerializerComp.h"

#include "izlib/CGzXmlReadArchive.h"
#include "izlib/CGzXmlWriteArchive.h"


/**
	Package besed on external ZLIB library.
*/
namespace ZlibPck
{


typedef ibase::TFileSerializerComp<izlib::CGzXmlReadArchive, izlib::CGzXmlWriteArchive> GzXmlFileSerializer;


} // namespace ZlibPck


#endif // !ZlibPck_included


