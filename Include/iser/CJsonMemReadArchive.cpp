#include <iser/CJsonMemReadArchive.h>


namespace iser
{


// public methods

CJsonMemReadArchive::CJsonMemReadArchive(const QByteArray &inputString)
	: CJsonReadArchiveBase()
{
	initArchive(inputString);
}


} // namespace iser


