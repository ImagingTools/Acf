#ifndef XlnPck_included
#define XlnPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "ibase/TFileSerializerComp.h"

#include "ixln/CFileXmlReadArchive.h"
#include "ixln/CFileXmlWriteArchive.h"


/**
	Standard Qt package.
*/
namespace XlnPck
{


typedef ibase::TFileSerializerComp<ixln::CFileXmlReadArchive, ixln::CFileXmlWriteArchive> XmlFileSerializer;


} // namespace XlnPck


#endif // !XlnPck_included


